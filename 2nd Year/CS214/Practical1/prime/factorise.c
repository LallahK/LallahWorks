#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long unsigned int conv(char*);
int next_pr(int);
void factorise(int, int);

int main(int argc, char *argv[])
{
	long pr;

	if (argc != 2) {
		printf("usage: ./factorise <number>\n");
		return EXIT_FAILURE;
	}
	pr = conv(argv[1]);
	factorise(pr, 2);

	return EXIT_SUCCESS;
}

long unsigned int conv(char *s)
{
	char *c = s;
	unsigned int val = 0;

	while (*c == ' ' || *c == '\t') {
		c++;
	}

	while (*c >= 48 && *c <= 57) {
		val = 10 * val + (*c - 48);
		c++;
	}
	return val;
}

int next_pr(int a) {
	int count, i;
	int b = a;
	while (1) {
		count = 0;
		b++;
		
		for (i = 1; i <= b / 2.0; i++) {
			if (b % i == 0) {
				count++;
			}
		}
		if (count == 1) {
			return b;
		}
	}

	return 0;
}

void factorise(int f, int i) {
	int t;
	if (f == 1) {
		printf("\n");
		return;
	}
	if (i * i > f) {
		printf("%d^1\n", f);
		return;
	}

	if (f % i == 0) {
		t = 0;
		while (f % i == 0) {
			f = f / i;
			t++;
		}
		printf("%d^%d ", i, t);
	}
	factorise(f, next_pr(i));

	/*for (i = 2; i < pr; i = next_pr(i)) {
		if (pr % i == 0) {
			t = 0;
			while (pr % i == 0) {
				pr = pr / i;
				t++;
			}
			printf("%d^%d ", i, t);
		}
	}*/
}
