#include <model.h>
#include <cmath>
#include <cassert>
#include <cstring>
#include <gasdev.h>

Model &Model::instance()
{
	static Model theSingleInstance;
	return theSingleInstance;
}

Model::Model()
{
#ifdef NOISE_ENABLED
	set_D_Mb(0.001);
	set_D_ms(0.0001);
#endif
	M = 1;
	m = 0.1;
	model_idumm = 7;
}

ld Model::get_Lsize()
{
	return Lsize;
}

void Model::set_Lsize(const ld &val)
{
	Lsize = val;
}

int Model::get_particles_nr()
{
	return particles_nr;
}

void Model::set_particles_nr(int val)
{
	particles_nr = val;
}

#ifdef NOISE_ENABLED
ld Model::get_D_Mb()
{
	return D_Mb;
}

ld Model::get_sqrt_2_D_Mb()
{
	return sqrt_2_D_Mb;
}

void Model::set_D_Mb(const ld &val)
{
	D_Mb = val;
	sqrt_2_D_Mb = sqrt(2 * D_Mb);
}

ld Model::get_D_ms()
{
	return D_ms;
}

ld Model::get_sqrt_2_D_ms()
{
	return sqrt_2_D_ms;
}

void Model::set_D_ms(const ld &val)
{
	D_ms = val;
	sqrt_2_D_ms = sqrt(2 * D_ms);
}
#endif

ld Model::get_F0_by_M()
{
	return F0_by_M;
}

ld Model::get_M()
{
	return M;
}

void Model::set_F0(const ld &val)
{
	F0 = val;
	F0_by_M = val / M;
}

void Model::set_M(const ld &val)
{
	M = val;
	F0_by_M = F0 / M;
	k_by_M = k / M;
	mu_by_M = mu / M;
}

ld Model::get_k_by_M()
{
	return k_by_M;
}

ld Model::get_k_by_m()
{
	return k_by_m;
}

void Model::set_k(const ld &val)
{
	k = val;
	k_by_M = k / M;
	k_by_m = k / m;
}

void Model::set_m(const ld &val)
{
	m = val;
	k_by_m = k / m;
}

void Model::set_mu(const ld &val)
{
	mu = val;
	mu_by_M = mu / M;
}

ld Model::get_mu_by_M()
{
	return mu_by_M;
}

ld Model::get_omega_0_2()
{
	return omega_0_2;
}

void Model::set_omega_0(const ld &val)
{
	omega_0 = val;
	omega_0_2 = omega_0 * omega_0;
}

void Model::set_Gamma_0(const ld &val)
{
	Gamma_0 = val;
}

void Model::set_b(const ld &val)
{
	b = val;
}

void Model::set_C(const ld &val)
{
	C = val;
}

void Model::set_d(const ld &val)
{
	d = val;
}

void Model::set_gamma_0(const ld &val)
{
	gamma_0 = val;
}

ld Model::get_gamma_0()
{
	return gamma_0;
}

void Model::set_c(const ld &val)
{
	c = val;
}

ld Model::get_c()
{
	return c;
}

void Model::set_q(const ld &val)
{
	q = val;
}

ld Model::get_nu()
{
	return nu;
}

void Model::set_nu(const ld &val)
{
	nu = val;
}

ld Model::get_psi0()
{
	return psi0;
}

void Model::set_psi0(const ld &val)
{
	psi0 = val;
}

ld Model::get_D_psi()
{
	return D_psi;
}

void Model::set_D_psi(const ld &val)
{
	D_psi = val;
	sqrt_2_D_psi = sqrt(2 * D_psi);
}

void Model::set_gauss_seed(int val)
{
	model_idumm = val;
}

ld Model::func_Gamma(const ld &xval)
{
	return Gamma_0 * (1 - C * tanh(b * xval));
}

void Model::set_h(const ld &val)
{
	h = val;
	sqrt_h = sqrt(h);
}

ld Model::get_h()
{
	return h;
}

void Model::set_relaxation_iterations(int val)
{
	relaxation_iterations = val;
}

int Model::get_relaxation_iterations()
{
	return relaxation_iterations;
}

void Model::set_observation_iterations(int val)
{
	observation_iterations = val;
}

int Model::get_observation_iterations()
{
	return observation_iterations;
}

bool Model::load_double(const char *key, bool verbose)
{
	if (lua_numberexpr(L, key, &loaded_double) == 0) {
		printf("problem w/ loading by key %s\n", key);
		return false;
	}
	if (verbose)
		printf("loaded by key %s: %lf\n", key, loaded_double);
	return true;
}

bool Model::load_int(const char *key)
{
	if (lua_intexpr(L, key, &loaded_int) == 0) {
		printf("problem w/ loading by key %s\n", key);
		return false;
	}
	return true;
}

