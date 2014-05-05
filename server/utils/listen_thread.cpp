#include "listen_thread.h"
#include "../../shared/debug.h"
#include "../server.h"
#include "../client.h"

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
  ListenThread::ListenThread(Client* client, int port, std::queue<ProcessJobParams> *process_queue,
                             std::mutex *process_queue_mutex, std::condition_variable *cv_process_queue) :
    client(client), process_queue(process_queue), process_queue_mutex(process_queue_mutex),
    cv_process_queue(cv_process_queue)
  {
    init_socket(port);
  }

  ListenThread::~ListenThread()
  {
    quit = true;
    worker_thread->join();
    delete worker_thread;
    close(socket_fd);
  }

  void ListenThread::start()
  {
    worker_thread = new std::thread(worker_loop, this);
  }

  void ListenThread::worker_loop(ListenThread *parent)
  {
    sockaddr_in *sock_addr = new sockaddr_in;
    socklen_t address_len = sizeof(struct sockaddr);
    fd_set rds;
    timeval tv;
    while(!parent->quit)
    {
      FD_ZERO(&rds);
      FD_SET(parent->get_socket_fd(), &rds);
      tv.tv_sec = 1;
      tv.tv_usec = 0;
      int select_ret = select(parent->socket_fd + 1, &rds, NULL, NULL, &tv);
      if (select_ret > 0)
      {
        int recsize = recvfrom(parent->socket_fd, parent->buf, BUF_SIZE, 0, (struct sockaddr*) sock_addr, &address_len);

        if (parent->get_client()->get_sock_addr() == nullptr)
        {
          sockaddr_in *sock_addr_copy = new sockaddr_in;
          memcpy(sock_addr_copy, sock_addr, address_len);
          parent->get_client()->set_sock_addr(sock_addr_copy, address_len);
        }

        char *msg_copy = new char[recsize];
        strncpy(msg_copy, parent->buf, recsize);
        ProcessJobParams job = {msg_copy, recsize, parent->get_client()};

        std::unique_lock<std::mutex> locker(*parent->process_queue_mutex);
        parent->process_queue->push(job);
        parent->cv_process_queue->notify_one();
      }
    }
    delete sock_addr;
  }

  void ListenThread::init_socket(int port)
  {
    D(std::cerr << "CREATING NEW SOCKET ON PORT " << port << std::endl);
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
