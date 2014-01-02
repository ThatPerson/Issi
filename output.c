int print_system_headers(void) {
	printf("%10s %10s %10s %10s %10s %10s %10s %10s %10s %10s %10s %10s %10s\n", "System", "Planet", "Moon", "SMass",  "TMass", "Distance", "OrbitP"/*Period*/, "OrbitT"/*Travelled*/, "OrbitC"/*Completed*/, "Eccentric", "GravPull", "Radius", "Density");
	int i;
	for (i = 0; i < (10*13) + 13; i++) {
		printf("-");
	}
	printf("\n");
}

int print_system_table(System s, float current_time) {

	printf("%10s %10s %10s %10.2Lg %10.2Lg %10s %10s %10s %10s %10s %10d %10.2g %10s\n", s.name, "", "", s.solar_mass, s.total_mass, "", "", "", "", "", 1/*((G*s.solar_mass)/(pow(1, 2)))*/, s.radius, "");
	int i;
	for (i = 0; i < s.num_planets; i++) {
		//Vector v = total_grav(s.planets[i], s, current_time);
		printf("%10s %10s %10s %10.4Lg %10.4Lg %10.4g %10.4g %10.4g %10.4g %10d %10.4g %10d %10.4g\n", "", s.planets[i].name, "", s.planets[i].mass, s.planets[i].total_mass, s.planets[i].solar_distance, s.planets[i].orbital_period, s.planets[i].orbit_travelled, s.planets[i].orbit_travelled / s.planets[i].orbital_period, 0, ((G*s.planets[i].mass)/(pow(s.planets[i].radius, 2))), s.planets[i].radius, s.planets[i].density);
		 //"", s.planets[i].name, "", s.planets[i].mass, s.planets[i].total_mass, s.planets[i].solar_distance, s.planets[i].orbital_period, s.planets[i].orbit_travelled, s.planets[i].orbit_travelled / 1, 0, ((G*s.planets[i].mass)/(pow(s.planets[i].radius, 2))), s.planets[i].radius, s.planets[i].density
		int q;
		for (q = 0; q < s.planets[i].moon_number; q++) {
			printf("%10s %10s %10s %10.4Lg %10.4Lg %10.4g %10.4g %10.4g %10.4g %10d %10.4g %10d %10.4g\n", "", "", s.planets[i].moon[q].name, s.planets[i].moon[q].mass, s.planets[i].moon[q].mass, s.planets[i].moon[q].distance, s.planets[i].moon[q].orbital_period, s.planets[i].moon[q].orbit_travelled, s.planets[i].moon[q].orbit_travelled/s.planets[i].moon[q].orbital_period, 0, ((G*s.planets[i].moon[q].mass)/(pow(s.planets[i].moon[q].radius, 2))), s.planets[i].moon[q].radius, s.planets[i].moon[q].density);
		}
	}
}

int tabulate(System s, float time, float time_increment) {
	float current_time;
	print_system_headers();
	for (current_time = 0; current_time < time; current_time += time_increment) {
		print_system_table(s, current_time);
	}
	return 1;
}
