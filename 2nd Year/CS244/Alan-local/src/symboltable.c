/**
 * @file    symboltable.c
 * @brief   A symbol table for ALAN-2019.
 * @author  W. H. K. Bester (whkbester@cs.sun.ac.za)
 * @date    2019-08-03
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "boolean.h"
#include "error.h"
#include "hashtable.h"
#include "symboltable.h"
#include "token.h"

/* --- global static variables ---------------------------------------------- */

static HashTab *table, *saved_table;
/* TODO: Nothing here, but note that the next variable keeps a running count of
 * the number of variables in the current symbol table.  It will be necessary
 * during code generation, to compute the size of the local variable array of a
 * method frame in the Java virtual machine.
 */
static unsigned int curr_offset;

/* --- function prototypes -------------------------------------------------- */

static void valstr(void *key, void *p, char *str);
static void freeprop(void *p);
static unsigned int shift_hash(void *key, unsigned int size);
static int key_strcmp(void *val1, void *val2);
#if 1
static void freekey(void *v);
#endif
static void freeval(void *v);


/* --- symbol table interface ----------------------------------------------- */

void init_symbol_table(void)
{
	saved_table = NULL;
	if ((table = ht_init(0.75f, shift_hash, key_strcmp)) == NULL) {
		eprintf("Symbol table could not be initialised");
	}
	curr_offset = 1;
}

Boolean open_subroutine(char *id, IDprop *prop)
{
	Boolean success;

	success = insert_name(id, prop);
	if (!success) {
		return success;
	}

	saved_table = table;
	if ((table = ht_init(0.75f, shift_hash, key_strcmp)) == NULL) {
		return FALSE;
	}
	curr_offset = 0;

	return TRUE;


	/* TODO:
	 * - Insert the subroutine name into the global symbol table; return TRUE or
	 *   FALSE, depending on whether or not the insertion succeeded.
	 * - Save the global symbol table to saved_table, initialise a new hash
	 *   table for the subroutine, and reset the current offset.
	 */
}

void close_subroutine(void)
{
	release_symbol_table();
	table = saved_table;
	/* TODO: Release the subroutine table, and reactivate the global table. */
}

Boolean insert_name(char *id, IDprop *prop)
{
	Boolean found;

	found = find_name(id, &prop);

	if (found == FALSE) {
		prop->offset = curr_offset;
		if (ht_insert(table, id, (void *) prop) != EXIT_SUCCESS) {
			free(id);
			freeprop(prop);
		} else if (prop->nparams == 0) {
			curr_offset++;
		}
	}

	return !found;

	/* TODO: Insert the properties of the identifier into the hash table, and
	 * remember to increment the current offset pointer if the identifier is a
	 * variable.
	 *
	 * VERY IMPORTANT: Remember to read the documentation of this function in
	 * the header file.
	 */
}

Boolean find_name(char *id, IDprop **prop)
{
	Boolean found;

	/* TODO: Nothing ... unless you want to. */
	found = ht_search(table, id, (void **) prop);
	if (!found && saved_table != NULL) {
		found = ht_search(saved_table, id, (void **) prop);
		if (found && !IS_CALLABLE_TYPE((*prop)->type)) {
			found = FALSE;
		}
	}

	return found;
}

int get_variables_width(void)
{
	return curr_offset;
}

void release_symbol_table(void)
{
	ht_free(table, freekey, freeval);
	/* TODO: Free the underlying structures of the symbol table. */
}

void print_symbol_table(void)
{
	ht_print(table, valstr);
}

/* --- utility functions ---------------------------------------------------- */

static void valstr(void *key, void *p, char *str)
{
	char *keystr = (char *) key;
	IDprop *idpp = (IDprop *) p;

	/* TODO: Nothing, but this should give you an idea of how to look at the
	 * contents of the symbol table.
	 */
	sprintf(str, "%s.%d", keystr, idpp->offset);
	sprintf(str, "%s@%d[%s]", keystr, idpp->offset,
			get_valtype_string(idpp->type));
}

/* TODO: Here you should add your own utility functions, in particular, for
 * deallocation, hashing, and key comparison.  For hashing, you MUST NOT use the
 * simple strategy of summing the integer values of characters.  I suggest you
 * use some kind of cyclic bit shift hash.
 */

static unsigned int shift_hash(void *key, unsigned int size)
{
	char *keystr = (char *) key;
	int hash, length;


	int a, i;
	hash = 0;
	a = 41;
	length = strlen(keystr);
	for (i = 0; i < length; i++) {
		hash = hash + keystr[i] * a;
	}

	/*length = strlen(keystr);
		for (int i = 0; i < length; i++) {
			hash = (hash << 5) | (hash >> 27); // 5-bit cyclic shift of the
			hash += (int) keystr[i]; // add in next character
		}*/

	return (hash % size);
}

static int key_strcmp(void *v1, void *v2)
{
    return strcmp((char *) v1, (char *) v2);
}

static void freeprop(void *p)
{
	ValType *v;

	for (v = ((IDprop *) p)->params; v; v++) {
		free(v);
	}
	free(p);
}

static void freekey(void *v) {
	free(v);
}

static void freeval(void *v) {
	IDprop *p;
	p = (IDprop *) v;
	if (p->nparams != 0) {
		free(p->params);
	}
	free(p);
}
