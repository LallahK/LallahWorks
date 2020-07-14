#include <stdlib.h>
#include <stdio.h>

int function9(int a, int b);

int phase_2(char* server_input, char* student_input) {
    int special_number, input, count, index;

    if (student_input == NULL) {
        return 1;
    }

    input = atoi(student_input);
    if (input == 0) {
        return 1;
    }
    special_number = atoi(server_input);

    count = 1;
    for (index = 2; index < special_number; index++) {
        if (function9(index, special_number) == 1) {
            count++;
        }
    }

    printf("%d\n", count);

    if (input != count) {
        return 1;
    }
    return 0;
}

int function9(int a, int b) {
    if (a == 0) {
        return b;
    } else {
        return function9(b % a, a);
    }
}

int main(int argc, char *argv[]) {
    printf("%d\n", phase_2(argv[1], argv[2]));
}