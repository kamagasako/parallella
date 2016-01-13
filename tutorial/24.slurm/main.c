#include <stdio.h>
#include <mpi.h>
#include <unistd.h>
#include "e-hal.h"
#include "e-loader.h"

int main(int argc, char *argv[]) {
  MPI_Init(&argc, &argv);

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

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

  e_load_group((char*) "e-hello.srec", &dev, 0, 0, rows, cols, E_TRUE);
  usleep(10000);
  int core_id;
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      e_read(&dev, i, j, 0x2000, &core_id, sizeof(int));
      printf("hello, world from process %d, eCore: 0x%03X (%d, %d)\n", rank, core_id, i, j);
    }
  }

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
