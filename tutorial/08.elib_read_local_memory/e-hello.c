#include "e-lib.h"

int main(int argc, char *argv[]) {
  e_coreid_t core_id = e_get_coreid();

  int row, col;
  e_coords_from_coreid(core_id, &row, &col);
  e_write(&e_group_config, &core_id, row, col, (int*) 0x2000, sizeof(int));

  return 0;
}