bool Model::load_cfg_from_file(const char *filename)
{
	if (strstr(filename, ".lua") == NULL) {
		printf("error: expected Lua file: %s\n", filename);
		return false;
	}

	L = luaL_newstate();
	luaL_openlibs(L);
	if (luaL_dofile(L, filename) == 1) {
		printf("error: can't load config from %s\n", filename);
		return false;
	}
	if (!load_int("integration.iterations.relaxation"))
		return false;
	set_relaxation_iterations(loaded_int);
	if (!load_int("integration.iterations.observation"))
		return false;
	set_observation_iterations(loaded_int);
	printf("relaxation during %d, observation during %d iterations\n",
		relaxation_iterations, observation_iterations);
	if (!load_double("integration.time_step"))
		return false;
	set_h(loaded_double);
	printf("time_step %lf\n", h);
	if (!load_double("model.spatial_period"))
		return false;
	set_Lsize(loaded_double);
	printf("spatial period %lf\n", Lsize);
	if (!load_int("model.number_of_particles"))
		return false;
	set_particles_nr(loaded_int);
	printf("number of particles %d\n", particles_nr);
#ifdef NOISE_ENABLED
	if (!load_double("model.macro_noise", true))
		return false;
	set_D_Mb(loaded_double);
	if (!load_double("model.motor_noise", true))
		return false;
	set_D_ms(loaded_double);
#endif
	if (!load_double("model.Gamma_0", true))
		return false;
	set_Gamma_0(loaded_double);
	if (!load_double("model.C", true))
		return false;
	set_C(loaded_double);
	if (!load_double("model.b", true))
		return false;
	set_b(loaded_double);
	if (!load_double("model.M", true))
		return false;
	set_M(loaded_double);
	if (!load_double("model.m", true))
		return false;
	set_m(loaded_double);
	if (!load_double("model.mu", true))
		return false;
	set_mu(loaded_double);
	if (!load_double("model.c", true))
		return false;
	set_c(loaded_double);
	if (!load_double("model.d", true))
		return false;
	set_d(loaded_double);
	if (!load_double("model.F0", true))
		return false;
	set_F0(loaded_double);
	if (!load_double("model.gamma_0", true))
		return false;
	set_gamma_0(loaded_double);
	if (!load_double("model.k", true))
		return false;
	set_k(loaded_double);
	if (!load_double("model.omega_0", true))
		return false;
	set_omega_0(loaded_double);
	if (!load_double("model.q", true))
		return false;
	set_q(loaded_double);
	if (!load_double("model.nu", true))
		return false;
	set_nu(loaded_double);
	if (!load_double("model.psi0", true))
		return false;
	set_psi0(loaded_double);
	if (!load_double("model.angle_noise", true))
		return false;
	set_D_psi(loaded_double);
	if (!load_int("model.gauss_seed"))
		return false;
	set_gauss_seed(loaded_int);
	puts("seed for rng loaded");
	lua_close(L);
	return true;
}

Point Model::f(ld ux, ld uy, const Point &p)
{
	Point res;
	res.X = p.Vx;
	res.Y = p.Vy;
	ld Gamma = func_Gamma(p.x);
	res.Vx = -Gamma * p.Vx + F0_by_M + k_by_M * p.x * cos(p.psi) +
		mu_by_M * (ux - p.Vx);
	res.Vy = -Gamma * p.Vy + F0_by_M + k_by_M * p.x * sin(p.psi) +
		mu_by_M * (uy - p.Vy);
	res.x = p.v;
	res.v = (d * p.e - gamma_0) * p.v - (omega_0_2 + k_by_m) * p.x;
	res.e = q - c * p.e - m * d * p.v * p.v * p.e;
	res.psi = -nu * (res.psi - psi0);
	return res;
}

Point Model::g(const Point &p)
{
	Point res(0, 0, 0, 0, 0, 0, 0, 0);
	res.Vx = sqrt_2_D_Mb * gasdev(&model_idumm);
	res.Vy = sqrt_2_D_Mb * gasdev(&model_idumm);
	res.v = sqrt_2_D_ms * gasdev(&model_idumm);
	res.psi = sqrt_2_D_psi * gasdev(&model_idumm);
	return res;
}

Point Model::heun_step(ld ux, ld uy, const Point &p)
{
	Point f0 = f(ux, uy, p);
	Point g0 = g(p);
	Point v1 = p + f0 * h + g0 * sqrt_h;
	Point f1 = f(ux, uy, v1);
	Point g1 = g(v1);
	Point f_avg = (f0 + f1) / 2;
	Point g_avg = (g0 + g1) / 2;
	Point v2 = p + f_avg * h + g_avg * sqrt_h;
	return v2;
}

void Model::periodify(Point &p)
{
	while (p.X > Lsize - EPS)
		p.X -= Lsize;
	while (p.X < -EPS)
		p.X += Lsize;
	while (p.Y > Lsize - EPS)
		p.Y -= Lsize;
	while (p.Y < -EPS)
		p.Y += Lsize;
}

std::pair<ld, ld> Model::get_common_speed(const std::vector<Point> &ensemble)
{
	ld vx = 0, vy = 0;
	for (auto const &p : ensemble) {
		vx += p.get_Vx();
		vy += p.get_Vy();
	}
	int denom = static_cast<int>(ensemble.size());
	return std::make_pair(vx / denom, vy / denom);
}
