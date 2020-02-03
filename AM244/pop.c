#include <stdlib.h>
#include <stdio.h>
#include <math.h>

double pop[] = {log(3.929), log(5.308), log(7.24), log(9.638), log(12.866), 
	log(17.069), log(23.192), log(31.433), log(38.588), log(50.156), 
	log(62.948), log(75.996), log(91.972), log(105.711), log(122.775), 
	log(131.669), log(150.927)};
double grad[15];

int main(int argc, char *argv[]) {
	double a = 0, b = 0, c = 0, m , d, x = 0;
	int i = 0;

	for (i = 0; i < 16; i++) {
		grad[i] = (1 / pop[i]) * ((pop[i + 1] - pop[i]) / 10);
		printf("%f\n", grad[i]);

		a = a + grad[i];
		x = x + pop[i] * pop[i];
		b = b + pop[i];
		c = c + pop[i] * grad[i];
	}

	m = (160 * c - a * b)/(x * 160 - b * b);
	x = (a - m * b) / 160;
	printf("%f %f\n", m, x);

}
