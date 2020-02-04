#ifndef TUT3_H
#define TUT3_H

/**
 * Compares x and y, and returns the largest value.  If the two values are
 * equal, then either x or y may be returned.
 *
 * @param[in]  x the first value to compare
 * @param[in]  y the second value to compare
 * @return the largest value of the given inputs
 */
int max(int x, int y);

/**
 * Computes x to the power of y.
 *
 * @param[in]  x the base (x >= 0)
 * @param[in]  y the exponent (y >= 0)
 * @return x to the power of y
 */
int power(int x, int y);

/**
 * Returns the greatest common divisor of the two parameter values.  This
 * function uses Euclid's algorithm.
 *
 * @param[in]  m the first value (m >= 0)
 * @param[in]  n the second value (n >= 0)
 * @return the GCD of the two parameter values
 */
int gcd(int m, int n);

#endif
