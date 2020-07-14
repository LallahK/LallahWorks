#include "phases.h"
#include "bomb_core.h"

int (*phase_funcs[])(char *, char *) = { phase_1, phase_2, phase_3, phase_4, phase_5 };

static void do_phase() {
  int result;
  char *server_input, *student_input;

  server_input = get_phase_info();
  student_input = read_line();

  result = phase_funcs[phase_num - 1](server_input, student_input);

  free(server_input);

  if (result == EXPLODED) {
    explode_bomb(student_input);
  } else {
    defuse_phase(student_input);
  }
}

int main() {

  bomb_init();

  /* TODO Think of something more fun for this */
  printf("Welcome to the CS244 Bomblab\n");

  for (int i = 1; i <= 5; i++) {
    phase_num = i;
    do_phase();
  }

  /* TODO Phase 6 */
  printf("Congratulations, you have defused the bomb!\n");

  bomb_exit();
}
