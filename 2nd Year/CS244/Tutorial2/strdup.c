#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *strdup(char[]);

int main(int argc, char *argv[])
{
	char *sp = malloc(sizeof(char) * strlen(argv[1]));
	sp = strdup(argv[1]);
	
	return EXIT_SUCCESS;
}

char *strdup(char *s)
{
	char *sp = s;
	while ((*sp++ = *s++) != '\0');
	return s;
}

