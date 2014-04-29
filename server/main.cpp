#include <cstdio>

#include "server.h"
#include "game_objects/map.h"
#include "game_objects/character.h"
#include "game_objects/bomb.h"

int main(int argc, char **argv)
{
  Server* server = new Server(5555);
  server->listen();
  return 0;
}
