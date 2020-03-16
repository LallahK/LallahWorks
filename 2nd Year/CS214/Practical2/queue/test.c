#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "queue.h"

#define NNUMS 10

int main()
{
	int i, *n, *nums, seed;
	QueueNode *queue;

	/* empty queue */
	queue = NULL;

	/* seed random-number generator */
	seed = time(NULL);
	srand(seed);

	/* build array of integers */
	nums = malloc(sizeof(int) * NNUMS);
	for (i = 0; i < NNUMS; i++) {
		nums[i] = rand();
	}

	/* enqueue */
	for (i = 0; i < NNUMS; i++) {
		n = malloc(sizeof(int));
		*n = nums[i];
		enqueue(&queue, n);
	}

	/* print original array and dequeue */
	for (i = 0; i < NNUMS; i++) {
		printf("[%d]: %d ", i, nums[i]);
		n = (int *) dequeue(&queue);
		if (n != NULL) {
			printf("--> %d (%s)\n", *n,
					(*n == nums[i]
					 	? "\033[0;32mcorrect\033[0m"
					 	: "\033[0;31mincorrect\033[0m"));
			free(n);
		} else {
			printf(" \033[0;33munexpected NULL\033[0m\n");
		}
	}

	/* deallocate array */
	free(nums);

	return EXIT_SUCCESS;
}
