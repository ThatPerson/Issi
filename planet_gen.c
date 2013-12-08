#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define G 6.67*pow(10, -11)
#define M 1.989*(pow(10,30)

typedef struct {
  float radius;
  float mass;
  float density;
  char * desc;
  char * living;
  char name[500];
  float solar_distance;
  float orbital_period;
} Planet;

char pre_name[500][500] = {"Plu", "Jup", "So", "Ea", "Ven", "Mer"};
char suc_name[500][500] = {"to", "iter", "lar", "rth", "us", "cur"};
int name_types = 6;
int living_types = 2;
int desc_types = 2;
char desc[500][500] = {"A swampy planet", "A marshland"};
char living[500][500] = {"infested by swamp rats", "overrun by bipeds"};

Planet planets[500];

Planet generate_planet(void) {
  Planet r;
  r.radius = rand()%7000 + 1000;
  float masskg = ((rand()%10)+1) * (pow(10, 12));
  r.density = masskg;
  r.solar_distance = ((rand()%1000)+1) * (pow(10, rand()%10+5));
  float area = 4/3*M_PI*(pow(r.radius, 3));
  
  r.mass = area * masskg;
  
  sprintf(r.name, "%s%s", pre_name[rand()%name_types], suc_name[rand()%name_types]);
  r.desc = desc[rand()%desc_types];
  r.living = living[rand()%living_types];
  
  r.orbital_period = (2*M_PI*sqrt((pow(r.solar_distance*1000, 3))/(G*M)))); // In Seconds
  
  return r;
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
  printf("Density: %g kg/m^3\n", r.density);
  printf("Distance from Sun: %gkm\n", r.solar_distance);
  
  printf("Orbital Period: %gs\n", r.orbital_period);
  
  printf("Speed: %fkm/s\n\n", (M_PI*(2*r.solar_distance))/r.orbital_period);
  
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