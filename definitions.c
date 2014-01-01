
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
