#include <stdio.h>
#include <stdlib.h>
#include "length.h"

int str_len1(char*);

/*int main(int argc, char *argv[])
{
	char *buffer = "Abdullah";
	printf("%d %d", str_len1(buffer), str_len2(buffer));
}*/

int str_len1(char *s)
{
	int i = 0;
	char *c = s;
	while (*c++ != '\0') {
		i++;
	}
	return i;
}

int str_len2(char *s)
{
	char *c = s;
	while (*c++ != '\0') {}
	return c - s - 1;
}
