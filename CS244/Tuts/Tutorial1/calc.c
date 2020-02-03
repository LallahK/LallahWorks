#include "polish.h"

int main (int argc, char *argv[]) {
	char c;
    char s[8];
    while ((c = getop(s)) != '\n') {
    	switch (c) {
		case 1 : 
        	push(atof(s));
			break;
		}
    }  
}


