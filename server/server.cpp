#include "server.h"

#include "../shared/messages/client.h"
#include "../shared/messages/server.h"
#include "../shared/character_move_directions.h"
#include "game_objects/character.h"
#include "game_objects/bomb.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

Server::Server(int port)
{
  init_socket(port);
  game_logic = new GameLogic(this);
  clients = new std::list<Client*>;
}

Server::~Server()
{
  close(socket_fd);
}

void Server::listen()
{
  sockaddr_in *sock_addr = new sockaddr_in;
  socklen_t address_len = sizeof(struct sockaddr);
  fd_set rds;
  FD_ZERO(&rds);
  FD_SET(socket_fd, &rds);
  std::cerr << "STARTED LISTENING..." << std::endl;
  while(true)
  {
    int select_ret = select(socket_fd + 1, &rds, NULL, NULL, NULL);
    if (select_ret > 0)
    {
      int recsize = recvfrom(socket_fd, buf, BUF_SIZE, 0, (struct sockaddr*) sock_addr, &address_len);
      process_single_msg(sock_addr, address_len);
      send_full_state();
    }
  }
  delete sock_addr;
}

void Server::process_single_msg(sockaddr_in *sock_addr, socklen_t address_len)
{
  unsigned char cmd = buf[0] & 0xF8;
  switch(cmd)
  {
    case REGISTER_CMD:
    {
      Client* client = new Client(socket_fd, sock_addr, address_len);
      add_client(client);
      game_logic->register_new_character(client);
      break;
    }
    case MOVE_CMD:
    {
      Client* client = find_client_by_sockaddr(sock_addr);
      char direction = buf[0] & 0x0F;
      game_logic->move_character(client->get_character_object_id(), direction);
      break;
    }
    case PLACE_BOMB_CMD:
    {
      Client* client = find_client_by_sockaddr(sock_addr);
      char previous_location = buf[0] & 0x0F;
      game_logic->place_bomb(client->get_character_object_id(), previous_location);
      break;
    }
  }
}

void Server::send_full_state()
{
  char* current_state = game_logic->get_current_full_state();
  send_to_all_clients(current_state, strlen(current_state) + 1);
  delete[] current_state;
}

void Server::send_create_character(char object_id, char x, char y, char last_move)
{
  char res[6];
  res[0] = CREATE_CHARACTER_CMD;
  res[1] = object_id;
  res[2] = x;
  res[3] = y;
  res[4] = last_move;
  res[5] = '\0';
  send_to_all_clients(res, 6);
}

void Server::send_destruct_cell(char x, char y)
{
  char res[4];
  res[0] = DESTRUCT_CELL_CMD;
  res[1] = x;
  res[2] = y;
  res[3] = '\0';
  send_to_all_clients(res, 4);
}

void Server::send_to_all_clients(char* msg, size_t msg_len)
{
  for(std::list<Client*>::iterator client = clients->begin();
      client != clients->end();
      client++)
    (*client)->write(msg, msg_len);
}

void Server::add_client(Client* client)
{
  clients->push_back(client);
}

void Server::remove_client(char object_id)
{
  for(std::list<Client*>::iterator client = clients->begin();
      client != clients->end();
      client++)
    if ((*client)->get_character_object_id() == object_id)
    {
      clients->erase(client);
      delete *client;
      break;
    }
}

Client* Server::find_client_by_sockaddr(sockaddr_in *sock_addr)
{
  for(std::list<Client*>::iterator client = clients->begin();
      client != clients->end();
      client++)
  {
    sockaddr_in *client_sock_addr = (*client)->get_sock_addr();
    if (client_sock_addr->sin_port == sock_addr->sin_port &&
        client_sock_addr->sin_addr.s_addr == sock_addr->sin_addr.s_addr)
      return *client;
  }
  return NULL;
}

Client* Server::find_client_by_character_id(char character_object_id)
{
  for(std::list<Client*>::iterator client = clients->begin();
      client != clients->end();
      client++)
    if ((*client)->get_character_object_id() == character_object_id)
      return *client;
  return NULL;
}

void Server::init_socket(int port)
{
  socket_fd = socket( PF_INET, SOCK_DGRAM, IPPROTO_UDP );

  if ( socket_fd == -1 )
  {
    std::cerr << "Error creating socket" << std::endl;
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in sock_addr;
  memset(&sock_addr, 0, sizeof(sock_addr));

  sock_addr.sin_family = PF_INET;
  sock_addr.sin_port = htons(port);
  sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  if ( bind(socket_fd, (sockaddr*) &sock_addr, sizeof(sock_addr)) == -1 )
  {
    std::cerr << "Error binding socket" << std::endl;
    close(socket_fd);
    exit(EXIT_FAILURE);
  }
  buf = new char[BUF_SIZE];
}
