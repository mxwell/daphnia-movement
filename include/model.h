#ifndef __SSU_KMY_PARAMS_H_
#define __SSU_KMY_PARAMS_H_

#include <types.h>
#include <point.h>
#include <luautils.h>

/* Class contains as fields parameters of simulation and model,
 * as methods getters and smart setters for parameters.
 * Model equations are described in methods.
 * Class implements the pattern 'Singleton', i.e. 
 * there could be only one instance of the class
 *
 * Note 1: in order to avoid errors D_M and D_m are
 * 		   replaced by D_Mb and D_ms
 * Note 2: support for external noise components eliminated
 *		   for a time being
 */

 /* #define NOISE_ENABLED */

class Model {
public:
	static Model &instance();

#ifdef NOISE_ENABLED
	ld get_D_Mb();
	ld get_sqrt_D_Mb();
	/* set D_Mb and its derivative */
	void set_D_Mb(const ld &val);

	ld get_D_ms();
	ld get_sqrt_D_ms();
	/* set D_ms and its derivative */
	void set_D_ms(const ld &val);
#endif

	/* returns (F0 / M) */
	ld get_F0_by_M();
	ld get_M();

	/* set F0 (or M) and its derivative */
	void set_F0(const ld &val);
	void set_M(const ld &val);

	/* returns (k / M) */
	ld get_k_by_M();
	/* returns (k / m) */
	ld get_k_by_m();
	/* set k (or m) and its derivative */
	void set_k(const ld &val);
	void set_m(const ld &val);
	ld get_omega_0_2();
	/* set omega_0 and its derivative */
	void set_omega_0(const ld &val);
	void set_Gamma_0(const ld &val);
	void set_b(const ld &val);
	void set_C(const ld &val);
	void set_d(const ld &val);
	void set_gamma_0(const ld &val);
	ld get_gamma_0();
	void set_c(const ld &val);
	ld get_c();
	void set_q(const ld &val);

	ld func_Gamma(const ld &xval);
	/* set h and its derivative */
	void set_h(const ld &val);
	ld get_h();
	void set_relaxation_iterations(int val);
	int get_relaxation_iterations();
	void set_observation_iterations(int val);
	int get_observation_iterations();

	bool load_cfg_from_file(const char *filename);

	/* deterministic part of r.h.s. */
	Point f(const Point &p);

	Point heun_step(const Point &p);

private:
	Model();
	bool load_double(const char *key, bool verbose = false);
	bool load_int(const char *key);

	/* parameters of model */
#ifdef NOISE_ENABLED
	ld D_Mb;
	ld sqrt_2_D_Mb;
	ld D_ms;
	ld sqrt_2_D_ms;
#endif

	ld b;
	ld C;
	ld c;
	ld d;
	ld F0;
	ld F0_by_M;
	ld Gamma_0;
	ld gamma_0;
	ld k;
	ld k_by_M;
	ld k_by_m;
	ld M;
	ld m;
	ld omega_0;
	ld omega_0_2;
	ld q;

	/* parameters of simulation */
	int relaxation_iterations;
	int observation_iterations;
	ld h;
	ld sqrt_h;

	/* fields for config loading from Lua file */
	lua_State *L;
	ld loaded_double;
	int loaded_int;
};

#endif /* __SSU_KMY_PARAMS_H_ */