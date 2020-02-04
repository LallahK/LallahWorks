#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int **p;
	int i, j;
	
	p = (int **)malloc(sizeof(int *) * 10);

	for (j = 0; j < 10; j++) {
		p[j] = (int *)malloc(sizeof(int) * 10);
		for (i = 0; i < 10; i++) {
			p[j][i] = j * 10 + i;
		}
	}
#if	1
	for (j = 0; j < 10; j++) {
		for (i = 0; i < 10; i++) {
			printf("%i ", p[j][i]);
		}
		printf("\n");
	}
#endif
	
	for (i = 0; i < 10; i++) {
		free(p[i]);
	}
	free(p);
}
