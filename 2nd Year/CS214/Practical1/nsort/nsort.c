#include <stdlib.h>
#include <stdio.h>

#define BUFFER_SIZE 1024

void read_int(char*);
void resize();
void q_sort(int *v, int, int);
void swap(int *v, int, int);

int size = 200;
int *vals;
int i;

int main(int argc, char *argv[])
{
	FILE *in;
	char line[BUFFER_SIZE];
	int m;
	vals = malloc(sizeof(int) * size);

	i = 0;

	if (argc != 2) {
		return EXIT_FAILURE;
	}
	if ((in = fopen(argv[1], "r")) == NULL) {
		printf("./nsort: could not open file %s\n", argv[1]);
		return EXIT_FAILURE;
	}
	
	while (fgets(line, BUFFER_SIZE, in) != NULL) {
		read_int(line);
	}

	q_sort(vals, 0, i - 1);

	for (m = 0; m < i; m++) {
		printf("%d\n", vals[m]);
	}
	
	fclose(in);
	return EXIT_SUCCESS;	
}



void read_int(char *c)
{
	int sign = 1;
	long v;

	while (*c != '\n' || *c == '\0') {
		v = 0;
		sign = 1;

		while (*c == ' ' || *c == '\t') {
			c++;
		}

		if (*c == '-') {
			c++;
			sign = -1;
		}

		while (*c >= 48 && *c <= 58) {
			v = v*10 + (*c++ - 48);		     
		}
		v = v * sign;

		vals[i++] = v;
		if (i + 1 == size) {
			resize();
		}
	}

}

void resize() {
	int k;
	int *newvals = malloc(sizeof(int) * size);
	size = size * 2;
	
	for (k = 0; k < i; k++) {
		newvals[k] = vals[k];
	}
	free(vals);
	vals = newvals;
}

void q_sort(int v[], int left, int right)
{
	int i, last;
	if (left >= right) {
		return;
	}
	swap(v, left, (left + right) / 2);
	last = left;
	for (i = left + 1; i <= right; i++) {
		if (v[i] < v[left]) {
			swap(v, ++last, i);
		}
	}
	swap(v, left, last);
	q_sort(v, left, last - 1);
	q_sort(v, last + 1, right);
}

void swap(int v[], int i, int j)
{
	int temp;

	temp = v[i];
	v[i] = v[j];
	v[j] = temp;
}
