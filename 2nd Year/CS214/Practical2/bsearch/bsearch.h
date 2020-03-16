#ifndef BSEARCH_H
#define BSEARCH_H

/**
 * Searches a range in the specified array for the specified value using the
 * binary search algorithm.
 *
 * @param[in]  n    the value for which to search
 * @param[in]  list the array to search
 * @param[in]  low  the lower range boundary index (inclusive)
 * @param[in]  high the upper range boundary index (exclusive)
 * @return the index into the list where the value is located, or -1 if n is
 *             not in the list
 */
int binary_search(int n, int list[], int low, int high);

#endif
