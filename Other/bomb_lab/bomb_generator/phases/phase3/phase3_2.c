#include <stdlib.h>
#include <stdio.h>

char* function8();
int function14(char *first, char* second);
int function30(char* string);

int phase_3(char *server_input, char *student_input) {
    int length;
    char *mixed;

    if (student_input == NULL) {
        return 1;
    }

    length = function30(server_input);

    mixed = function8(server_input, length);
    if (function14(student_input, mixed) == 1) {
        return 1;
    }

    return 0; 
}

char* function8(char* string, int length) {
    int i, temp, one, two;
    char* fibostring;

    one = 0;
    two = 1;
    fibostring = malloc(15 * sizeof(char));
    for (i = 0; i < 15; i++) {
        fibostring[i] = string[(one + two) % length];

        temp = two;
        two = one + two;
        one = temp;
    }

    return fibostring;
}

int function30(char* string) {
    if (string[0] == '\0') {
        return 0;
    }
    return 1 + function30(string + 1);
}

int function14(char *first, char* second) {
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