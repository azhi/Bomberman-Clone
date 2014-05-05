#include "dispatcher_thread.h"
#include "../server.h"
#include "../../shared/messages/client.h"
#include "../../shared/debug.h"

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

namespace Utils
{
  DispatcherThread::DispatcherThread(Server *server, int port) : server(server)
  {
    last_free_port = port;
    init_socket(port);
  }

  DispatcherThread::~DispatcherThread()
  {
    quit = true;
    worker_thread->join();
    delete worker_thread;
    close(socket_fd);
  }

  void DispatcherThread::start()
  {
    worker_thread = new std::thread(worker_loop, this);
  }

  int DispatcherThread::next_free_port()
  {
    last_free_port++;
    return last_free_port;
  }

  void DispatcherThread::worker_loop(DispatcherThread* parent)
  {
    sockaddr_in *sock_addr = new sockaddr_in;
    socklen_t address_len = sizeof(struct sockaddr);
    fd_set rds;
    timeval tv;
    std::cerr << "STARTED LISTENING..." << std::endl;
    while(!parent->quit)
    {
      FD_ZERO(&rds);
      FD_SET(parent->get_socket_fd(), &rds);
      tv.tv_sec = 1;
      tv.tv_usec = 0;
      int select_ret = select(parent->get_socket_fd() + 1, &rds, NULL, NULL, &tv);
      if (select_ret > 0)
      {
        int recsize = recvfrom(parent->get_socket_fd(), parent->buf, BUF_SIZE, 0, (struct sockaddr*) sock_addr, &address_len);

        if (parent->buf[0] == REGISTER_CMD)
        {
          Server *server = parent->get_server();
          int port = parent->next_free_port();
          server->add_client(new Client(server, port));

          char response[3];
          response[0] = (port >> 8) & 0xFF;
          response[1] = port & 0xFF;
          response[2] = '\0';
          sendto(parent->get_socket_fd(), response, 3, 0, (struct sockaddr*) sock_addr, address_len);
        }
      }
    }
    delete sock_addr;
  }

  void DispatcherThread::init_socket(int port)
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
}
