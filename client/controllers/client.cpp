#include "client.h"

#include "../../shared/debug.h"
#include "../../shared/messages/client.h"
#include "../../shared/messages/server.h"
#include "../../shared/character_move_directions.h"
#include "../game_objects/character.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

Client::Client(long ip, int port, GameObjects::Map *map, SDLWrapper *sdl_wrapper) :
  ip(ip), port(port), map(map), sdl_wrapper(sdl_wrapper)
{
  init_socket();
}

Client::~Client()
{
  close(socket_fd);
}

void Client::write(char* cmd)
{
  struct sockaddr_in sock_addr;
  memset(&sock_addr, 0, sizeof(sock_addr));

  sock_addr.sin_family = PF_INET;
  sock_addr.sin_port = htons(port);
  sock_addr.sin_addr.s_addr = htonl(ip);

  sendto(socket_fd, cmd, strlen(cmd) + 1, 0, (sockaddr*) &sock_addr, sizeof(sockaddr_in));
}

void Client::read_and_process()
{
  timeval tv;
  tv.tv_sec = 0;
  tv.tv_usec = 10;

  sockaddr_in sock_addr;
  socklen_t address_len = sizeof(sockaddr);
  int select_ret = wait_for_socket(&tv);
  if (select_ret > 0)
  {
    int recsize = recvfrom(socket_fd, buf, BUF_SIZE, 0, (sockaddr*) &sock_addr, &address_len);
    D(std::cerr << "RECEIVED: " << (int) buf[0] << std::endl);
    process_single_msg();
  }
}

void Client::do_register()
{
  send_register();
  sockaddr_in sock_addr;
  socklen_t address_len = sizeof(sockaddr);
  int select_ret = wait_for_socket(NULL);
  if (select_ret > 0)
  {
    int recsize = recvfrom(socket_fd, buf, BUF_SIZE, 0, (sockaddr*) &sock_addr, &address_len);
    char cmd = buf[0];
    if (cmd != CREATE_CHARACTER_CMD)
    {
      std::cerr << "Server answered not with CREATE_CHARACTER after registration. Terminating..." << std::endl;
      exit(EXIT_FAILURE);
    }
    process_add_character(true);
  }
}

void Client::send_register()
{
  char msg[2];
  msg[0] = REGISTER_CMD;
  msg[1] = '\0';
  write(msg);
}

void Client::send_character_move(char move_direction)
{
  char msg[2];
  msg[0] = MOVE_CMD;
  msg[0] |= move_direction;
  msg[1] = '\0';
  write(msg);
}

void Client::send_place_bomb(bool previous_location)
{
  char msg[2];
  msg[0] = PLACE_BOMB_CMD;
  if (previous_location)
    msg[0] |= 0x01;
  msg[1] = '\0';
  write(msg);
}

void Client::process_single_msg()
{
  char cmd = buf[0];
  switch(cmd)
  {
    case FULL_STATE_DUMP_CMD:
      map->restore_from_binary_dump(buf);
      break;
    case CREATE_CHARACTER_CMD:
      process_add_character(false);
      break;
  }
}

int Client::wait_for_socket(timeval* tv)
{
  FD_ZERO(&rds);
  FD_SET(socket_fd, &rds);

  int select_ret = select(socket_fd + 1, &rds, NULL, NULL, tv);
  return select_ret;
}

void Client::process_add_character(bool main_character)
{
  char object_id = buf[1];
  char x = buf[2];
  char y = buf[3];
  char last_move = buf[4];

  GameObjects::Character *character = new GameObjects::Character(sdl_wrapper, map, object_id,
                                                                 x, y, last_move, false,
                                                                 main_character);
  map->add_character(character);

  if (main_character)
    map->set_main_character(character);
}

void Client::init_socket()
{
  socket_fd = socket( PF_INET, SOCK_DGRAM, IPPROTO_UDP );

  if ( socket_fd == -1 )
  {
    std::cerr << "Error creating socket" << std::endl;
    exit(EXIT_FAILURE);
  }

  buf = new char[BUF_SIZE];

  FD_ZERO(&rds);
  FD_SET(socket_fd, &rds);
}
