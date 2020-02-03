#include <stdio.h>
#include <stdlib.h>

#define MAXLINE 250
int LINES = 2;

int getlin(char *s){
    char c;
    char *sp = s;
    while ((c = getchar()) != '\n') {
      *sp++ = c;
    }
    return sp - s;
}

void copy(char *to, char *from) {
    while ((*to++ = *from++) != '\0') {}
}

int main(int argc, char *argv[]) {
    char s[250], m[250];
    int max = 0, len;
    while (LINES-- > 0)
        if ((len = getlin(s)) > max) {
            max = len;
            copy(m, s);
        }
    printf("%s", m);
}

