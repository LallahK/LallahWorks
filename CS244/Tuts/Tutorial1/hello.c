#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv) {
	int number = 12;
    int *ip[2];
    ip[0] = &number;

    printf("%p %p", ip[0], &number);
    return EXIT_SUCCESS;
}
