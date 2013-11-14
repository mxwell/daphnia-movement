#include "model.h"
#include "gtest/gtest.h"
#include <cmath>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

class ModelTest : public ::testing::Test {
protected:
	virtual void SetUp() {
		model = &Model::instance();
	}

	bool fp_eq(const ld &arg1, const ld &arg2) {
		return fabs(arg1 - arg2) < EPS;
	}

	Model *model;
};

TEST_F(ModelTest, DerivativesCalc) {
	ld F0 = -0.2;
	ld M = 1.2;
	model->set_F0(F0);
	model->set_M(M);

	ASSERT_TRUE(fp_eq(model->get_F0_by_M(), F0 / M))
		<< "expected automatical calculation of F0 / M";

	ld k = 1;
	model->set_k(k);

	ASSERT_TRUE(fp_eq(model->get_k_by_M(), k / M))
		<< "expected automatical calculation of k / M";

	ld m = 0.1;
	model->set_m(m);

	ASSERT_TRUE(fp_eq(model->get_k_by_m(), k / m))
		<< "expected automatical calculation of k / m";

	ld omega_0 = 0.2;
	model->set_omega_0(omega_0);

	ASSERT_TRUE(fp_eq(model->get_omega_0_2(), omega_0 * omega_0))
		<< "expected automatical calculation of omega_0^2";
}

TEST_F(ModelTest, GammaFunc)
{
	ld b = 1.1;
	ld xval = 0.33;
	ld C = 0.77;
	ld Gamma_0 = 1.9;

	model->set_b(b);
	model->set_C(C);
	model->set_Gamma_0(Gamma_0);

	ld shoulda_be = Gamma_0 * (1 - C * tanh(b * xval));

	ASSERT_TRUE(fp_eq(model->func_Gamma(xval), shoulda_be)) <<
		"values of function Gamma evaluations should be the same";
}

TEST_F(ModelTest, CheckConfigFile)
{
	ASSERT_TRUE(!model->load_cfg_from_file("foo.txt"))
		<< "not Lua file should be discarded";
	ASSERT_TRUE(!model->load_cfg_from_file("foo.lua"))
		<< "non-existing file should be discarded";
	ASSERT_TRUE(model->load_cfg_from_file("test_config.lua"))
		<< "config should be loaded from actual file";
	ASSERT_TRUE(model->get_relaxation_iterations() == 100)
		<< "value of relaxation iterations should be updated";
	ASSERT_TRUE(model->get_observation_iterations() == 10 * 1000)
		<< "value of observation iterations should be updated";
	ASSERT_TRUE(fp_eq(model->get_h(), 0.0051))
		<< "value of time step should be updated";
	ASSERT_TRUE(fp_eq(model->get_gamma_0(), 0.22))
		<< "value of gamma_0 should be updated";
	ASSERT_TRUE(fp_eq(model->get_c(), 0.12))
		<< "value of c should be updated";
}

TEST_F(ModelTest, PointsEquality)
{
	ld a = 1, b = 2, c = 3, d = 4, e = 5;
	Point p1(a, b, c, d, e);
	Point p2(a + 0.1, b, c, d, e);
	ASSERT_TRUE(p1 != p2);
	Point p3(a, b, c - 0.2, d, e);
	ASSERT_TRUE(p1 != p3);
	ASSERT_TRUE(p2 != p3);
	ASSERT_TRUE(p2 == p2) <<
		"point should be equal to itself";
	Point as_p2(a + 0.1, b, c, d, e);
	ASSERT_TRUE(p2 == as_p2) <<
		"points with the same fields should be equal";
	Point near_p3(a + 1e-9, b, c - 0.2, d, e);
	ASSERT_TRUE(p3 == near_p3) <<
		"very near points should be equal";
}
