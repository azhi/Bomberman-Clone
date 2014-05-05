#include "server.h"

#include "../shared/debug.h"
#include "../shared/messages/client.h"
#include "../shared/messages/server.h"
#include "../shared/character_move_directions.h"
#include "game_objects/character.h"
#include "game_objects/bomb.h"

#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

Server::Server(int port)
{
  game_logic = new GameLogic(this);
  clients = new std::list<Client*>;

  process_queue_mutex = new std::mutex;
  cv_process_queue = new std::condition_variable;
  process_queue = new std::queue<ProcessJobParams>;
  listen_thread = new Utils::ListenThread(port, process_queue, process_queue_mutex, cv_process_queue);
  listen_thread->start();

  write_thread = new Utils::WriteThread(listen_thread->get_socket_fd());
  write_thread->start();
}

Server::~Server()
{
  delete listen_thread;
}

void Server::process()
{
  while(true)
  {
    ProcessJobParams picked_job;
    std::unique_lock<std::mutex> locker(*process_queue_mutex);

    cv_process_queue->wait(locker);

    while(!process_queue->empty())
    {
      picked_job = process_queue->front();
      process_queue->pop();
      locker.unlock();
      process_single_msg(picked_job.buf, picked_job.buf_len, picked_job.sock_addr, picked_job.address_len);
      // send_full_state();
      locker.lock();
    }
  }
}

void Server::process_single_msg(char* buf, size_t buf_len, sockaddr_in *sock_addr, socklen_t address_len)
{
  unsigned char cmd = buf[0] & 0xF8;
  switch(cmd)
  {
    case REGISTER_CMD:
    {
      Client* client = new Client(game_logic, write_thread, sock_addr, address_len);
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
    case CL_PLACE_BOMB_CMD:
    {
      Client* client = find_client_by_sockaddr(sock_addr);
      char previous_location = buf[0] & 0x0F;
      game_logic->place_bomb(client->get_character_object_id(), previous_location);
      break;
    }
    case UNREGISTER_CMD:
    {
      Client* client = find_client_by_sockaddr(sock_addr);
      game_logic->unregister_character(client);
      remove_client(client->get_character_object_id());
      break;
    }
  }
}

void Server::send_full_state()
{
  char* msg = game_logic->get_current_full_state();
  send_to_all_clients(msg, strlen(msg) + 1, true);
  delete[] msg;
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

void Server::send_delete_character(char object_id)
{
  char res[3];
  res[0] = DELETE_CHARACTER_CMD;
  res[1] = object_id;
  res[2] = '\0';
  send_to_all_clients(res, 3);
}

void Server::send_move_character(char object_id, char x, char y, char last_move)
{
  char res[6];
  res[0] = MOVE_CHARACTER_CMD;
  res[1] = object_id;
  res[2] = x;
  res[3] = y;
  res[4] = last_move;
  res[5] = '\0';
  send_to_all_clients(res, 6);
}

void Server::send_kill_character(char *object_ids, int object_count)
{
  char res[object_count + 2];
  res[0] = KILL_CHARACTER_CMD;
  for(int i = 0; i < object_count; i++)
    res[i+1] = object_ids[i];
  res[object_count + 1] = '\0';
  send_to_all_clients(res, object_count + 2);
}

void Server::send_place_bomb(char object_id, char x, char y)
{
  char res[5];
  res[0] = PLACE_BOMB_CMD;
  res[1] = object_id;
  res[2] = x;
  res[3] = y;
  res[4] = '\0';
  send_to_all_clients(res, 5);
}

void Server::send_destruct_cell(char* xys, int xys_length)
{
  char res[xys_length + 2];
  res[0] = DESTRUCT_CELL_CMD;
  for(int i = 0; i < xys_length; i++)
    res[i+1] = xys[i];
  res[xys_length + 1] = '\0';
  send_to_all_clients(res, xys_length + 2);
}

void Server::send_to_all_clients(char* msg, size_t msg_len, bool force_full_state)
{
  for(std::list<Client*>::iterator client = clients->begin();
      client != clients->end();
      client++)
    (*client)->async_write(msg, msg_len, force_full_state);
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

