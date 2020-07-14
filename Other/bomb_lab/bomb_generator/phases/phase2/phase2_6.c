#include <stdlib.h>
#include <stdio.h>

int ipow(int base, int exp);

int magic_int = {{ simple_int_range(varname = 'K', min = 100, max = 999) }};

int phase_2(char* server_input, char* student_input) {
    int input, value, calculated, rem, col, i;

    if (student_input == NULL) {
        return 1;
    }

    input = atoi(student_input);
    if (input == 0) {
        return 1;
    }
    value = atoi(server_input);

    rem = 0;
    calculated = 0;
    for (i = 2; i >= 0; i--) {
        col = magic_int % (int) ipow(10, i + 1);
        col = col / ((int) ipow(10, i));
        col = col + rem * 10;

        calculated = calculated * 10 + col / value;
        rem = col % value;
    }

    if (calculated != input) {
        return 1;
    }

    return 0;
}

int ipow(int base, int exp)
{
    int result = 1;
    for (;;)
        {
            if (exp & 1)
                result *= base;
            exp >>= 1;
            if (!exp)
                break;
            base *= base;
        }

    return result;
}