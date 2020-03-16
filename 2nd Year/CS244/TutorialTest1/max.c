#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int *i = malloc(sizeof(int));
	int *max = malloc(sizeof(int));
	int *compare = malloc(sizeof(int));
	
	*max = atoi(argv[1]);
	while (*i < argc)
	{
		*compare = atoi(argv[*i]);
		(*i)++;
		if (*max < *compare)
			*max = *compare;
	}
	
	printf("%d\n", *max);
	free(i);
	free(max);
	free(compare);
	return EXIT_SUCCESS; 
}
