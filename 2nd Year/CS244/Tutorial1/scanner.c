#include "polish.h"
#include <ctype.h>

#define GETCHSIZE 50
static char getchStack[GETCHSIZE];
static char *gp;

static int getch(void);
static void ungetch(char c);
int getop(char s[]);

int main(int argc, char *argv) {
	char s[5];
    printf("number: %d", getop(s)); 
}

int getop(char *s) {
 	int c;
	int i = 0;
    
    while ((c = s[0] = getchar()) == ' ' || c == '\t');
    s[1] = '\0';
    
    printf("%d\n", 34);
	if (!(isdigit(c)))
		return c;
    else while (isdigit(c == getchar()))
        s[++i] = c;
    s[i] = '\0';
    return 1;
}
