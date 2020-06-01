#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char *argv[]) {
    int *sums;
    
    if (argc != 3) {
        printf("./version1 <threads> <numbers>\n");
        return 1;
    }
    
    int threads = atoi(argv[1]);
    int numbers = atoi(argv[2]);
    int executions = numbers / threads;

    sums = malloc(threads * sizeof(int));
    for (int i = 0; i < threads; i++) {
        sums[i] = 0;
    }

    printf("threads: %d \nnumbers: %d \n", threads, numbers);

    double time = omp_get_wtime();
#pragma omp parallel num_threads(threads) 
{
    int my_rank = omp_get_thread_num();
    
    for (int i = 0; i < executions; i++) {
        int x = rand() % 100;
        sums[my_rank] += x;
    }

}
    long int sum = 0;
    for (int i = 0; i < threads; i++) {
        sum += sums[i];
    }
    time = omp_get_wtime() - time;

    printf("result: %li\n", sum);
    printf("time: %7.5f\n", time);
    return 0;
}
