#include <stdlib.h>
#include <stdio.h>

int function15(int* block, int length);

int array[20] = {{ array_of_length(varname = 'K', length = 20, min = 2, max = 100) }};

int phase_2(char *server_input, char *student_input) {
    int max, local_max, length, *block, loop, input;

    max = 0;

    if (student_input == NULL) {
        return 1;
    }

    input = atoi(student_input);
    if (input == 0) {
        return 1;
    }

    length = atoi(server_input);
    block = malloc(length * sizeof(length));
    for (int i = 0; i < length; i++) {
        block[i] = 0;
    }

    loop = 0;
    for (int i = 0; i < 20; i++) {
        block[loop] = array[i];

        local_max = function15(block, length);
        if (local_max > max) {
            max = local_max;
        }
        loop = (loop + 1) % length;
    }

    if (input == max) {
        return 0;
    } else {
        return 1;
    }
}

int function15(int* block, int length) {
    int i, sum;

    sum = 0;
    for (i = 0; i < length; i++) {
        sum = sum + block[i];
    }

    return sum;
}

int main(int argc, char *argv[]) {
    printf("%d\n", phase_2(argv[1], argv[2]));
}