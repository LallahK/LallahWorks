/* vim settings: se ts=4 
 * File: bank.c 
 * 
 * An account, Account holders, Deposit and Withdrawal transactions 
 *				 
 * Run: . run.sh 
 * 
 * Input: 
 * 
 * Output: 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "loader.h"
#include "parser.h"
#ifdef _OPENMP 
#include <omp.h> 
#endif

static void get_args(int argc, char* argv[], int* thread_count_p, char*** fname);
static void usage(char* prog_name);
static void deposit(int acc_num, double amount);
static void withdrawal(int acc_num, double amount);
static void transfer(int acc1, int acc2, double amount);
static void acc_balance(int acc_num, double* balance);
static void init_locks(int count);

double* account_balances;
omp_lock_t** locks;
omp_lock_t* trans;

/*------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
	int i, success;
	int thread_count;
	char** fname;
	
	get_args(argc, argv, &thread_count, &fname);
	account_balances = (double*) calloc(thread_count,sizeof(double));	
 	if (account_balances == NULL) {
		fprintf(stderr,"Memory could not be allocated, exiting\n");
		exit(0);		
	}

	/* TODO: Replace the test-code:
		- Read in the transaction list for each thread from its 
			transaction file (use parse_transaction_file(fname)) 
		- Execute the transactions in parallel; each thread should 
			execute its own list of transactions  
 	*/

#ifdef DEBUG
	for (i = 0; i < thread_count; i++) printf("Account %d balance before execution %f\n", i, account_balances[i]);
#endif

	init_locks(thread_count);
	trans = malloc(sizeof(omp_lock_t *));
	omp_init_lock(trans);

#pragma omp parallel num_threads(thread_count)
{
	int tc = omp_get_thread_num();
	struct transaction_t *tran;
	double bal;

	success = parse_transaction_file(fname[tc]);
	if (success) {
		printf("File: %s loaded succesfully\n", fname[tc]);
	} else {
		printf("File: %s could not be loaded\n", fname[tc]);
	}
	#pragma omp barrier

	tran = transactions[tc];
	while (tran != NULL) {
		switch (tran->type) {
			case DP_T : deposit(tran->dest, tran->amount);
				break;
			case WD_T : withdrawal(tran->src, tran->amount);
				break;
			case TR_T : transfer(tran->src, tran->dest, tran->amount);
				break;
			case BL_T : acc_balance(tran->src, &bal);
				printf("User %d, Balan %7s,    Acc %d (Bal %7.2f), %23s -- Successful, Acc %d (Bal %7.2f)\n"
				, omp_get_thread_num(), "\0", tran->src, bal
				, "\0", tran->src, bal);

				break;
		}
		tran = tran->next;
	}
}
	printf("--- Transaction batch completed ---\n");
	for (i = 0; i < thread_count; i++) {
		printf("Account %d balance after the completion of ", i);
		printf("transaction batch: %7.2f\n", account_balances[i]);
		omp_destroy_lock(locks[i]);
	}

#ifdef DEBUG
	for (i = 0; i < thread_count; i++) printf("Account %d balance after execution: %f\n", i, account_balances[i]);
#endif
	/* end TODO */

	return 0;		
}

static void init_locks(int count) {
	int i;

	locks = malloc(count * (sizeof(omp_lock_t **)));
	for (i = 0; i < count; i++) {
		locks[i] = malloc(sizeof(omp_lock_t *));
		omp_init_lock(locks[i]);
	}
}

/*------------------------------------------------------------------
 * @brief	get_args
 * Get command line args
 * In args:	argc, argv
 * Out args:	thread_count_p, m_p, n_p
 */
static void get_args(int argc, char* argv[], int* thread_count_p, char*** fname)	{
	int i;

	if (argc <= 2) usage(argv[0]);
	*thread_count_p = strtol(argv[1], NULL, 10);
	if (argc != 2 + *thread_count_p) {
		usage(argv[0]);
	}

    *fname = malloc((*thread_count_p) * sizeof(char *));
	for (i = 0; i < *thread_count_p; i++) {
	  (*fname)[i] = malloc(sizeof(char)*64);
	  strcpy((*fname)[i], argv[2 + i]);
	}
}	/* get_args */

