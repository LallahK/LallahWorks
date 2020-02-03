#include <stdio.h>
#include <stdlib.h>

int strend(char[], char[]);

int main(int argc, char *argv[])
{
	if (argc != 3) return EXIT_FAILURE;
	printf("%d", strend(argv[1], argv[2]));
	return EXIT_SUCCESS;
}

int strend(char *s, char *t)
{
	char *sp1 = s, *sp2 = t;
	while (*sp1 != '\0')
	{
		if (*sp1 != *sp2)
			sp2 = t;
		if (*sp1 == *sp2)
			sp2++;
		sp1++;
	} 	
	return (*sp2 == '\0') ? 1 : 0;
}
