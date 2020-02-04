#include <stdio.h>
#include <stdlib.h>
#include "compare.h"
#include "copy.h"

void q_sort(void*, int, int (*cmp)(void *val1, void *val2));
int _sort(void*, int, int (*cmp)(void *val1, void *val2));
void swap(void *v[], int, int);
void print_val(void*);
int cmp(void*, void*);

int main(int argc, char *argv[])
{
	int i;
	int a[] = {1, 2, 6, 1, 10, 7, 0, -1};
	for (i = 0; i < 8; i++) {
	    print_val(&a[i]);
	}
    printf("\n");
	
	q_sort(a, 8, &cmp);

	for (i = 0; i < 8; i++) {
		print_val(&a[i]);
	}
	printf("\n");

	return 	EXIT_SUCCESS;	
}

void q_sort(void *v, int len, int (*comp)(void *, void *))
{
	int p;
	p = _sort(v, len, comp);
	
	if (p != 0) {
		q_sort(v, p, comp);
	}
	if (len - p != 0) {
		q_sort(v + p + 1, len - p - 1, comp); 
	}
}

int _sort(void *v, int len, int (*cmp)(void *val1, void *val2))
{
	void *low, *high, *piv;

	if (len < 2) {
		return 0;
	}

	piv = v;
	for (low = v[len - 1], high = v[len - 1]; ; low--) {
		if (low == v) {
			break;
		}
		if (cmp(low, piv)) {
			swap(&v, low - v, high - v);

			high--;
		}
	}

	int i;

	swap(&v, high - v, 0);

	return high - v;
}

void swap(void *v[], int a, int b)
{
	void *temp;

	printf("start %d %d\n", a, b);
	temp = v[a];
	v[a] = v[b];
	v[b] = temp;
	printf("end\n");
}

void print_val(void *val)
{
#if 1
	printf("%d ", *((int *) val));
#endif
}

int cmp(void *val1, void *val2)
{
#if 1
	int a = *((int *) val1), b = *((int *) val2);
	return (a >= b);
#endif
}
