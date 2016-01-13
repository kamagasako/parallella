#include <stdio.h>
#include <mpi.h>

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
    MPI_Send(&tmp, 1, MPI_INT, rank + 1, tag, MPI_COMM_WORLD);
  } else {
    MPI_Status status;
    MPI_Recv(&tmp, 1, MPI_INT, rank - 1, tag, MPI_COMM_WORLD, &status);
  }

  if (primary) {
    e_finalize();
  }
  MPI_Finalize();
  return 0;
}
