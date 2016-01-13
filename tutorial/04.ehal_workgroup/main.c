#include <stdio.h>
#include "e-hal.h"

int main(int argc, char *argv[]) {
  e_platform_t platform;
  e_epiphany_t dev;

  e_init(NULL);
  e_reset_system();
  e_get_platform_info(&platform);
  printf("hello, world with %dx%d eCore\n", platform.rows, platform.cols);

  e_open(&dev, 0, 0, rows, cols);
  e_reset_group(&dev);

  e_finalize();
  return 0;
}
