#include <stdio.h>
#include <mpi.h>
#include "e-hal.h"

int main(int argc, char *argv[]) {
  MPI_Init(&argc, &argv);

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  printf("hello, world from process %d\n", rank);

  e_init(NULL);
  int primary = !(rank % 2);
  int tmp = 0;
  int tag = 0;
  if (primary) {
    e_reset_system();
    MPI_Send(&tmp, 0, MPI_INT, rank + 1, tag, MPI_COMM_WORLD);
  } else {
    MPI_Status status;
    MPI_Recv(&tmp, 0, MPI_INT, rank - 1, tag, MPI_COMM_WORLD, &status);
  }

  e_platform_t platform;
  e_get_platform_info(&platform);
  int rows = platform.rows / 2;
  int cols = platform.cols;
  e_epiphany_t dev;
  e_open(&dev, (primary ? 0 : rows), 0, rows, cols);
  e_reset_group(&dev);

  e_close(&dev);
  tag++;
  if (!primary) {
    MPI_Send(&tmp, 0, MPI_INT, rank - 1, tag, MPI_COMM_WORLD);
  } else {
    MPI_Status status;
    MPI_Recv(&tmp, 0, MPI_INT, rank + 1, tag, MPI_COMM_WORLD, &status);
    e_finalize();
  }
  MPI_Finalize();
  return 0;
}
