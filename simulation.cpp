#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>

#include <vector>
#include <string>
#include <complex>

#include <point.h>
#include <progressbar.h>
#include <model.h>

using namespace std;

template<typename Number> inline Number sqr(const Number &x)
{
	return (x) * (x);
}

string generate_output_name()
{
	char timestamp_buf[64];
	time_t t1 = time(NULL);
	struct tm *t2 = localtime(&t1);
	strftime(timestamp_buf, sizeof(timestamp_buf), "%b%d-%H%M", t2);
	return "results/" + string(timestamp_buf);
}

ld rand_from_range(ld fm, ld to)
{
	return rand() * (to - fm) / RAND_MAX + fm;
}

/*#define DO_FFT*/

#ifdef DO_FFT
void fft_rec(vector<complex<double> > &);
#endif

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
		printf("Particles will be put on grid %dX%d\n", rows, columns);
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
						rand_from_range(-2.5, 2.5), 0,
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
		ld u_abs = sqrt(sqr(u.first) + sqr(u.second));
		if (std::isnan(u_abs)) {
			printf("\nError: common speed value "
				"appears to be NaN at it = %d\n", it);
			return 0;
		}
		if (u_abs > 1e6) {
			printf("\nError: common speed value "
				"appears to be too large at it = %d: %lf\n",
				it, u_abs);
			return 0;
		}
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
	string output_name = generate_output_name();
	string trace_name = output_name + ".log";
	FILE *out = fopen(trace_name.c_str(), "wt");
#ifdef DO_FFT
	vector<complex<ld> > speeds;
#endif
	ld h = model.get_h();
	ld t = model.get_relaxation_iterations() * h;
	int dump_mask = model.get_dumps_period_mask();
	for (int it = 0; it < iterations; ++it) {
		auto u = model.get_common_speed(ensemble);
		ld u_abs = sqrt(sqr(u.first) + sqr(u.second));
#ifdef DO_FFT
		speeds.push_back(complex<ld>(u_abs));
#endif
		if ((it & dump_mask) == dump_mask) {
			if (std::isnan(u_abs)) {
				printf("\nError: common speed value "
					"appears to be NaN at it = %d\n", it);
				fclose(out);
				return 0;
			}
			if (u_abs > 1e6) {
				printf("\nError: common speed value "
					"appears to be too large at it = %d: %lf\n",
					it, u_abs);
				fclose(out);
				return 0;
			}
			fprintf(out, "%lf %lf", t, u_abs);
			for (auto &p : ensemble)
				fprintf(out, "\t%s", p.pos_to_string());
			fprintf(out, "\n");
		}
		for (auto &p : ensemble) {
			p = model.heun_step(u.first, u.second, p);
			model.periodify(p);
		}
		progress.check_and_move(it);
		t += h;
	}
	progress.finish_successfully();
	puts("Done");
	printf("Results are put to %s\n", trace_name.c_str());
	fclose(out);

#ifdef DO_FFT
	int biggest_bit = 31;
	while ((static_cast<int>(speeds.size()) & (1 << biggest_bit)) == 0)
		--biggest_bit;
	int size_for_fft = 1 << biggest_bit;
	printf("Spectrum will be calculated on base of the last %d common speed values\n",
		size_for_fft);
	speeds.erase(speeds.begin(),
		speeds.begin() + (speeds.size() - size_for_fft));
	assert(static_cast<int>(speeds.size()) == size_for_fft);
	fft_rec(speeds);
	ld coef = 2 * M_PI / (size_for_fft * model.get_h());
	size_for_fft /= 2;
	string spectrum_name = output_name + "-spectrum.txt";
	FILE *speed_spectrum = fopen(spectrum_name.c_str(), "wt");
	ld max_so_far = 0;
	for (int i = 0; i < size_for_fft; ++i) {
		ld val = speeds[i].real();
		if (val < max_so_far / 30)
			continue;
		max_so_far = max(max_so_far, val);
		fprintf(speed_spectrum, "%lf\t%lf\n", i * coef, val);
	}
	printf("Spectrum is put to %s\n", spectrum_name.c_str());
	fclose(speed_spectrum);
#endif
	
	return 0;
}

