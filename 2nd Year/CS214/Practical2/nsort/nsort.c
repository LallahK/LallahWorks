#include <stdlib.h>
#include <stdio.h>

void read_int(signed int*, short int);

#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
	int **arr;
	int *a;
	int size = BUFFER_SIZE;
	FILE *src;
	char *c;
	char buffer[BUFFER_SIZE];

 	a = malloc(sizeof(int) * size);	

	if (argc == 2) {
		if ((src = fopen(argv[1], "r")) == NULL) {
			printf("./nsort: could not open file %c%s%c\n", 34, argv[1], 34);
			return EXIT_FAILURE;
		}
		
	} else {
		
	}


	/*double int size = 0, i;
	signed int *arr;
	char *c;
	if (argc != 2) {
		return EXIT_FAILURE;
	}

	c = argv[1];
	while (*c != '\0') {
		size = size*10 + (*c++ - 48);
	}
	
	arr = (int *)malloc(sizeof(int) * size);

	for (i = 0; i < size; i++) {
		read_int(arr, i);
	}

	for (i = 0; i < size; i++) {
		printf("%d\n", arr[size - i - 1]);
	}

	free(arr);*/
	return EXIT_SUCCESS;
}

void resize(int **arr, int *old, int size)
{
	int i;
	int	*new;
	new = malloc(sizeof(int) * size * 2);

    for(i = 0; i < size; i++) {
		new[i] = old[i];
	}
    free(old);
	*arr = new;
}

void read_int(signed int *a, short int i)
{
	char buffer[BUFFER_SIZE];
	char *c;
	signed int v = 0;
	signed int sign = 1;
	
	scanf("%s", buffer);

	c = buffer;
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
	a[i] = v;
}
