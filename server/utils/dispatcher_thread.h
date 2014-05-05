#ifndef __DISPATCHER_THREAD_H_
#define __DISPATCHER_THREAD_H_

#include <thread>

class Server;

namespace Utils
{
  class DispatcherThread
  {
    public:
      DispatcherThread(Server *server, int port);
      ~DispatcherThread();

      void start();

      static void worker_loop(DispatcherThread *params);

      int get_socket_fd(){ return socket_fd; };
      Server *get_server(){ return server; };
      int next_free_port();

      bool quit = false;

    private:
      void init_socket(int port);

      Server *server;
      std::thread *worker_thread;
      int socket_fd;
      int last_free_port;
      char* buf;
  };
}

#endif
