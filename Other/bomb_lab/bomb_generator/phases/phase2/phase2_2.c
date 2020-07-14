#include <stdlib.h>
#include <stdio.h>

void function7(int* output, int* val, int operation);

int numbers[9] = {1, 1, 1, 1, 1, 1, 1, 1, 1};

int phase_2(char *server_input, char *student_input) {
    int index, val, output, operation;
    char c;

    if (student_input == NULL) {
        return 1;
    }

    index = 0;
    val = 0;
    output = 0;
    operation = 1;
    while(1) {
        c = student_input[index++];
        if (c >= 49 && c <= 57) {
            if (numbers[c - 49] != 1) {
                return 1;
            }
            numbers[c - 49] = 0;

            val = val * 10 + (c - 48);
        } else if (c == 43) {
            function7(&output, &val, operation);
            operation = 1;
        } else if (c == 45) {
            function7(&output, &val, operation);
            operation = 2;
        } else if (c == '\0') {
            function7(&output, &val, operation);
            break;
        } else {
            return 1;
        }
    }

    for (index = 0; index < 9; index++) {
        if (numbers[index] == 1) {
            return 1;
        }
    }

    if (output != 100) {
        return 1;
    }

    return 0;
}

void function7(int* output, int* val, int operation) {
    if (operation == 1) {
        *output = *output + *val;
        *val = 0;
    } else if (operation == 2) {
        *output = *output - *val;
        *val = 0;
    }
}

int main(int argc, char *argv[]) {
    printf("%d\n", phase_2(argv[1], argv[2]));
}