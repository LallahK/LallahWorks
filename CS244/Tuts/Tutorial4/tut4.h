#ifndef TUT4_H
#define TUT4_H

/**
 * Returns n factorial.
 *
 * @param[in]  n the nonnegative numnber for which to compute the factorial
 * @return n factorial
 */
int fact(int n);

/**
 * Returns the value of the Ackermann function for the specified parameters.
 *
 * @param[in]  x the first parameter
 * @param[in]  y the second parameter
 * @return the value of the Ackermann function for the specified parameters
 */
int ackerman(int x, int y);

/**
 * Computes x to the power of y.  This function exponentiates recursively.
 *
 * @param[in]  x the base (x >= 0)
 * @param[in]  y the exponent (y >= 0)
 * @return x to the power of y
 */
int power(int x, int y);

/**
 * Exchanges the contents of the variables to which the two paramaters point.
 *
 * @param[in,out]  x the first variable
 * @param[in,out]  y the second variable
 */
void swap(int *x, int *y);

/*
 *       INPUT: int n - number to located
 *              int list[] - a sorted list of integers
 *              int low - lower list boundary
 *              int high - upper list boundary
 *      OUTPUT: int
 * DESCRIPTION: Returns the position inside 'list' containing
 *              the value 'n'. Returns -1 if 'n' could not be located
 */
/**
 * Searches a range in the specified array for the specified value using the
 * binary search algorithm.
 *
 * @param[in]  n    the value for which to search
 * @param[in]  list the array to search
 * @param[in]  low  the lower range boundary (inclusive)
 * @param[in]  high the upper range boundary (exclusive)
 * @return the index into the list where the value is located, or -1 if n is
 *             not in the list
 */
int binary_search(int n, int list[], int low, int high);

#endif
