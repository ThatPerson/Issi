#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "definitions.c"
#include "parsi_module.c"
#include "calculations.c"
#include "planet_gen.c"
#include "output.c"

int main(int argc, char * argv[]) {
	srand(time(NULL));
	systeml = generate_system(5);
	tabulate(systeml, 1, 1);
	return 1;
}
