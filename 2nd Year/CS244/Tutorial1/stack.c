#include "polish.h"

#define STACKSIZE 100

static double stack[STACKSIZE];
static double *sp = stack;

int main(int argc, char *argv) {
    	

}

void push(double f) {
  	*sp++ = f;
}

double pop(void) {
    double val = *sp--;
   	return val;
}

