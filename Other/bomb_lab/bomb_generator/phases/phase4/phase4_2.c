#include <stdlib.h>
#include <stdio.h>
#include <string.h>

enum {
    STATE_VAL_FROM,
    STATE_BREAK,
    STATE_VAL_TO,
    STATE_NEXT
};

int function17(char* student_input, int length);
int function18();

int sequence[10] = {{ array_of_length(varname = 'K', length = 10, min = 1, max = 100) }};

int phase_4(char *server_input, char* student_input) {
    int index, input, state, from, to, length;
    char c;

    if (student_input == NULL) {
        return 1;
    }

    length = strlen(student_input);

    if (function17(student_input, length) == 1) {
        return 1;
    }

    state = STATE_VAL_FROM;
    index = 0;

    while(student_input[index] != ':' && student_input[index] != '\0') {
        c = student_input[index++];
        switch(state) {
            case STATE_VAL_FROM:
                if (c >= 48 && c <= 57) {
                    from = c - 48;
                    state = STATE_NEXT;
                } else {
                    return 1;
                }
                break;
            case STATE_NEXT:
                if (c == 45) {
                    state = STATE_VAL_TO;
                } else {
                    return 1;
                }
                break;
            case STATE_VAL_TO:
                if (c >= 48 && c <= 57) {
                    to = c - 48;
                    state = STATE_BREAK;
                } else {
                    return 1;
                }
                break;
            case STATE_BREAK:
                if (c == 32) {
                    int temp = sequence[to];
                    sequence[to] = sequence[from];
                    sequence[from] = temp;
                    state = STATE_VAL_FROM;
                } else {
                    return 1;
                }
                break;
            default:
                return 1;
                break;
        }
    }

    if (length - index < 2) {
        return 1;
    }

    input = atoi(student_input + index + 1);
    if (input == 0) {
        return 1;
    }

    if (function18() == 1) {
        return 1;
    }

    index = atoi(server_input);
    if (sequence[index] == input) {
        return 0;
    } else {
        return 1;
    }
}

int function17(char* student_input, int length) {
    int i, valid;
    char c;
    
    for (i = 0; i < length; i++) {
        c = student_input[i];
        valid = 0;

        if (c >= 48 && c <= 57) valid = 1;
        else if (c == 32) valid = 1;
        else if (c == 45) valid = 1;
        else if (c == 58) valid = 1;

        if (valid == 0) {
            return 1;
        }
    }

    return 0;
}

int function18() {
    int i, prev = 0;
    for (i = 0; i < 10; i++) {
        if (prev > sequence[i]) {
            return 1;
        }
        prev = sequence[i];
    }
    printf("\n");

    return 0;
}