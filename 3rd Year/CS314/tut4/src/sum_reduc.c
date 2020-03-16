#include <stdio.h>
#include <string.h>
#include <time.h>
#include <mpi.h>

int method_1(int);
int method_2(int);

const int MAX_STRING = 100;

int main(void) {
	MPI_Init(NULL, NULL);
	int ranks, rank;
	double start, end;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	start = MPI_Wtime();
	ranks = method_1(rank);
	end = MPI_Wtime();
	if (rank == 0) {
		printf("%-25s (%10.8f) : %d\n", "Collective Communications", (end - start), ranks);
	}

	start = MPI_Wtime();
	ranks = method_2(rank);
	end = MPI_Wtime();
	if (rank == 0) {
		printf("%-25s (%10.8f) : %d\n", "Point-to-point", (end - start), ranks);
	}

	MPI_Finalize();
}

int method_1(int rank) {
	int 	total = 0;

	MPI_Reduce(&rank, &total, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);	
	printf("%d - %d\n", total, rank);
	return total;
}

int method_2(int rank) {
	int 	comms;
	int 	total = 0;

	if (rank == 0) {
		MPI_Comm_size(MPI_COMM_WORLD, &comms);
		int val;
		for (int i = 1; i < comms; i++) {
			MPI_Recv(&val, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			total = total + val;
		}
	} else {
		MPI_Send(&rank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}
	return total;
}
