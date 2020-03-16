/* vim settings: :se ts=4 
 * File:    mpi_psum.c
 *
 * Purpose: Implement a parallel algorithm for calculating the sum 
 *          of a list of values (the list may be divided bewteen a 
 *          number of files). 
 *
 * Sources: This is a modified version of Pacheco's mpi_vector_add.c 
 *
 * Compile: make / mpicc -g -Wall -o mpi_psum.o mpi_psum.c
 * Run:     . run.sh 
 *
 * Input:   The file(s) that contain the list(s) of values 
 * Output:  The average of all the values 
 *
 * Notes:
 * 1.The order of the vectors, n, should be evenly divisible
 *   by comm_sz
 * 2.DEBUG compile flag.    
 * 3.This program does fairly extensive error checking.  When
 *   an error is detected, a message is printed and the processes
 *   quit.  Errors detected are incorrect values of the vector
 *   order (negative or not evenly divisible by comm_sz), and
 *   malloc failures.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

void get_args(int argc, char* argv[], char** input_file);
void usage(char* prog_name);
void check_for_error(int local_ok, char fname[], char message[], 
	MPI_Comm comm);
void allocate_vector(int** local_x_pp, int local_n, MPI_Comm comm);
void read_and_scatter_data(int *n_p, int* local_n_p, int** local_a, 
	char* input_file, int my_rank, int comm_sz, MPI_Comm comm);
void gather_and_print_vector(int local_b[], int local_n, int n, 
	char title[], int my_rank, MPI_Comm comm);
void calc_local_sum(int local_x[], int local_n, int my_rank, double *local_sum);
void calc_global_sum(double local_sum, double *global_sum, MPI_Comm comm);

/*-------------------------------------------------------------------*/
int main(int argc, char* argv[]) {
	int      n, local_n;
	int      comm_sz, my_rank;
	int*     local_x;
	MPI_Comm comm;
	char*    input_file = NULL;
	double   local_sum = 0, global_sum = 0;
	double start, finish;
	
	get_args(argc, argv, &input_file);
	MPI_Init(NULL, NULL);
	comm = MPI_COMM_WORLD;
	MPI_Comm_size(comm, &comm_sz);
	MPI_Comm_rank(comm, &my_rank);

	start = MPI_Wtime(); 	
	read_and_scatter_data(&n, &local_n, &local_x, 
			input_file, my_rank, comm_sz, comm);
	finish = MPI_Wtime(); 	
	if (my_rank == 0) {
		printf("Read and Scatter took %e seconds \n", finish-start);
	}

	#ifdef DEBUG
	gather_and_print_vector(local_x, local_n, n, "The full vector is:", 
		my_rank, comm);
	#endif

	start = MPI_Wtime(); 	
	calc_local_sum(local_x, local_n, my_rank, &local_sum);
	calc_global_sum(local_sum, &global_sum, comm); 
	finish = MPI_Wtime(); 	

	if (my_rank == 0) {
		printf("The global sum = %f \n", global_sum);
		printf("Calculation took %e seconds \n", finish-start);
	}
	free(local_x);
	MPI_Finalize();

	return 0;
}  /* main */

/*-------------------------------------------------------------------
 * Function: get_args
 * Purpose:  assign command line args
 * In:       argc, argv
 * Out:      input_file
 */
void get_args(int argc, char* argv[], char** input_file){
	if (argc != 2) usage(argv[0]);

 	*input_file = malloc(sizeof(char)*strlen(argv[1]));	
	strcpy(*input_file, argv[1]);

} /* get_args */

/*---------------------------------------------------------------------------
 * Function: usage 
 * Purpose:  print a message showing what command line arguments are needed 
 * In:       prog_name 
 * Out: 
 */
void usage(char* prog_name){
    fprintf(stderr, "Usage: %s <filename> \n", prog_name);
	exit(0);
} /* usage */

/*-------------------------------------------------------------------
 * Function:  check_for_error
 * Purpose:   Check whether any process has found an error.  If so,
 *            print message and terminate all processes.  Otherwise,
 *            continue execution.
 * In args:   local_ok: 1 if calling process has found an error, 0
 *            otherwise
 *    fname:  name of function calling check_for_error
 *    message:message to print if there's an error
 *    comm:   communicator containing processes calling
 *    check_for_error:should be MPI_COMM_WORLD.
 * Note:
 *    The communicator containing the processes calling check_for_error
 *    should be MPI_COMM_WORLD.
 */
void check_for_error(
	int      local_ok  /* in */, 
	char     fname[]   /* in */,
	char     message[] /* in */, 
	MPI_Comm comm      /* in */) {

	int ok;

	MPI_Allreduce(&local_ok, &ok, 1, MPI_INT, MPI_MIN, comm);
	if (ok == 0) {
		int my_rank;
		MPI_Comm_rank(comm, &my_rank);
		if (my_rank == 0) {
			fprintf(stderr, "Proc %d > In %s, %s\n", my_rank, 
				fname, message);
			fflush(stderr);
		}
		MPI_Finalize();
		exit(-1);
	}
}  /* check_for_error */

/*-------------------------------------------------------------------
 * Function:allocate_vector
 * Purpose: allocate storage for x
 * In args: local_n:  the size of the local vector
 *          comm:     the communicator containing the calling processes
 * Out args:local_x_pp: pointers to memory blocks to be allocated for 
 *                      the local vector
 * Errors:  One or more of the calls to malloc fails
 */
void allocate_vector(
	int      ** local_x_pp /* out */, 
	int      local_n    /* in  */,
	MPI_Comm comm       /* in  */) {

	int		local_ok = 1;
	char*	fname = "allocate_vector";

	*local_x_pp = malloc(local_n*sizeof(int));

	if (*local_x_pp == NULL) local_ok = 0;
	check_for_error(local_ok, fname, 
		"Can't allocate local vector", comm);
}  /* allocate_vector */

