#include <stdio.h>
#include <stdlib.h> 

int strlength(char[]);

int main(int argc, char *argv[]) {
	printf("%d", strlength2(argv[1]));
	return EXIT_SUCCESS;
}

int strlength(char *s) 
{
	return (sizeof(s)/sizeof(char));
}

int strlength2(char *s)
{
	char *sp = s;
	while (*sp++ != '\0');
	return sp - s - 1;
}
