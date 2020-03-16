#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "lcs.h"

/*int main(int argc, char *argv[])
{
	int **a;
	a = lcslen("Abdullah", "Dierdre");

	return EXIT_SUCCESS;
}*/

int **lcslen(const char *x, const char *y)
{
	int lx, ly, i, j;
	int **mat;

	lx = strlen(x);
	ly = strlen(y);
	
	mat = (int **)malloc(sizeof(int *) * (lx + 1));
	for (i = 0; i < lx + 1; i++) {
		mat[i] = (int *)malloc(sizeof(int) * (ly + 1));
	}

	for (i = 0; i < lx + 1; i++) {
		mat[i][0] = 0;
	}
	for (j = 0; j < ly + 1; j++) {
		mat[0][j] = 0;
	}

	for (i = 1; i < lx + 1; i++) {
		for (j = 1; j < ly + 1; j++) {
			if (x[i - 1] == y[j - 1]) {
				mat[i][j] = mat[i - 1][j - 1] + 1;
			}
			else {
				mat[i][j] = (mat[i][j - 1] < mat[i - 1][j] ? mat[i - 1][j] :
				mat[i][j - 1]);
			}
		}
	}

	return mat;
}

void recon(char *s, int **C, const char *x, const char *y, int i, int j)
{
	int max;
	max = (strlen(x) > strlen(y)) ? strlen(x) : strlen(y);

	if (i == 0 || j == 0) {
		*s = '\0';
	} else if (x[i -1] == y[j - 1]) {
		recon(s, C, x, y, i - 1, j - 1);
		printf("%c", x[i - 1]);
		s[max - C[i][j]] = x[i -1];
	} else if (C[i - 1][j] > C[i][j - 1]) {
		recon(s, C, x, y, i - 1, j);
	} else {
		recon(s, C, x, y, i, j - 1);
	}
}

void freemat(int **mat, int m)
{
	int i;
	for (i = 0; i < m; i++) {
		free(mat[i]);
	}
	free(mat);
}
