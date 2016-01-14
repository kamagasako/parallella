#include "e-lib.h"
#include "addr.h"

int main(int argc, char *argv[]) {
  e_coreid_t core_id = e_get_coreid();

  int row, col;
  e_coords_from_coreid(core_id, &row, &col);
  e_write(&e_group_config, &core_id, row, col, (int*) ADDR, sizeof(int));

  return 0;
}
