#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void hello_world();

int main (int argc, char* argv[]) {
  int thread_count = strtol(argv[1], NULL, 10);

  # pragma omp parallel num_threads(thread_count)
  {
    hello_world();
  }

  return 0;
}

void hello_world() {
  int thread, total;

  thread = omp_get_thread_num();
  total = omp_get_num_threads();

  printf("Hello, world: Thread %d/%d\n", thread, total);
}
