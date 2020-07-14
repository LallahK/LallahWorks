#include <stdio.h>

final int K = {{ simple_int_range(varname='K', min=2, max=20000) }};

/* TODO Remove this once the actual explode_bomb has been implemented */
void explode_bomb(int phase_num, int user_input) {
  /*  PASS  */
}

/**
 * This is a terrible implementation for this phase.
 */
int phase_1(int server_input, int user_input) {
  if (server_input * K != user_input) {
    explode_bomb(0, user_input);
  }
}
