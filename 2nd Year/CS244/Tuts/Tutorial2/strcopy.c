#include <stdio.h>
#include <stdlib.h>

int strcopy(char[], char[]);

int main(int argc, char *argv[])
{
	return EXIT_SUCCESS;
}

void strcopy(char *to, char *from)
{
	int i = 0;
	while ((to[i] = from[i]) != '\0')
		i++;
}

void strcopy2(char *to, char *from)
{
	char *sp = from;
	while ((*to++ = *sp++) != '\0');
}
