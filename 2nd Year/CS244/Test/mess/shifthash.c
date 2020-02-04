#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int hashCode(char*);

int main(int argc, char *argv[]) {
	char *s = argv[1];
	printf("%d", hashCode(s));
}

static int hashCode(char *s) {
	int h = 0;
	int length = strlen(s);
    for (int i = 0; i < length; i++) {
	    h = (h << 5) | (h >> 27); // 5-bit cyclic shift of the running sum
		h += (int) s[i]; // add in next character
	}
	return h;
}
