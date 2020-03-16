/**
  * @file loader.h
  * @description A definition of the structures and functions to store and
  *              represent the different transactions 
  */

#ifndef _LOADER_H
#define _LOADER_H

#define DP_T 0
#define WD_T 1 
#define TR_T 2 
#define BL_T 3 

#define MAX_ACCS 100

/**
 */
struct transaction_t {
  /** The type of transaction */
  int type;
  /** The source account */
  int src;
  /** The destination account, if applicable */
  int dest;
  /** The amount, if applicable */
  double amount;
  /** A pointer to the next instruction */
  struct transaction_t *next;
};

/*
 *	This stores one pointer to a transaction list. 
 *	Update it to store an array of pointers, one to each transaction list 
 */
struct transaction_t *transactions[MAX_ACCS];

/*
 * Creates a deposit transaction and adds 
 * it to a linked list of transactions 
 */
void load_deposit(int type, int dest, double amount); 

/*
 * Creates a withdrawal transaction and adds 
 * it to a linked list of transactions 
 */
void load_withdrawal(int type, int src, double amount); 

/*
 * Creates a transfer transaction and adds 
 * it to a linked list of transactions 
 */
void load_transfer(int type, int src, int dest, double amount); 

/*
 * Creates a balance transaction and adds 
 * it to a linked list of transactions 
 */
void load_balance(int type, int src); 

/*
 * Frees all the transactions on termination.
 */
void dealloc_transactions();

void dealloc_transaction(struct transaction_t *transaction);

#endif


