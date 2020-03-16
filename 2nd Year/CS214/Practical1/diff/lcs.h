#ifndef LCS_H
#define LCS_H

/**
 * Allocates a two-dimensional array C, and populates C so that C[i][j] stores
 * the length of the LCS of x[0:i] and y[0:j].
 *
 * @param[in]     x    the first string, of length m
 * @param[in]     y    the second string, of length n
 * @return
 *     a two-dimensional array
 */
int **lcslen(const char *x, const char *y);

/**
 * Recursively reconstructs the LCS of the strings x and y, up to to indices i
 * and j, respectively, using the matrix C as computed by lcslen.  Space for
 * the result string s must be allocated by the caller.
 *
 * @param[in/out] s    the LCS
 * @param[in]     C    the matrix of LCS lengths
 * @param[in]     x    the first string
 * @param[in]     y    the second string
 * @param[in]     i    index ending the first string
 * @param[in]     j    index ending the second string
 */
void recon(char *s, int **C, const char *x, const char *y, int i, int j);

/**
 * Frees the two-dimensional int matrix with n rows.
 *
 * @param[in]     mat  the two-dimensional matrix to free
 * @param[in]     n    the number of rows in mat
 */
void freemat(int **mat, int m);

#endif

/* vim: textwidth=79 tabstop=4:
 */
