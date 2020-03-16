#ifndef HEAP_H
#define HEAP_H

#define MAX_HEAP_SIZE 256

typedef struct heap_node HeapNode;
struct heap_node {
	int       frequency;  /*<< the character frequency              */
	char      c;          /*<< the character                        */
	HeapNode *left;       /*<< the left node (in the Huffman tree)  */
	HeapNode *right;      /*<< the right node (in the Huffman tree) */
};

typedef struct heap {
	int n;                      /*<< the number of elements in the heap      */
	HeapNode a[MAX_HEAP_SIZE];  /*<< the array for storing the heap elements */
} Heap;

/**
 * Initialise the heap to all zero bits.
 *
 * @param[in/out]  h     the heap
 */
void heap_initialize(Heap *H);

/**
 * Remove the minimum node in the heap, and then reorganises the heap so that
 * the new minimum node is in the correct position.
 *
 * @param[in/out]  h     the heap
 * @param[out]     node  the removed minimum node
 */
void heap_remove(Heap *H, HeapNode *node);

/**
 * Inserts a new node into the heap.
 *
 * @param[in/out]  h     the heap
 * @param[in]      node  the node to insert into the heap
 */
void heap_insert(Heap *H, HeapNode *node);

#endif
