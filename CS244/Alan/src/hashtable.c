/**
 * @file    hashtable.c
 * @brief   A generic hash table.
 * @author  W. H. K. Bester (whkbester@cs.sun.ac.za)
 * @date    2018-07-10
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"
#include "symboltable.h"

#define INITIAL_DELTA_INDEX  4
#define PRINT_BUFFER_SIZE 1024

/** an entry in the hash table */
typedef struct htentry HTentry;
struct htentry {
	void    *key;       /*<< the key                      */
	void    *value;     /*<< the value                    */
	HTentry *next_ptr;  /*<< the next entry in the bucket */
};

#if 0
typedef struct {
    char *id;
    int num;
} Name;
#endif


/** a hash table container */
struct hashtab {
	/** a pointer to the underlying table                              */
	HTentry **table;
	/** the current size of the underlying table                       */
	unsigned int size;
	/** the current number of entries                                  */
	unsigned int num_entries;
	/** the maximum load factor before the underlying table is resized */
	float max_loadfactor;
	/** the index into the delta array                                 */
	unsigned short idx;
	/** a pointer to the hash function                                 */
	unsigned int (*hash)(void *, unsigned int);
	/** a pointer to the comparison function                           */
	int (*cmp)(void *, void *);
};

/* --- function prototypes -------------------------------------------------- */

/* TODO: For the following functions, refer to the TODO note at the end of the
 * file.
 */

#if 0
static void val1str(void*, void*, char*);
#endif
static double pow(double, double);
static int getsize(HashTab *ht);
static HTentry **talloc(int tsize);
static void rehash(HashTab *ht);

/* TODO: For this implementation, we want to ensure we *always* have a hash
 * table that is of prime size.  To that end, the next array stores the
 * difference between a power of two and the largest prime less than that
 * particular power of two.  When you rehash, compute the new prime size using
 * the following array.
 */

/** the array of differences between a power-of-two and the largest prime less
 * than that power-of-two.                                                    */
unsigned short delta[] = { 0, 0, 1, 1, 3, 1, 3, 1, 5, 3, 3, 9, 3, 1, 3, 19, 15,
1, 5, 1, 3, 9, 3, 15, 3, 39, 5, 39, 57, 3, 35, 1 };

#define MAX_IDX (sizeof(delta) / sizeof(short))

/* --- hash table interface ------------------------------------------------- */

HashTab *ht_init(float loadfactor,
				 unsigned int (*hash)(void *, unsigned int),
				 int (*cmp)(void *, void *))
{
	HashTab *ht;
	unsigned int i;

	ht = (struct hashtab*)malloc(sizeof(struct hashtab));
	ht->table = talloc(INITIAL_DELTA_INDEX);

	if (ht == NULL) {
		free(ht);
		return NULL;
	} else if (!(ht == NULL) && ht->table == NULL) {
		free(ht);
		return NULL;
	}

	ht->size = INITIAL_DELTA_INDEX;
	ht->num_entries = 0;
	ht->max_loadfactor = loadfactor;
	ht->idx = 2;
	ht->hash = hash;
	ht->cmp = cmp;

	for (i = 0; i < ht->size; i++) {
		ht->table[i] = NULL;
	}
	/*for (i = 0; i < INITIAL_DELTA_INDEX; i++) {
		ht->table[i] = NULL;
		ht->table[i] = (struct htentry*)malloc(sizeof(struct htentry));
		ht->table[i]->next_ptr = NULL;
	}*/

	/* TODO:
	 * - Initalise a hash table structure by calling an allocation function
	 *   twice:
	 *   (1) once to allocate space for a HashTab variable, and
	 *   (2) once to allocate space for the table field of this new HashTab
	 *       variable.
	 * - If any allocation fails, free anything that has already been allocated
	 *   successfully, and return NULL.
	 * - Also set up the other fields of the newly-allocated HashTab structure
	 *   appropriately.
	 */
	return ht;
}

int ht_insert(HashTab *ht, void *key, void *value)
{
	int k = 0;
	HTentry *p;

	k = ht->hash(key, ht->size);
	if (ht->table[k] == NULL) {
		ht->table[k] = (struct htentry*)malloc(sizeof(struct htentry));
		p = ht->table[k];
	} else {
		p = ht->table[k];
		while (!(p->next_ptr == NULL)) {
			p = p->next_ptr;
		}
		p->next_ptr = (struct htentry*)malloc(sizeof(struct htentry));
		p = p->next_ptr;
	}

	if (p == NULL) {
		return HASH_TABLE_NO_SPACE_FOR_NODE;
	}

	p->key = (void *)malloc(sizeof(char) * strlen((char *) key));
	strcpy(p->key, key);
	p->value = value;
	p->next_ptr = NULL;

	ht->num_entries++;
	if (ht->num_entries/(1.0 * ht->size) >= ht->max_loadfactor) {
			rehash(ht);
	}
	/* TODO: Insert a new key--value pair, rehashing if necessary.  The best way
	 * to go about rehashing is to put the necessary elements into a static
	 * function called rehash.  Remember to free space (the "old" table) you do
	 * not use any longer.  Also, if something goes wrong, use the #define'd
	 * constants in hashtable.h for return values; remember, unless it runs out
	 * of memory, no operation on a hash table may terminate the program.
	 */

	return EXIT_SUCCESS;
}

