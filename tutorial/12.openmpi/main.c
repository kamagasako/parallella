#include <stdio.h>

int main(int argc, char *argv[]) {
  MPI_Init(&argc, &argv);

  printf("hello, world\n");

  MPI_Finalize();
  return 0;
}
