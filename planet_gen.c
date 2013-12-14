#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define G 6.67*pow(10, -11)
#define M 1.989*(pow(10,30))

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
	Moon moon[3];
	int moon_number;
} Planet;

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

Planet planets[500];

Moon generate_moon(Planet p) {
	Moon r;
	int l = ((p.radius/2)-1000);
	r.radius = rand()%l + 1000;
	float masskg = ((rand()%3)+1)*(pow(10, 12));
	r.density = masskg;
	float area = 4/3*M_PI*(pow(r.radius, 3));
	//strcpy(r.name, "Hello");
	r.mass = area*masskg;
	sprintf(r.name, "%s%s", mon_pre_name[rand()%mon_name_types], mon_suc_name[rand()%mon_name_types]);
	/* Here we use the idea of a sphere of influence to calculate the maximum distance the moon can orbit at.
	 * it is defined as T_SOI = a(m/M)^2/5, where a is the orbit of the planet around the sun, m and M are
	 * the masses of the sun and the planet */
	float t_soi = (p.solar_distance)*(pow((p.mass / M), (2/5)));
	r.distance = rand()%((int)t_soi-70) + 70;
	r.orbital_period = (2*M_PI*sqrt((pow(r.distance*1000, 3))/(G*p.mass)));
	return r; 
}
								 
Planet generate_planet(void) {
	Planet r;
	r.radius = rand()%7000 + 1000;
	float masskg = ((rand()%10)+1) * (pow(10, 12));
	r.density = masskg;
	/*We do not calculate the SOI here as there are no other objects which could affect the orbit, so it just orbits
	 * the sun no matter the distance.*/
	r.solar_distance = ((rand()%1000)+1) * (pow(10, rand()%10+5));
	float area = 4/3*M_PI*(pow(r.radius, 3));
	
	r.mass = area * masskg;
	
	sprintf(r.name, "%s%s", pre_name[rand()%name_types], suc_name[rand()%name_types]);
	r.desc = desc[rand()%desc_types];
	r.living = living[rand()%living_types];
	
	r.orbital_period = (2*M_PI*sqrt((pow(r.solar_distance*1000, 3))/(G*M))); // In Seconds
	
	r.moon_number = rand()%3;
	
	int i;
	for (i = 0; i < r.moon_number; i++) {
		r.moon[i] = generate_moon(r);
	}
	
	return r;
}

int print_moon(Moon p, int indent) {
	printf("\t%s\n\t", p.name);
	int q;
	for (q = 0; q < strlen(p.name); q++) {
		printf("-");
	}	
	printf("\n\tMass: %gkg\n", p.mass);
	printf("\tRadius: %gkm\n", p.radius);
	printf("\tDensity: %gkg/m^3\n", p.density);
	printf("\tDistance: %gkm\n", p.distance);
	printf("\tOrbital Period: %gs\n", p.orbital_period);
	printf("\tSpeed: %gkm/s\n", (M_PI*(2*p.distance))/p.orbital_period);
	
	return 1;
}
int print_planet(Planet r) {
	printf("%s\n", r.name);
	int q;
	for (q = 0; q < strlen(r.name); q++) {
		printf("-");
	}
	printf("\n");
	printf("%s %s\n", r.desc, r.living);
	printf("Mass: %gkg\n", r.mass);
	printf("Radius: %gkm\n", r.radius);
	printf("Density: %gkg/m^3\n", r.density);
	printf("SOI: %gkm\n", (r.solar_distance)*(pow((r.mass / M), (2/5))));
	printf("Distance from Sun: %gkm\n", r.solar_distance);
	
	printf("Orbital Period: %gs\n", r.orbital_period);
	
	printf("Speed: %fkm/s\n", (M_PI*(2*r.solar_distance))/r.orbital_period);
	
	printf("Moons: %d\n", r.moon_number);
	
	int i;
	for (i = 0; i < r.moon_number; i++) {
		printf("\n");
		print_moon(r.moon[i], 1);
	 }
	printf("\n\n");
	return 1;
}

int main(int argc, char * argv[]) {
	srand(time(NULL));
	if (argc == 2) {
		int count = atoi(argv[1]);
		int i;

		for (i = 0; i < count; i++) {
			planets[i] = generate_planet();
			print_planet(planets[i]);
		}
	}
	return 1;
}
