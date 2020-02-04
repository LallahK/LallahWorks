#include <stdio.h>
#include <stdlib.h>
#include "length.h"
#include "copy.h"

char* str_dup(char*);

char* str_dup(char *s)
{
	int i = str_len2(s);
	char *c = (char *)malloc(sizeof(char)*i);

	str_cpy(s, c);
	return c;
}
