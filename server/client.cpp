#include "client.h"

#include "../shared/debug.h"

#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

Client::Client(sockaddr_in *sock_addr, socklen_t sock_addr_len)
  : sock_addr(sock_addr), sock_addr_len(sock_addr_len)
{
}

Client::~Client()
{
}

void Client::async_write(char *msg, int msg_len, Utils::WriteThread *write_thread)
{
  char* msg_copy = new char[msg_len];
  strncpy(msg_copy, msg, msg_len);
  Utils::WriteJobParams job = {msg_copy, msg_len, sock_addr, sock_addr_len};

  std::unique_lock<std::mutex> locker(*write_thread->write_queue_mutex);
  write_thread->write_queue->push(job);
  write_thread->cv_write_queue->notify_one();
}
