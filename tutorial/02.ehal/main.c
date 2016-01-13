#include <stdio.h>
#include "e-hal.h"

int main(int argc, char *argv[]) {
  e_init(NULL);

  printf("hello, world\n");

  e_finalize();
  return 0;
}
