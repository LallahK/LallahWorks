#include <stdlib.h>
#include <stdio.h>

char* function3();
int function6(char *first, char* second);

char string[27] = "{{ short_strings_gen(varname = 'K', count = 4) }}";

int phase_3(char *server_input, char *student_input) { 
    char *mixed;

    if (student_input == NULL) {
        return 1;
    }

    mixed = function3();
    if (function6(student_input, mixed) == 1) {
        return 1;
    }

    return 0; 
}

char* function3() {
    int i, j, k;
    char *mixed, ***cuboid;
    
    cuboid = malloc(3 * sizeof(char**));
    for (i = 0; i < 3; i++) {
        cuboid[i] = malloc(3 * sizeof(char*));
        for (j = 0; j < 3; j++) {
            cuboid[i][j] = malloc(3 * sizeof(char));
        }
    }

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            for (k = 0; k < 3; k++) {
                cuboid[i][j][k] =  string[i * 9 + 3 * j + k];
            }
        }
    }

    mixed = malloc(28 * sizeof(char));

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            for (k = 0; k < 3; k++) {
                mixed[i * 9 + 3 * j + k] = cuboid[i][k][j];
            }
        }
    }
    mixed[27] = '\0';

    return mixed;
}

int function6(char *first, char* second) {
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