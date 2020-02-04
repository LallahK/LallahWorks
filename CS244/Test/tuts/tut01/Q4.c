#include <stdio.h>
#include <stdlib.h>

int get_word(char[]);
void cpy_str(char*, char*);
int str_cmp(char*, char*);

int main(int argc, char *argv[])
{
	int maxl = 0, l = 0;
	char buffer[1000], s[1000];
	while(str_cmp(s, "end")) {
		l = get_word(s);
		if (l > maxl) {
			maxl = l;
			cpy_str(buffer, s);
		}
	}
	printf("%s\n", buffer);
}

int get_word(char s[])
{
	char ch, *c = s;
	while ((ch = getchar()) != '\n') {
		*c++ = ch;
	}
	*c = '\0';
	return c - s;
}

void cpy_str(char* init, char* target)
{
	char *a = init, *b = target;
	while ((*a++ = *b++) != '\0') {}
}

int str_cmp(char* first, char* sec)
{
	char *a = first, *b = sec;
	while (*a == *b && *a == '\0') {}
	return (*a) - (*b);
}
