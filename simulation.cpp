#include <cmath>
#include <cstdio>
#include <ctime>

#include <vector>

#include <point.h>
#include <progressbar.h>
#include <model.h>

using namespace std;

template<typename Number> inline Number sqr(const Number &x)
{
	return (x) * (x);
}

void generate_output_name(char *name)
{
	char timestamp_buf[64];
	time_t t1 = time(NULL);
	struct tm *t2 = localtime(&t1);
	strftime(timestamp_buf, sizeof(timestamp_buf), "%b%d-%H%M", t2);
	sprintf(name, "results/%s.log", timestamp_buf);
}

int main(int argc, char const *argv[])
{
	Model &model = Model::instance();
	if ( (argc < 2 && !model.load_cfg_from_file("config.lua")) ||
		(argc > 1 && !model.load_cfg_from_file(argv[1]))) {
		puts("error: can't load configuration");
		return 0;
	}
	int iterations = model.get_relaxation_iterations();
	vector<Point> ensemble;
	{
		int total = model.get_particles_nr();
		int rows = static_cast<int>(sqrt(static_cast<double>(total)));
		assert (rows > 0);
		int columns = (total + (rows - 1)) / rows;
		assert(rows * columns >= total);
		ld between_rows = model.get_Lsize() / (rows + 1);
		ld between_columns = model.get_Lsize() / (columns + 1);
		for (int x = 0; x < columns; ++x) {
			for (int y = 0; y < rows; ++y) {
				if (static_cast<int>(ensemble.size()) >= total)
					break;
				ld hor_pos = between_columns * (x + 1);
				ld ver_pos = between_rows * (y + 1);
				Point p(hor_pos, ver_pos, 
						0, 0 ,
						0.3 + (ensemble.size() % 5) * 0.1, 0,
						0, 0);
				ensemble.push_back(p);
			}
			if (static_cast<int>(ensemble.size()) >= total)
				break;
		}
	}
	ProgressBar progress;
	puts("\trelaxation");
	progress.start(iterations);
	for (int it = 0; it < iterations; ++it) {
		auto u = model.get_common_speed(ensemble);
		for (auto &p : ensemble) {
			p = model.heun_step(u.first, u.second, p);
			model.periodify(p);
		}
		progress.check_and_move(it);
	}
	progress.finish_successfully();
	iterations = model.get_observation_iterations();
	puts("\tobservation");
	progress.start(iterations);
	char output_name[128];
	generate_output_name(output_name);
	FILE *out = fopen(output_name, "wt");
	ld h = model.get_h();
	ld t = model.get_relaxation_iterations() * h;
	for (int it = 0; it < iterations; ++it) {
		auto u = model.get_common_speed(ensemble);
		for (auto &p : ensemble) {
			p = model.heun_step(u.first, u.second, p);
			model.periodify(p);
		}
		progress.check_and_move(it);
		if ((it & 511) == 511) {
			auto u = model.get_common_speed(ensemble);
			ld u_abs = sqrt(sqr(u.first) + sqr(u.second));
			fprintf(out, "%lf", u_abs);
			for (auto &p : ensemble)
				fprintf(out, "\t%s", p.pos_to_string());
			fprintf(out, "\n");
		}
		t += h;
	}
	progress.finish_successfully();
	puts("Done");
	printf("Results are put to %s\n", output_name);
	fclose(out);
	return 0;
}
