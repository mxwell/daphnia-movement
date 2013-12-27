#include <cmath>
#include <cstdio>
#include <ctime>

#include <vector>

#include <point.h>
#include <progressbar.h>
#include <model.h>

using namespace std;

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
	Point p(4, 6, 0, 0, 0.5, 0, 0, 0);
	ProgressBar progress;
	puts("\trelaxation");
	progress.start(iterations);
	for (int it = 0; it < iterations; ++it) {
		p = model.heun_step(p);
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
		p = model.heun_step(p);
		model.periodify(p);
		double vx = p.get_Vx();
		double vy = p.get_Vy();
		double v = sqrt(vx * vx + vy * vy);
		progress.check_and_move(it);
		if ((it & 511) == 511)
			fprintf(out, "%lf\t%s\n", v, p.pos_to_string());
		t += h;
	}
	progress.finish_successfully();
	puts("Done");
	printf("Results are put to %s\n", output_name);
	fclose(out);
	return 0;
}
