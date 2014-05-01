#ifndef __LISTEN_THREAD_H_
#define __LISTEN_THREAD_H_

#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <queue>

struct ProcessJobParams;

namespace Utils
{
  class ListenThread
  {
    public:
      ListenThread(int port, std::queue<ProcessJobParams> *process_queue,
                   std::mutex *process_queue_mutex, std::condition_variable *cv_process_queue);
      ~ListenThread();

      void start();

      static void worker_loop(ListenThread *params);

      int get_socket_fd(){ return socket_fd; };

      bool quit = false;

      std::queue<ProcessJobParams> *process_queue;
      std::mutex *process_queue_mutex;
      std::condition_variable *cv_process_queue;

    private:
      void init_socket(int port);

      std::thread *worker_thread;
      int socket_fd;
      char* buf;
  };
}

#endif
