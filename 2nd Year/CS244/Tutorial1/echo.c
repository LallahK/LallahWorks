#include <stdio.h>
#include <stdlib.h>

#define MAXLENGTH 100

int main(int argc, char *argv[]) {
    FILE *fp;
    char line[100];
	char *cp;
    char *lp;
    fp = fopen(argv[1], "r");
    if (fp == NULL) return 0;
    
    while (1) {
        cp = line;
        lp = line;
        while ((*cp++ = fgetc(fp)) != '\n') {}
        
        while (lp != cp) {
            if (*lp == ' ') {
				while (*lp == ' '){ lp++ }
            }
        }
    }
    

    /*int c;
    while ((c = fgetc(fp)) != EOF) {
        putchar(c);
    }*/
    
    fclose(fp);          
    //printf("\n%d\t%d\t%d", chars, words, lines);
    return EXIT_SUCCESS;

}
