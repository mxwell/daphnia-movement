#include <vector>
#include <complex>
#include <cmath>

#define _USE_MATH_DEFINES

using namespace std;
typedef complex<double> base;

void fft_rec(vector<base> &v)
{
	int n = static_cast<int>(v.size());
	if (n == 1)
		return;
	int n2 = n / 2;

	vector<base> a0(n2), a1(n2);
	for (int i = 0, j = 0; i < n; i += 2, ++j) {
		a0[j] = v[i];
		a1[j] = v[i + 1];
	}
	fft_rec(a0);
	fft_rec(a1);

	double ang = 2 * M_PI / n;
	base w(1), wn(cos(ang), sin(ang));	
	for (int i = 0; i < n2; ++i) {
		v[i] 		= a0[i] + w * a1[i];
		v[i + n2] 	= a0[i] - w * a1[i];
		w *= wn;
	}
}

void test_fft(const char *spectrum_name)
{
	int n = 1024;
	vector<base> v(n);
	double dt = 0.01;
	for (int i = 0; i < n; ++i) {
		double x = i * dt;
		v[i] = base(sin(x) + 0.5 * sin(x * 2) + 2 * sin(x * 5));
	}

	fft_rec(v);
	FILE *out = fopen(spectrum_name, "w");
	int n2 = n / 2;
	double coef = 2 * M_PI / (n * dt);
	double a0 = v[0].real();
	double threshold = 0.01;
	for (int i = n2; i < n; ++i) {
		double value = v[i].real() / a0;
		if (fabs(value) > threshold)
			fprintf(out, "%lf %lf\n", coef * (i - n), value);
	}
	for (int i = 0; i < n2; ++i) {
		double value = v[i].real() / a0;
		if (fabs(value) > threshold)
			fprintf(out, "%lf %lf\n", coef * i, value);
	}
	fclose(out);
}
