#include <stdio.h>
#include <stdlib.h>

void quicksort(int*);
void swap(int*, int*);
static void sort(int*, int);
static int partition(int*, int);
void printarr(int*, int);

int main(int argc, char *argv[])
{
	int arrlength = 8;
	int arr[] = {5, 1, 4, 7, 3, 9, 8, 3};
	sort(arr, arrlength);
	printarr(arr, arrlength);
	
	return EXIT_SUCCESS;	
}

void quicksort(int *v)
{
	
}

static void sort(int *v, int len)
{
	int p;
	if (len > 1)
	{
		p = partition(v, len);
		sort(v, p);
		sort(v + p + 1, len - p - 1);
	}
}

static int partition(int *v, int len)
{
	int *lp = malloc(sizeof(int)), *hp = malloc(sizeof(int));
	int comp = *v;
	int index;

	lp = v;
	hp = (v + len);
	while (1)
	{
		while (*(++lp) < comp) if (lp - len  - v == 0) break;
		while (*(--hp) > comp) if (hp - v == 0) break;
		if (lp - hp > 0) break;
		swap(lp, hp);
	}
	swap(hp, v);
	index = hp - v;
	return index;
}

void swap(int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

void printarr(int *v, int len)
{
	int i;
	for (i = 0; i < len; i++)
	{
		printf("%d ", v[i]);
	} 
	printf("\n");
}