/*------------------------------------------------------------------
 * @brief	usage
 * print a message showing what the command line should be, and terminate
 * In arg:	prog_name
 */
static void usage (char* prog_name) {
	 
	fprintf(stderr, "usage: %s <thread_count> [<datafile.txt>]\n", prog_name);
	exit(0);
}	/* usage */

/*--------------------------------------------------------------------
 * @brief deposit 
 * Add amount to balance 
 * @param acc_num:	Account number
 * @param amount:	Amount to deposit 
 * @param balance:	Balance of acc_num	
 */
static void deposit(int acc_num, double amount)
{
	double prev;

	omp_set_lock(locks[acc_num]);
	prev = account_balances[acc_num];
	account_balances[acc_num] += amount;
	printf("User %d, Depos %7.2f, To Acc %d (Bal %7.2f), %23s -- Successful, Acc %d (Bal %7.2f)\n"
	, omp_get_thread_num(), amount, acc_num, prev
	, "\0", acc_num, account_balances[acc_num]);
	omp_unset_lock(locks[acc_num]);
} 

/*--------------------------------------------------------------------
 * @brief withdrawal 
 * If amount available, subtract amount from balance 
 * @param acc_num:	Account Number
 * @param amount:	Amount to withdraw 
 * @param balance:	Balance of acc_num	
 */
static void withdrawal(int acc_num, double amount)
{
	char buffer[100];

	omp_set_lock(locks[acc_num]);
	sprintf(buffer, "User %d, Withd %7.2f, Fr Acc %d (Bal %7.2f), %23s "
	, omp_get_thread_num(), amount, acc_num, account_balances[acc_num]
	, "\0");

	if (amount <= account_balances[acc_num]) { 
		account_balances[acc_num] -= amount;
		printf("%s-- Successful, Acc %d (Bal %7.2f)\n", buffer, acc_num,
		account_balances[acc_num]);
	} else {
		printf("%s-- Failed: Insufficient Funds\n", buffer);	
	}
	omp_unset_lock(locks[acc_num]);
} 

/*--------------------------------------------------------------------
 * @brief transfer 
 * If amount available in acc1, 
 *  subtract amount from acc1 balance and add to acc2 balance
 * @param acc1:		Number of account from which money is transferred
 * @param acc2:		Number of account to which money is transferred 
 * @param amount:	Amount to transfer 
 * @param balance:	Balance of acc1	
 */
static void transfer(int acc1, int acc2, double amount)
{
	char buffer1[100], buffer2[100];

	omp_set_lock(trans);
	omp_set_lock(locks[acc1]);
	omp_set_lock(locks[acc2]);
	sprintf(buffer1, "User %d, Trans %7.2f, Fr Acc %d (Bal %7.2f), "
	, omp_get_thread_num(), amount, acc1, account_balances[acc1]);
	sprintf(buffer2, " To Acc %d (Bal %7.2f) -- ", acc2, account_balances[acc2]);
	if (amount <= account_balances[acc1]) {
		account_balances[acc1] -= amount;	
		account_balances[acc2] += amount;
		printf("%s%sSuccessful, Acc %d (Bal %7.2f)\n", buffer1, buffer2, acc2,
		account_balances[acc2]);
	} else {
		printf("%s%sFailed: Insufficie\n", buffer1, buffer2);
	}
	omp_unset_lock(locks[acc2]);
	omp_unset_lock(locks[acc1]);
	omp_unset_lock(trans);
}

/*--------------------------------------------------------------------
 * @brief balance 
 * Return the current balance of account acc_num 
 * @param acc_num:	The number of the account 
 * @param balance:	The current balance of account acc_num
 */
static void acc_balance(int acc_num, double* balance)
{
	omp_set_lock(locks[acc_num]);
	*balance = account_balances[acc_num];
	omp_unset_lock(locks[acc_num]);
}

