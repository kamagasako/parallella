#include <stdio.h>
#include <unistd.h>
#include "e-hal.h"
#include "e-loader.h"

int main(int argc, char *argv[]) {
  e_platform_t platform;
  e_epiphany_t dev;

  e_init(NULL);
  e_reset_system();
  e_get_platform_info(&platform);

  e_open(&dev, 0, 0, platform.rows, platform.cols);
  e_reset_group(&dev);

  e_load_group((char*) "e-hello.srec", &dev, 0, 0, platform.rows, platform.cols, E_TRUE);

  usleep(10000);
  int core_id;
  for (int i = 0; i < platform.rows; i++) {
    for (int j = 0; j < platform.cols; j++) {
      e_read(&dev, i, j, 0x2000, &core_id, sizeof(int));
      printf("hello, world from eCore: 0x%03X (%d, %d)\n", core_id, i, j);
    }
  }

  e_finalize();
  return 0;
}
