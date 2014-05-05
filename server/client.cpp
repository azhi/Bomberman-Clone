#include "client.h"

#include "../shared/debug.h"
#include "server.h"

#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

Client::Client(Server *server, int port) : server(server), port(port), sock_addr(nullptr)
{
  game_logic = server->get_game_logic();
  listen_thread = new Utils::ListenThread(this, port, server->process_queue, server->process_queue_mutex,
                                          server->cv_process_queue);
  write_thread = new Utils::WriteThread(listen_thread->get_socket_fd());

  listen_thread->start();
  write_thread->start();
}

Client::~Client()
{
}

void Client::process_first_send(char *msg, int msg_len)
{
  do_async_write(msg, msg_len);
  sent_create_character = true;
  send_full_state();
}

void Client::async_write(char *msg, int msg_len, bool force_full_state)
{
  if (!sent_create_character)
    process_first_send(msg, msg_len);
  else
    if (force_full_state)
    {
      sents_without_sync = 0;
      do_async_write(msg, msg_len);
    }
    else
      if (sents_without_sync > MAX_SENTS_WITHOUT_SYNC)
        send_full_state();
      else
      {
        sents_without_sync++;
        do_async_write(msg, msg_len);
      }
}

void Client::send_full_state()
{
  sents_without_sync = 0;
  char* msg = game_logic->get_current_full_state();
  do_async_write(msg, strlen(msg) + 1);
  delete[] msg;
}

void Client::do_async_write(char *msg, int msg_len)
{
  if (sock_addr != nullptr)
  {
    char* msg_copy = new char[msg_len];
    strncpy(msg_copy, msg, msg_len);
    Utils::WriteJobParams job = {msg_copy, msg_len, sock_addr, sock_addr_len};

    std::unique_lock<std::mutex> locker(*write_thread->write_queue_mutex);
    write_thread->write_queue->push(job);
    write_thread->cv_write_queue->notify_one();
  }
}
