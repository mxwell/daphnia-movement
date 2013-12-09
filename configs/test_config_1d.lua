integration = {
	iterations = {
		relaxation = 0,
		observation = 200 * 1000
	},
	time_step = 0.0005
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

	c 		= 0.1,
	d 		= 0.1,
	F0		= 0.0,
	gamma_0 = 0.2,
	k 		= 1,
	omega_0	= 0.1,
	q		= 1,
	nu		= 0,
	psi0	= 0,
	angle_noise	= 0.005,
	gauss_seed	= 7
}
