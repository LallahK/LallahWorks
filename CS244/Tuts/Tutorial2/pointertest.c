#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
	int i;
	char *p;
	p = (char*)malloc(sizeof(char));
	*p = '*';
	i = strcmp(p, "*");
	
	printf("%d\n", i);

	return EXIT_SUCCESS;
}