Boolean ht_search(HashTab *ht, void *key, void **value)
{
	int k;
	HTentry *p;

	/* TODO: Nothing!  This function is complete, and it should explain by
	 * example how the hash table looks and must be accessed.
	 */

	k = ht->hash(key, ht->size);
	for (p = ht->table[k]; p != NULL; p = p->next_ptr) {
		if (p != NULL) {
			if (ht->cmp(key, p->key) == 0) {
				*value = p->value;
				break;
			}
		}
	}

	return (p ? TRUE : FALSE);
}

Boolean ht_free(HashTab *ht, void (*freekey)(void *k), void (*freeval)(void *v))
{
	unsigned int i;
	HTentry *p, *q;

	/* free the nodes in the buckets */
	/* TODO */
	for (i = 0; i < ht->size; i++) {
		p = ht->table[i];
		while (p != NULL) {
			q = p;
			p = p->next_ptr;
			freekey(q->key);
			freeval(q->value);
			free(q);
		}
		ht->table[i] = NULL;
	}

	/* free the table and container */
	/* TODO */
	free(ht->table);
	free(ht);

	return EXIT_SUCCESS;
}

void ht_print(HashTab *ht, void (*keyval2str)(void *k, void *v, char *b))
{
	unsigned int i;
	HTentry *p;
	char buffer[PRINT_BUFFER_SIZE];

	/* TODO: This function is complete and useful for testing, but you have to
	 * write your own keyval2str if you want to use it.
	 */
	for (i = 0; i < ht->size; i++) {
		printf("bucket[%2i]\n", i);
		for (p = ht->table[i]; p != NULL; p = p->next_ptr) {
			keyval2str(p->key, p->value, buffer);
			printf(" --> %s\n", buffer);
		}
		printf(" --> NULL\n");
	}
}

/* --- utility functions ---------------------------------------------------- */

/* TODO: I suggest completing the following helper functions for use in the
 * global functions ("exported" as part of this unit's public API) given above.
 * You may, however, elect not to use them, and then go about it in your own way
 * entirely.  The ball is in your court, so to speak, but remember: I have
 * suggested using these functions for a reason -- they should make your life
 * easier.
 */

static int getsize(HashTab *ht)
{
	int new_idx = 0, i;

	ht->idx++;
	i = ht->idx;
	new_idx = pow(2, i) - delta[i];

	return new_idx;
	/* TODO: Compute the next prime size of the hash table. */
}

static HTentry **talloc(int tsize)
{
	HTentry **t = (struct htentry**)malloc(sizeof(struct htentry *) * tsize);
	return t;
	/* TODO: Allocate space for one hash table entry.  It could just be a
	 * one-liner, and really just saves on typing
	 */
}

static void rehash(HashTab *ht)
{
	HTentry **new_tb, *p, *q;
	unsigned int new_size = getsize(ht), i, k;
	new_tb = talloc(new_size);

	for (i = 0; i < new_size; i++) {
		new_tb[i] = NULL;
	}

	for (i = 0; i < ht->size; i++) {
		for (p = ht->table[i] ; p != NULL; p = p->next_ptr) {
			k = ht->hash(p->key, new_size);
			if (new_tb[k] == NULL) {
				new_tb[k] = (struct htentry*)malloc(sizeof(struct htentry));
				q = new_tb[k];
			} else {
				q = new_tb[k];
				printf("%s\n", (char*) q->value);
				while (!(q->next_ptr == NULL)) {
					q = q->next_ptr;
				}
				q->next_ptr = (struct htentry*)malloc(sizeof(struct htentry));
				q = q->next_ptr;
			}
			q->key = p->key;
			q->value = p->value;
			q->next_ptr = NULL;
		}
	}

	free(ht->table);
	ht->table = new_tb;
	ht->size = new_size;

	/* TODO: Rehash the hash table by
	 * (1) allocating a new table that uses as size the next prime in the
	 *     "almost-double" array,
	 * (2) moving the entries in the existing tables to appropriate positions in
	 *     the new table, and
	 * (3) freeing the old table.
	 */
}

static double pow(double a, double b)
{
	double r = 1;
	int i;

	for (i = 0; i < b; i++) {
		r = r*a;
	}
	return r;

}

#if 0
static void val1str(void *key, void *value, char *buffer)
{
    sprintf(buffer, "%s:[%d]", (char *) key, ((Name *) value)->num);
}
#endif
