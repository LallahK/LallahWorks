#include <stdio.h>
#include <stdlib.h>

int str_cmp(char*, char*);

int main(int argc, char *argv[])
{
	printf("%d", str_cmp("Dierd", "Dierdre"));
}

int str_cmp(char *c, char *s)
{
	char *a = c, *b = s;
	while (*a == *b && (*a != '\0' || *b != '\0')) {
		a++;
		b++;
	}
	return *a - *b;
}
