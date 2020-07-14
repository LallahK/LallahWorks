#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char* function11(char* input);
void function12(char* input, int len);
int function13(char* server_input, char* user_input, char* string, int length);

char* sentance = {{ pangram_generate(varname = 'K') }};

int phase_4(char *server_input, char *student_input) {
    int length;
    char* spaceless_string;

    if (student_input == NULL) {
        return 1;
    }

    spaceless_string = function11(sentance);
    length = strlen(spaceless_string);
    function12(spaceless_string, length);
    
    return function13(server_input, student_input, spaceless_string, length);
}

char* function11(char* input) {
    int spaceless = 1, index = 0, result_index;
    char c, *result;

    while ((c = input[index++]) != '\0') {
        if (c != 32) spaceless++;
    }

    result = malloc((spaceless) * sizeof(char));

    index = 0;
    result_index = 0;
    while((c = input[index++]) != '\0') {
        if (c != 32) {
            if (c < 97) result[result_index++] = c + 32;
            else result[result_index++] = c;
        }
    }
    result[result_index] = '\0';

    return result;
}

void function12(char* input, int len) {
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
}

int function13(char* server_input, char* user_input, char* string, int length) {
    int l = 0, mid, r = length - 1, index = 0;
    
    while (1) {
        if (r < l) return 1;
        mid = l + (r - l) / 2;

        if (user_input[index] == 'l') {
            r = mid - 1;
        } else if (user_input[index] == 'r') {
            l = mid + 1;
        } else if (user_input[index] == 'c') {
            break;
        } else {
            return 1;
        }
        index++;
    }

    if (server_input[0] == string[mid]) {
        return 0;
    } else {
        return 1;
    }
}

int main(int argc, char *argv[]) {
    printf("%d\n", phase_3(argv[1], argv[2]));
}