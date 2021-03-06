
#define G 6.67*pow(10, -11)
#define M 1.989*(pow(10,30))
#define R 695500

typedef struct {
	int radius;
	long double mass;
	float density;
	char name[500];
	float distance;
	float orbital_period;
	float orbit_travelled;
} Moon;
								 
typedef struct {
	int radius;
	long double mass;
	float density;
	char * desc;
	char * living;
	char name[500];
	float solar_distance;
	float orbital_period;
	float orbit_travelled;
	Moon moon[3];
	int moon_number;
	long double total_mass;
} Planet;

typedef struct {
	Planet planets[500];
	char name[500];
	int num_planets;
	int num_moons;
	long double solar_mass;
	long double total_mass;
	float radius;
	int soi;
} System;

typedef struct {
	float angle;
	float value;
} Vector;
