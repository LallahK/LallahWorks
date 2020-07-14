#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int function20(char* student_input, int length);
int function21(char *string);

int automata[16] = {{ array_of_length(varname = 'K', length = 16, min = 0, max = 100) }};

int phase_4(char *server_input, char *student_input) {
    int length, index, state, tp_one, tp_two, val;
    char c;

    if (student_input == NULL) {
        return 1;
    }

    length = function21(student_input);
    if (function20(student_input, length) == 1) {
        return 1;
    }

    state = 0;
    index = 0;
    while(index < 12) {
        c = student_input[index];
        if (c == '\0') {
            return 1;
        }

        tp_one = automata[state];
        tp_two = automata[state + 1];
        val = server_input[index++];

        if (val < tp_one && val < tp_two) {
            if (c != 115) return 1;
        
        } else if (val >= tp_one && val < tp_two) {
            state = (state + 2) % 16;
            if (c != 110) return 1;

        } else if (val < tp_one && val >= tp_two) {
            state = (state + 4) % 16;
            if (c != 100) return 1;

        } else {
            if (tp_one < tp_two) {
                state = (state + 4) % 16;
                if (c != 100) return 1;

            } else {
                state = (state + 2) % 16;
                if (c != 110) return 1;
        
            }
        }
    }

    if (student_input[index] != '\0') {
        return 1;
    }

    return 0;
}


int function20(char* student_input, int length) {
    int i, valid;
    char c;
    
    for (i = 0; i < length; i++) {
        c = student_input[i];
        valid = 0;

        if (c == 110) valid = 1;
        else if (c == 115) valid = 1;
        else if (c == 100) valid = 1;

        if (valid == 0) {
            return 1;
        }
    }

    return 0;
}

int function21(char* string) {
    if (string[0] == '\0') {
        return 0;
    }
    return 1 + function21(string + 1);
}