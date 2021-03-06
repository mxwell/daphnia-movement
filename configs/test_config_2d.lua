integration = {
	iterations = {
		relaxation = 0,
		observation = 100 * 1000 * 1000
	},
	time_step = 0.0005
}

model = {
	-- noise
	macro_noise 	= 0.001,
	motor_noise		= 0.0001,
	-- function \Gamma(x)
	Gamma_0	= 1,
	C		= 0.5,
	b		= 1,

	-- mass of particle
	M 		= 2,
	-- mass of motor
	m 		= 0.2,

	c 		= 0.1,
	d 		= 0.1,
	F0		= 0.0,
	--gamma_0 = 0.01,
	gamma_0 = 0.5,
	k 		= 0.1,
	--omega_0	= 0.5,
	omega_0	= 0.3,
	q		= 1,
	nu		= 0,
	psi0	= 0,
	angle_noise	= 0.005,
	gauss_seed	= 7
}
