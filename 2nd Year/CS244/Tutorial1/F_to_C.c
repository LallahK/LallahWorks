#include <stdio.h> 
#include <stdlib.h>

int main(int argc, char *argv[]) {
	float fahr, celsius;
	int lower = atoi(argv[1]);
	int upper = atoi(argv[2]);
	int step = atoi(argv[3]);

    /*printf("%d %d %d", lower, upper, step);

	for (fahr = lower; fahr <= upper; fahr += step) {
		celsius = 5 * (fahr - 32) / 9;
		printf("%10.2f\t%10.2f\n", fahr, celsius);
		fahr += step;
	}*/

	return EXIT_SUCCESS;
}
