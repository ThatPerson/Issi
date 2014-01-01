#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "definitions.c"
#include "parsi_module.c"
#include "calculations.c"
#include "planet_gen.c"

int main(int argc, char * argv[]) {
	srand(time(NULL));
	if (argc == 2) {
		int count = atoi(argv[1]);
		systeml = generate_system(count);
		print_system(systeml);

	}
	return 1;
}
