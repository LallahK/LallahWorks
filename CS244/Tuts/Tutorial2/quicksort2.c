#include <stdio.h>
#include <stdlib.h>

void swap(void*, void*);
static void sort(void*, int, int (*comp)(void*, void*));
static int partition(void*, int, int (*comp)(void*, void*));
void printarr(int*, int);

int main(int argc, char *argv[])
{
	int arrlength = 8;
	int arr[] = {5, 1, 4, 7, 3, 9, 8, 3};
	
	printarr(arr, arrlength);
	
	return EXIT_SUCCESS;	
}

static void sort(void *v, int len, int (*comp)(void*, void*))
{
	int p;
	if (len > 1)
	{
		p = partition(v, len, comp);
		/*sort(v, p);
		sort(v + p + 1, len - p - 1);*/
	}
}

static int partition(void *v, int len, int (*comp)(void*, void*))
{
	void *lp = malloc(sizeof(int)), *hp = malloc(sizeof(int));
	int c = *v;
	int index;

	lp = v;
	hp = (v + len);
	while (1)
	{
		while () if (lp - len  - v == 0) break;
		while () if (hp - v == 0) break;
		if (lp - hp > 0) break;
		swap(lp, hp);
	}
	swap(hp, v);
	index = hp - v;
	return index;
}

void swap(void *v, int a, intb)
{
	void *temp = v[a];
 	v[a] = v[b];
	v[b] = *temp;
}

void printarr(void *v, int len)
{
	int i;
	for (i = 0; i < len; i++)
	{
		printf("%d ", v[i]);
	} 
	printf("\n");
}

