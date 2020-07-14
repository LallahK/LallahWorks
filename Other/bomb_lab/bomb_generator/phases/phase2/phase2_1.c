#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int function4(char* input, int len);
int function5(char* input, int len);

int phase_2(char* server_input, char* student_input) {
    int user_val, i, input_len, val, new_val;
    char* second_input, *first_input;

    if (student_input == NULL) {
        return 1;
    }

    user_val = atoi(student_input);
    if (user_val == 0) {
        return 1;
    }

    val = atoi(server_input);
    input_len = strlen(server_input);

    first_input = malloc(input_len * sizeof(char));
    second_input = malloc(input_len * sizeof(char));

    for (i = 0; i <= 7; i++) {
        sprintf(first_input, "%d", val);
        strcpy(second_input, first_input);

        new_val = function5(first_input, input_len) - function4(second_input, input_len);
        
        if (new_val == val) {
            break;
        }
        val = new_val;
    }

    if (user_val == val) {
        return 0;
    } else {
        return 1;
    }
}

int function4(char* input, int len) {
    int index, key, j;

    for (index = 0; index < len; index++) {
        key = input[index];

        j = index - 1;
        while (j >= 0 && input[j] > key) { 
            input[j + 1] = input[j]; 
            j = j - 1; 
        } 
        input[j + 1] = key; 
    }

    return atoi(input);
}

int function5(char* input, int len) {
    int index, key, j;

    for (index = 0; index < len; index++) {
        key = input[index];

        j = index - 1;
        while (j >= 0 && input[j] < key) { 
            input[j + 1] = input[j]; 
            j = j - 1; 
        } 
        input[j + 1] = key; 
    }

    return atoi(input);
}

int main(int argc, char *argv[]) {
    printf("%d\n", phase_2(argv[1], argv[2]));
}