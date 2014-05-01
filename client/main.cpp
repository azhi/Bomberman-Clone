#include <iostream>

#include "controllers/main_controller.h"

int main(int argc, char **argv)
{
  bool is_bot = argc > 2 && strcmp("--bot", argv[2]) == 0;
  MainController *mc = new MainController(argv[1], is_bot);
  mc->main_cycle();
  return 0;
}