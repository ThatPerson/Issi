#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define G 6.67*pow(10, -11)
#define M 1.989*(pow(10,30))
#define R 695500

typedef struct {
	float radius;
	float mass;
	float density;
	char name[500];
	float distance;
	float orbital_period;
} Moon;
								 
typedef struct {
	float radius;
	float mass;
	float density;
	char * desc;
	char * living;
	char name[500];
	float solar_distance;
	float orbital_period;
	float orbit_travelled;
	Moon moon[3];
	int moon_number;
} Planet;

typedef struct {
	Planet planets[500];
	char name[500];
	int num_planets;
	int num_moons;
	float solar_mass;
	long long total_mass;
	float radius;
	int soi;
} System;

typedef struct {
	float angle;
	float value;
} Vector;

char pre_name[500][500] = {"Plu", "Jup", "So", "Ea", "Ven", "Mer"};
char suc_name[500][500] = {"to", "iter", "lar", "rth", "us", "cur"};
char mon_pre_name[500][500] = {"Mo", "Tit", "Miran"};
char mon_suc_name[500][500] = {"on", "an", "da"};
int mon_name_types = 3;
int name_types = 6;
int living_types = 2;
int desc_types = 2;
char desc[500][500] = {"A swampy planet", "A marshland"};
char living[500][500] = {"infested by swamp rats", "overrun by bipeds"};

char sol_pre_name[500][500] = {"Betel", "Sol", "Roat", "Long"};
char sol_suc_name[500][500] = {"geuse", "ar", "ley", "range"};
int sol_name_types = 4;

System systeml;

#include "parsi_module.c"

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
	 
	l.value = (G*x.mass*y.mass)/(pow(r, 2));
 
	/*
	 * We need to normalise A so it is going anticlockwise around from the sun to the sun. This is done because A can be 
	 * the same value on either side, and it is needed to work out the eventual vector from all planets combined.
	 */
	
	if (C < 0)
		A += 180;
		
	l.angle = A;
	
	return l;
}

Moon generate_moon(Planet p, System *s) {
	Moon r;
	//int l = (int)((p.radius/4));
	//r.radius = rand()%((int)(p.radius/4));
	r.radius = rand()%((int)(p.radius));
	float masskg = ((rand()%3)+1)*(pow(10, 12));
	r.density = masskg;
	float area = 4/3*M_PI*(pow(r.radius, 3));
	//strcpy(r.name, "Hello");
	r.mass = area*masskg;
	sprintf(r.name, "%s%s", mon_pre_name[rand()%mon_name_types], mon_suc_name[rand()%mon_name_types]);
	/* Here we use the idea of a sphere of influence to calculate the maximum distance the moon can orbit at.
	 * it is defined as T_SOI = a(m/M)^2/5, where a is the orbit of the planet around the sun, m and M are
	 * the masses of the sun and the planet */
	float t_soi = (p.solar_distance)*(pow((p.mass / s->solar_mass), (2/5)));
	r.distance = rand()%((int)t_soi-70) + 70;
	r.orbital_period = (2*M_PI*sqrt((pow(r.distance*1000, 3))/(G*p.mass)));
	s->num_moons++;
	//s->total_mass += r.mass;
	return r; 
}
								 
Planet generate_planet(System *s) {
	Planet r;
	
	float masskg = ((rand()%5)+1) * (pow(10, 12)); // Density
	r.density = masskg;
	/*We do not calculate the SOI here as there are no other objects which could affect the orbit, so it just orbits
	 * the sun no matter the distance.*/
	r.solar_distance = ((rand()%1000)+1) * (pow(10, rand()%10+5));
	
	r.radius = s->radius/(100*(rand()%10));
	
	r.mass = ((4/3)*M_PI*(pow(r.radius, 3))) * r.density;
	
	//printf("cuberoot(((%f/%f)/((4/3)*%f)))\n", r.mass, r.density, M_PI);
	sprintf(r.name, "%s%s", pre_name[rand()%name_types], suc_name[rand()%name_types]);
	r.desc = desc[rand()%desc_types];
	r.living = living[rand()%living_types];
	
	r.orbital_period = (2*M_PI*sqrt((pow(r.solar_distance*1000, 3))/(G*s->solar_mass))); // In Seconds
	//s->total_mass += r.mass;
	
	r.moon_number = rand()%3;
	if (r.mass < 1000) 
		r.moon_number = 0;
	
	int i;
	for (i = 0; i < r.moon_number; i++) {
		r.moon[i] = generate_moon(r, s);
	}
	
	return r;
}

int print_moon(Moon p, int indent) {
	printf("\t\t%s\n\t\t", p.name);
	int q;
	for (q = 0; q < strlen(p.name); q++) {
		printf("-");
	}	
	printf("\n\t\tMass: %gkg\n", p.mass);
	printf("\t\tRadius: %gkm\n", p.radius);
	printf("\t\tDensity: %gkg/km^3\n", p.density);
	printf("\t\tDistance: %gkm\n", p.distance);
	printf("\t\tOrbital Period: %gs\n", p.orbital_period);
	printf("\t\tSpeed: %gkm/s\n", (M_PI*(2*p.distance))/p.orbital_period);
	
	return 1;
}
int print_planet(Planet r, System s) {
	printf("\t%s\n\t", r.name);
	int q;
	for (q = 0; q < strlen(r.name); q++) {
		printf("-");
	}
	printf("\n");
	printf("\t%s %s\n", r.desc, r.living);
	printf("\tMass: %gkg\n", r.mass);
	printf("\tRadius: %gkm\n", r.radius);
	printf("\tDensity: %gkg/km^3\n", r.density);
	printf("\tSOI: %gkm\n", (r.solar_distance)*(pow((r.mass / s.solar_mass), (2/5))));
	printf("\tDistance from Sun: %gkm\n", r.solar_distance);
	
	printf("\tOrbital Period: %gs\n", r.orbital_period);
	
	printf("\tSpeed: %fkm/s\n", (M_PI*(2*r.solar_distance))/r.orbital_period);
	
	printf("\tMoons: %d\n", r.moon_number);
	
	int i;
	for (i = 0; i < r.moon_number; i++) {
		printf("\n");
		print_moon(r.moon[i], 1);
	 }
	printf("\n\n");
	return 1;
}

int print_system(System s) {
	printf("The %s System\n", s.name);
	int i;
	for (i = 0; i < strlen(s.name)+ 11; i++) {
		printf("-");
	}
	printf("\n");
	printf("Solar Mass: %gkg\n", s.solar_mass);
	//printf("Total Mass: %gkg\n", s.total_mass);
	printf("Number of Planets: %d\n", s.num_planets);
	printf("Number of Moons: %d\n", s.num_moons);
	printf("\n");
	for (i = 0; i < s.num_planets; i++) {
		print_planet(s.planets[i], s);
	}
	return 1;
}

System generate_system(int count) {
	System s;
	sprintf(s.name, "%s%s", sol_pre_name[rand()%sol_name_types], sol_suc_name[rand()%sol_name_types]);
	s.num_planets = rand()%50;
	float density = 1.45*pow(10,11);
	s.radius = R;
	s.solar_mass = (((4/3)*M_PI*pow(s.radius, 3)))*density;
	s.total_mass = s.solar_mass;
	s.num_moons = 0;
	int q;
	for (q = 0; q < s.num_planets; q++) {
		s.planets[q] = generate_planet(&s);
	}
	return s;
}

int main(int argc, char * argv[]) {
	srand(time(NULL));
	if (argc == 2) {
		int count = atoi(argv[1]);
		systeml = generate_system(count);
		print_system(systeml);

	}
	return 1;
}
