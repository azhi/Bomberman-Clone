#include <iostream>

#include "controllers/main_controller.h"

int main(int argc, char **argv)
{
  MainController *mc = new MainController();
  mc->main_cycle();
  return 0;
}