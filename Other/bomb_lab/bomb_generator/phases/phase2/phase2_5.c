#include <stdlib.h>
#include <stdio.h>

void function22(int* sum, int value, int col);
int ipow(int base, int exp);

int values[12] = {{ array_of_length(varname = 'K',length = 12, min = 0, max = 999) }};

int phase_2(char* server_input, char* student_input) {
    int input, magical, total, value, i, j;

    if (student_input == NULL) {
        return 1;
    }

    input = atoi(student_input);
    if (input == 0) {
        return 1;
    }
    magical = atoi(server_input);

    total = 0;
    for (i = 0; i < 3; i++) {
        value = 0;
        for (j = 0; j < 12; j++) {
            if (j % magical == 0)
                function22(&value, values[j], i);
        }
        total = total + (int) ipow(10, i) * value;
    }

    if (input != total) {
        return 1;
    }

    return 0;
}

void function22(int* sum, int value, int col) {
    int mod;
    mod = (value % ipow(10, col + 1));
    mod = mod / ipow(10, col);
    *sum += mod;
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