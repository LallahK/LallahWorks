#include <stdio.h>
#include <stdlib.h>
#include "bsearch.h"

#define MAX_ITEMS_A 1 
#define MAX_ITEMS_B 2
#define MAX_ITEMS_C 3
#define MAX_ITEMS_D 9

#define LIST_A_NAME "list_a"
#define LIST_B_NAME "list_b"
#define LIST_C_NAME "list_c"
#define LIST_D_NAME "list_d"

int binary_search_c(int n, int list[], int low, int high)
{
	int middle;

	if (low > high)
		return -1;
		
	middle = (low + high)/2;
		
	if (n == list[middle])
		return middle;
	if (n < list[middle]) {
		high = middle - 1;
	} else {
		low = middle + 1;
	}
		
	return binary_search_c(n, list, low, high);
}

void evaluate_binary_search(const char *s, int list[], int high, int n)
{
	int pos;
	
	pos = binary_search(n, list, 0, high);
	printf("binary_search(%d, %s, 0, %d) returned %d - ", n, s, high, pos);
	if (pos == binary_search_c(n, list, 0, high)) {
		printf("\033[0;32mcorrect\033[0m\n");
	} else {
		printf("\033[0;31mincorrect\033[0m, should return %d\n",
				binary_search_c(n, list, 0, high));
	}
}

int main(int argc, char *argv[])
{
	int list_a[MAX_ITEMS_A] = {4};
	int list_b[MAX_ITEMS_B] = {10, 11};
	int list_c[MAX_ITEMS_C] = {3, 5, 7};
	int list_d[MAX_ITEMS_D] = {5, 23, 78, 102, 103, 104, 256, 16384, 32768};

	evaluate_binary_search(LIST_A_NAME, list_a, MAX_ITEMS_A, list_a[0]);
	evaluate_binary_search(LIST_A_NAME, list_a, MAX_ITEMS_A, 10);
	evaluate_binary_search(LIST_B_NAME, list_b, MAX_ITEMS_B, list_b[0]);
	evaluate_binary_search(LIST_B_NAME, list_b, MAX_ITEMS_B, list_b[MAX_ITEMS_B-1]);
	evaluate_binary_search(LIST_B_NAME, list_b, MAX_ITEMS_B, 12);
	evaluate_binary_search(LIST_C_NAME, list_c, MAX_ITEMS_C, list_c[0]);
	evaluate_binary_search(LIST_C_NAME, list_c, MAX_ITEMS_C, list_c[1]);
	evaluate_binary_search(LIST_C_NAME, list_c, MAX_ITEMS_C, list_c[MAX_ITEMS_C-1]);
	evaluate_binary_search(LIST_C_NAME, list_c, MAX_ITEMS_C, 9);
	evaluate_binary_search(LIST_D_NAME, list_d, MAX_ITEMS_D, list_d[0]);
	evaluate_binary_search(LIST_D_NAME, list_d, MAX_ITEMS_D, list_d[MAX_ITEMS_D/2]);
	evaluate_binary_search(LIST_D_NAME, list_d, MAX_ITEMS_D, list_d[MAX_ITEMS_D-1]);
	evaluate_binary_search(LIST_D_NAME, list_d, MAX_ITEMS_D, 65536);

	return EXIT_SUCCESS;
}
