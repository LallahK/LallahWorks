#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lcs.h"

int main(int argc, char *argv[])
{
	int **C, m, n;
	char *s;

	if (argc != 3) {
		fprintf(stderr, "Usage: showdiff <string1> <string2>\n");
		exit(1);
	}

	C = lcslen(argv[1], argv[2]);
	m = strlen(argv[1]);
	n = strlen(argv[2]);
	s = malloc(((m < n ? m : n) + 1)*sizeof(char));
	recon(s, C, argv[1], argv[2], m, n);
	printf("%s\n", s);

	free(s);
	freemat(C, m);

	return EXIT_SUCCESS;
}
