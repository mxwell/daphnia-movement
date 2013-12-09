#include <cstdio>
#include <vector>
#include <algorithm>

typedef double ld;

const double leftV = -0.4;
const double rightV = 0.4;
const double deltaV = 0.01;

using namespace std;

int main(int argc, char **argv) {
	if (argc < 2) {
		puts("missing arguments");
		return 0;
	}
	FILE *in = fopen(argv[1], "rt");
	if (in == NULL) {
		printf("can't open file %s\n", argv[1]);
		return 0;
	}
	ld minVx = 1e9, maxVx = -1e9;
	ld minVy = 1e9, maxVy = -1e9;
	int n = (rightV - leftV) / deltaV + 1;
	vector<vector<int> > hits(n, vector<int>(n, 0));
	int total = 0;
	int lines = 0;
	for (;;) {
		ld t, X, Y, Vx, Vy, x, v, e, psi;
		if (fscanf(in, "%lf %lf %lf %lf %lf %lf %lf %lf %lf",
						&t, &X, &Y, &Vx, &Vy, &x, &v, &e, &psi) != 9)
			break;
		++lines;
		minVx = min(minVx, Vx);
		maxVx = max(maxVx, Vx);
		minVy = min(minVy, Vy);
		maxVy = max(maxVy, Vy);
		int ix = (Vx - leftV) / deltaV;
		int iy = (Vy - leftV) /  deltaV;
		if (0 <= ix && ix < n && 0 <= iy && iy < n) {
			++hits[ix][iy];
			++total;
		}
	}
	printf("total number of points: %d\n", lines);
	printf("total number of hits in [%lf,%lf][%lf,%lf]: %d\n",
		leftV, rightV, leftV, rightV, total);
	printf("Vx in [%lf, %lf]\n", minVx, maxVx);
	printf("Vy in [%lf, %lf]\n", minVy, maxVy);
	fclose(in);
	const char *output_filename = "out.txt";
	FILE *out = fopen(output_filename, "wt");
	if (out == NULL) {
		printf("can't open file %s to write\n", output_filename);
		return 0;
	}
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			fprintf(out, "%lf %lf %lf\n", leftV + i * deltaV, leftV + j * deltaV,
				static_cast<double>(hits[i][j]) / total);
		}
		fprintf(out, "\n");
	}
	fclose(out);
	return 0;
}
