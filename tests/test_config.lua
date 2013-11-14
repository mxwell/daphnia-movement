integration = {
	iterations = {
		relaxation = 100,
		observation = 10 * 1000
	},
	time_step = 0.0051
}

model = {
	-- noise
	macro_noise 	= 0.001,
	motor_noise		= 0.0001,
	-- function \Gamma(x)
	Gamma_0	= 2,
	C		= 0.8,
	b		= 1,

	-- mass of particle
	M 		= 1,
	-- mass of motor
	m 		= 0.1,

	c 		= 0.12,
	d 		= 0.1,
	F0		= -0.1,
	gamma_0 = 0.22,
	k 		= 1,
	omega_0	= 0.1,
	q		= 1,
}
