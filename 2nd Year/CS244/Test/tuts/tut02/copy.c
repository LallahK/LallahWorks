#include <stdio.h>
#include <stdlib.h>
#include "copy.h"

void str_cpy(char*, char*);

/*int main(int argc, char *argv[])
{
	char buffer[1000], *buff = "Popsicle";
	str_cpy(buff, buffer);
	printf("%s", buffer);
}*/

void str_cpy(char *ori, char *targ)
{
	char *a = ori, *b = targ;
	while ((*b++ = *a++) != '\0'){}
	*b = '\0';
}
