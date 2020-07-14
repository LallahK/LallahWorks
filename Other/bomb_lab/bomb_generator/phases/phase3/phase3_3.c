#include <stdlib.h>
#include <stdio.h>

char* function21(char* input);
int function23(char *first, char* second);
void function26(char *input, char *skipstring, int jump, int state);

int jumps[3] = {{ array_of_length(varname = 'K', length = 3, min = 2, max = 5) }};;

int phase_3(char *server_input, char *student_input) {
    char *mixed;

    if (student_input == NULL) {
        return 1;
    }

    mixed = function21(server_input);
    if (function23(student_input, mixed) == 1) {
        return 1;
    }

    return 0; 
}

char* function21(char* input) {
    int index, jump;
    char* skipstring, c;

    skipstring = malloc(41 * sizeof(char));

    function26(input, skipstring, 1, 0);

    return skipstring;
}

void function26(char *input, char *skipstring, int jump, int state) {
    if (input[0] == '\0') {
        skipstring[0] = '\0';
        return;
    }

    if (state < jumps[0]) {
        skipstring[0] = input[0];
        function26(input + 1, skipstring + 1, jump, state + 1);
    } else {
        if (jump == 1) {
            jump = 2;
            function26(input + jumps[1], skipstring, jump, 0);
        } else {
            jump = 1;
            function26(input + jumps[2], skipstring, jump, 0);
        }
    }
}

int function23(char *first, char* second) {
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