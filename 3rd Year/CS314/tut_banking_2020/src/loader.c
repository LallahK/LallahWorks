/**
 * @file loader.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _OPENMP
#include <omp.h>
#endif

#include "syntax.h"
#include "loader.h"

/*
 * Creates a deposit transaction and adds 
 * it to a linked list of transactions 
 */
void load_deposit(int type, int dest, double amount){
	struct transaction_t *trans = malloc(sizeof(struct transaction_t)),
	*trans_list;
	int tc = omp_get_thread_num();

	trans->type = type;
	trans->src = 0;
	trans->dest = dest;
	trans->amount = amount;
	trans->next = NULL;
	if (transactions[tc] == NULL) {
		transactions[tc] = trans;
	} else { 
		trans_list = transactions[tc];
		while (trans_list->next != NULL) {
		  trans_list = trans_list->next;
		}
		trans_list->next = trans;
	}

#ifdef DEBUG
	printf("Loaded: %d  deposit %d %f \n", tc, dest, amount);
#endif
}

/*
 * Creates a withdrawal transaction and adds 
 * it to a linked list of transactions 
 */
void load_withdrawal(int type, int src, double amount){
	struct transaction_t *trans = malloc(sizeof(struct transaction_t)),
	*trans_list;
	int tc = omp_get_thread_num();

	trans->type = type;
	trans->src = src;
	trans->dest = 0;
	trans->amount = amount;
	trans->next = NULL;
	if (transactions[tc] == NULL) {
		transactions[tc] = trans;
	} else { 
		trans_list = transactions[tc];
		while (trans_list->next != NULL) {
		  trans_list = trans_list->next;
		}
		trans_list->next = trans;
	}

#ifdef DEBUG
	printf("Loaded: %d withdraw %d %f \n", tc, src, amount);
#endif
} 

/*
 * Creates a transfer transaction and adds 
 * it to a linked list of transactions 
 */
void load_transfer(int type, int src, int dest, double amount){
	struct transaction_t *trans = malloc(sizeof(struct transaction_t)),
	*trans_list;
	int tc = omp_get_thread_num();

	trans->type = type;
	trans->src = src;
	trans->dest = dest;
	trans->amount = amount;
	trans->next = NULL;
	if (transactions[tc] == NULL) {
		transactions[tc] = trans;
	} else { 
		trans_list = transactions[tc];
		while (trans_list->next != NULL) {
		  trans_list = trans_list->next;
		}
		trans_list->next = trans;
	}
#ifdef DEBUG
	printf("Loaded: %d transfer %d %d %f \n", tc, src, dest, amount);
#endif
}

/*
 * Creates a balance transaction and adds 
 * it to a linked list of transactions 
 */
void load_balance(int type, int src){
	struct transaction_t *trans = malloc(sizeof(struct transaction_t)),
	*trans_list;
	int tc = omp_get_thread_num();

	trans->type = type;
	trans->src = src;
	trans->dest = 0;
	trans->amount = 0;
	trans->next = NULL;
	if (transactions[tc] == NULL) {
		transactions[tc] = trans;
	} else { 
		trans_list = transactions[tc];
		while (trans_list->next != NULL) {
		  trans_list = trans_list->next;
		}
		trans_list->next = trans;
	}
#ifdef DEBUG
	printf("Loaded: %d balance %d \n", tc, src);
#endif
} 

/*
 * Frees all the transactions after termination.
 */

void dealloc_transactions(){
	int i;
	for (i = 0; i < 4; i++) {
		dealloc_transaction(transactions[0]);
	}
}

void dealloc_transaction(struct transaction_t *transaction){
	struct transaction_t *old, *new = transaction;
	if (new != NULL) {
		while (new != NULL) {
			old = new;
			new = new->next;
			free(old);
		}
	}

}
