#include <stdlib.h>
#include <stdio.h>

int function32(char *first, char* second);
char* function33(char *input, int length);
int function34(char* string);

int rotations[2] = {{ array_of_length(varname = 'K', length = 2, min = 2, max = 5) }};

int phase_3(char *server_input, char *student_input) {
    char *mixed;
    int length;

    if (student_input == NULL) {
        return 1;
    }

    length = function34(server_input);

    mixed = function33(server_input, length);
    if (function32(student_input, mixed) == 1) {
        return 1;
    }

    return 0; 
}

char* function33(char* input, int length) {
    char *choicestring, *rotstring;
    int i, x, index;

    rotstring = malloc((length + 1) * sizeof(char));

    for (i = 0; i < length; i++) {
        if (i + rotations[0] >= length) {
            x = i + rotations[0] - length;
        } else {
            x = i + rotations[0];
        }

        rotstring[x] = input[i];
    }
    rotstring[length] = '\0';

    choicestring = malloc((length + 1) * sizeof(char));

    index = 0;
    for (i = 0; i < rotations[1]; i++) {
        x = i;
        while (x < length) {
            choicestring[index++] = rotstring[x];

            x = x + rotations[1];
        }
    }
    choicestring[length] = '\0';

    return choicestring;

}

int function34(char* string) {
    if (string[0] == '\0') {
        return 0;
    }
    return 1 + function34(string + 1);
}

int function32(char *first, char* second) {
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