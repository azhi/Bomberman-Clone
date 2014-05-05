#ifndef __LISTEN_THREAD_H_
#define __LISTEN_THREAD_H_

#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <queue>

struct ProcessJobParams;
class Client;

namespace Utils
{
  class ListenThread
  {
    public:
      ListenThread(Client* client, int port, std::queue<ProcessJobParams> *process_queue,
                   std::mutex *process_queue_mutex, std::condition_variable *cv_process_queue);
      ~ListenThread();

      void start();

      static void worker_loop(ListenThread *parent);

      int get_socket_fd(){ return socket_fd; };
      Client *get_client(){ return client; };

      bool quit = false;

      std::queue<ProcessJobParams> *process_queue;
      std::mutex *process_queue_mutex;
      std::condition_variable *cv_process_queue;

    private:
      void init_socket(int port);

      Client* client;
      std::thread *worker_thread;
      int socket_fd;
      char* buf;
  };
}

#endif
