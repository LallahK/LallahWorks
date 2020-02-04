#ifndef TUT5_H
#define TUT5_H

typedef struct item {
	int  number;
	char name[32];
} item;

typedef struct node {
	char name[32];
	struct node *left;
	struct node *right;
} node;


/**
 * Performs a binary sort on the items pointed to by the parameter list.
 *
 * @param[in/out]   list  pointer to an array of items
 * @param[in]       n     total number of elements in the array
 */
extern void binary_sort(item *list, int n);

/**
 * Locates a node whose name field matches the name specified as a parameter.
 * If the node exists, it is deleted and the tree is reorganised if necessary).
 * The memory occupied by the node is also released using the free functions.
 *
 * @param[in/out]   root  reference parameter to the root node of a binary tree
 * @param[in]       name  name field of the node that must be deleted
 */
extern void delete_node(node **root, char *name);

/**
 * Converts the specified integer value to a NULL terminated character string
 * that contains the binary representation of the value specified by n.
 *
 * @param[in]       n  any legal signed integer value
 * @param[out]      s  pointer to a character string which must be able to hold
 *                     33 characters
 */
extern void bin_to_string(int n, char *s);

#endif