/*-------------------------------------------------------------------
 * Function: read_and_scatter_data
 * Purpose:  Get the order of the vectors from stdin on proc 0 and
 *           broadcast to other processes.
 *           Read a vector from a file and distribute
 *           among the processes using a block distribution.
 * In args:  local_n: size of local vectors
 *           n:       size of global vector
 *           vec_name:name of vector being read (e.g., "x")
 *           my_rank: calling process' rank in comm
 *           comm_sz:    number of processes in communicator
 *           comm:    communicator containing calling processes
 * Out args: local_a:   local vector read
 *           n_p:       global value of n
 *           local_n_p: local value of n = n/comm_sz
 *           n should be positive and evenly divisible by comm_sz
 *
 * Errors:   if the malloc on process 0 for temporary storage
 *           fails the program terminates
 *
 * Note: 
 *           This function assumes a block distribution and the order
 *           of the vector evenly divisible by comm_sz.
 */

void read_and_scatter_data(
	int*     n_p        /* out */,
	int*     local_n_p  /* out */, 
	int**    local_a    /* out */, 
	char*    input_file /* in  */,
	int      my_rank    /* in  */, 
	int      comm_sz    /* in  */, 
	MPI_Comm comm       /* in  */) {

	int* a = NULL;
	int i;
	int local_ok = 1;
	char* fname = "read_and_scatter_data";
	FILE* fp;
	#ifdef DEBUG
	char* debug_fname;
	#endif

	if (my_rank == 0) {
		fprintf(stderr,"Reading data from: %s\n", input_file);
		fp = fopen(input_file,"r");
		if (fp == NULL)	local_ok = 0;
		check_for_error(local_ok, fname, 
			"Can't open data file", comm);

		/* Read n */
		fscanf(fp, "%d", n_p);
		if ((*n_p <= 0) || (*n_p % comm_sz != 0)) local_ok = 0;
		check_for_error(local_ok, fname,
			"n should be > 0 and evenly divisible by comm_sz", comm);
		/* Read vector a */
		a = malloc((*n_p)*sizeof(int));
		if (a == NULL) local_ok = 0;
		check_for_error(local_ok, fname, 
			"Can't allocate temporary vector", comm);
		for (i = 0; i < *n_p; i++)
			fscanf(fp, "%d", &a[i]);
	
/* TODO: 
		   Broadcast local n
		   Scatter the values of vector a between the local vectors 
*/

		free(a);
		fclose(fp);
	} else {
		check_for_error(local_ok, fname, 
			"Can't open data file", comm);
		check_for_error(local_ok, fname,
			"n should be > 0 and evenly \
			divisible by comm_sz", comm);
		check_for_error(local_ok, fname, 
			"Can't allocate temporary vector", comm);

/* TODO: 
		   Receive local n from process 0
		   Receive local vector from process 0 
*/

	}
}  /* read_and_scatter_data */  

/*-------------------------------------------------------------------
 * Function:gather_and_print_vector
 * Purpose: Print a vector that has a block distribution to stdout
 * In args: local_x:local storage for vector to be printed
 *          local_n:order of local vectors
 *          n:      order of global vector (local_n*comm_sz)
 *          msg:    message to precede print out
 *          comm:   communicator containing processes calling
 *          gather_and_print_vector
 *
 * Error:   if process 0 can't allocate temporary storage for
 *          the full vector, the program terminates.
 *
 * Note:
 *          Assumes order of vector is evenly divisible by the 
 *          number of processes
 *          Process 0 should print the vector 
 */
void gather_and_print_vector(
	int      local_x[] /* in */, 
	int      local_n   /* in */, 
	int      n         /* in */, 
	char     msg[]     /* in */, 
	int      my_rank   /* in */,
	MPI_Comm comm      /* in */) {

	int i;
	int* x = NULL; 
	int local_ok = 1;
  char* fname = "gather_and_print_vector";

  if (my_rank == 0) {
    x = malloc(n*sizeof(int));
    if (x == NULL) local_ok = 0;
    check_for_error(local_ok, fname,
      "Can't allocate temporary vector", comm);

    /* TODO: execute gather*/

    /* print vector of concatenated local vectors */
    printf("\n%s:\n", msg);
    for (i = 0; i < n; i++) {
      printf("%d ", x[i]);
      if ((i+1)%local_n == 0) printf("; ");
    }
    free(x);	
   }  else {
    check_for_error(local_ok, fname,
      "Can't allocate temporary vector", comm);

    /* TODO: execute gather */

   }
}  /* gather_and_print_vector */

/*-------------------------------------------------------------------
 * Function:   calc_local_sum
 * Purpose:    Calculate the sum of a vector of values 
 * In args:    local_x: the vector of values 
 * In args:    local_n: the number of values in local_x 
 * Out arg:    local_sum: sum of all the values in local_x 
 */
void calc_local_sum(
	int    local_x[]  /* in  */,
	int    local_n    /* in  */,
	int    my_rank    /* in  */,
	double    *local_sum /* out */) {

/* TODO */

}  /* calc_local_sum */

/*-------------------------------------------------------------------
 * Function: calc_global_sum 
 * Purpose:  Calculate the total sum of the local sums calculated 
 *           by each thread on it's subset of the data (use a collective communication command
 * In args:  local_sum for this thread
 * Out arg:  global_sum: total sum of the sub-sums calculated 
 *           by each thread 
 */
void calc_global_sum(
	double   local_sum   /* in  */, 
	double   *global_sum /* out */, 
	MPI_Comm comm        /* in  */) {

/* TODO */

}  /* calc_global_sum using a collective comms command*/
