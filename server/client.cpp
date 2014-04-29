#include "client.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

Client::Client(int socket_fd, sockaddr_in *sock_addr, socklen_t sock_addr_len)
  : socket_fd(socket_fd), sock_addr_len(sock_addr_len)
{
  this->sock_addr = new sockaddr_in;
  memcpy(this->sock_addr, sock_addr, sock_addr_len);
}

Client::~Client()
{
}

void Client::write(char* msg, size_t msg_len)
{
  sendto(socket_fd, msg, msg_len, 0, (struct sockaddr*) sock_addr, sock_addr_len);
}
