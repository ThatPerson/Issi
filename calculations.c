

Vector planetary_effect (Planet x, Planet y) {
	Vector l;
	float b = x.solar_distance;
	float a = y.solar_distance;
	float x_dof = (x.orbit_travelled / x.orbital_period) * 360;
	/*
	 * This while loop is here so if a planet is in its second orbit (ie, you have run the simulation for > orbital_period
	 * it will still be able to work. This should not be a problem (the code which increments the orbit travelled _should_
	 * reset it, but this is just to prove that there is no error here
	 */
	while (x_dof > 360) {
		x_dof -= 360;
	}
	float y_dof = (y.orbit_travelled / y.orbital_period) * 360;
	// See above
	while (y_dof > 360) {
		y_dof -= 360;
	}
	float C = y_dof - x_dof;
	
	/*
	 * if C > 0 then y is ahead of x, else x is ahead of y. Because it is only used in trignometric operators I see no need
	 * to absolute it, as ((cos(90))-(cos(-90)) = 0). It can therefore be used to tell us which direction the affect works,
	 * and be used to work out the eventual Vector angle.
	 */
	float A = asin((a*sin(C)) / sqrt((pow(a, 2)) + (pow(b, 2)) - (2*a*b*cos(C))));
	
	/*
	 * Now we start to work out the actual effect on the planet. This is done using the relationship F = GMm/r^2, in order
	 * to get the effect of the action. To do this, we need to use some trig to work out the distance between x and y. We
	 * can use the same relationship as with the other one, because r = (a*sin(C))/sin(A) would give the distance where
	 * a is the distance to y, C is.... well, C, and A is the calculated angle. This gives us r to put into the above 
	 * formula
	 */
	 
	float r = (a*sin(C))/sin(A);
	 
	l.value = (G*x.mass*y.total_mass)/(pow(r, 2));
 
	/*
	 * We need to normalise A so it is going anticlockwise around from the sun to the sun. This is done because A can be 
	 * the same value on either side, and it is needed to work out the eventual vector from all planets combined.
	 */
	
	if (C < 0)
		A += 180;
		
	l.angle = A;
	
	return l;
}

Vector total_grav(Planet a, System s, float time) {
	int i, q;
	Vector total_vector;
	/* 
	 * Because the angle of the vector is going anticlockwise from the sun, it goes to show that the angle of the effect
	 * from the sun must be at 0 degrees. We can use the standard formula to hence work out the effect from the sun on 
	 * the planet.
	 */
	total_vector.angle = 0;
	total_vector.value = (G*a.mass*s.solar_mass)/(pow(a.solar_distance, 2));
	/*
	 * We loop over all the planets and run the corresponding functions to work out the gravitational effect. We do not
	 * do this for the moons, as they are taken as the total_mass of the planet as it is unlikely that they will cause
	 * too much effect due to their close proximity to the other planet.
	 */
	for (i = 0; i < s.num_planets; i++) {
		
		total_vector = balance_vector(total_vector, planetary_effect(a, s.planets[i]));
	}
	/*
	 * The planet will not be affecting itself, unless it were somebody on the surface. Because of this, we can ignore
	 * this until we get to working out surface gravity. Despite this, the moons of the planet will affect it, so we
	 * do these here.
	 */
	for (i = 0; i < a.moon_number; i++) {
		/*
		 * We calculate these moons much like the other ones, only it is much easier; at the beginning, all moons are
		 * in the SOI in a like parallel to the sun. Because of this, we just need to work out the angle you have 
		 * changed, and then you can calculate the angle from the sun to use with balance_vector.
		 */
		Vector l;
		l.angle = (a.moon[i].orbit_travelled / a.moon[i].orbital_period) * 360;
		l.angle = 180 - l.angle;
		l.value = (G * a.mass * a.moon[i].mass) / (pow(a.moon[i].distance, 2));
		total_vector = balance_vector(total_vector, l);
	}
	return total_vector;
}

