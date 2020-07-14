#include <stdlib.h>
#include <stdio.h>

int function27(char *first, char* second);
char* function28(char* input, int length);
int function29(char* string);

char skip = {{ simple_int_range(varname = 'K', min = 105, max = 115) }};

int phase_3(char *server_input, char *student_input) {
    int length;
    char *mixed;

    if (student_input == NULL) {
        return 1;
    }

    length = function29(server_input);

    mixed = function28(server_input, length);
    if (function27(student_input, mixed) == 1) {
        return 1;
    }

    return 0; 
}

char* function28(char* input, int length) {
    int left, right, state, index;
    char* jumpstring;

    jumpstring = malloc((length + 1) * sizeof(char));

    left = 0;
    right = length - 1;
    state = 0;
    index = 0;
    while (right >= left) {
        if (state == 0) {
            jumpstring[index++] = input[left++];
            if (input[left] > skip) {
                state = 1;
            }
        } else {
            jumpstring[index++] = input[right--];
            if (input[right] <= skip) {
                state = 0;
            }
        }
    }
    jumpstring[index] = '\0';

    return jumpstring;
}

int function29(char* string) {
    if (string[0] == '\0') {
        return 0;
    }
    return 1 + function29(string + 1);
}

int function27(char *first, char* second) {
    int index;

    while(first[index] == second[index]) {
        if (first[index] == '\0' || second[index] == '\0')
            break;
        index++;
    }

    if (first[index] == '\0' && second[index] == '\0') {
        return 0;
    } else {
        return 1;
    }
}