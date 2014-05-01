#ifndef __WRITE_THREAD_H_
#define __WRITE_THREAD_H_

#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <queue>
#include <netinet/in.h>

namespace Utils
{
  struct WriteJobParams
  {
    char *msg;
    int msg_len;
    sockaddr_in *sock_addr;
    socklen_t socklen;
  };

  class WriteThread
  {
    public:
      WriteThread(int socket_fd);
      ~WriteThread();

      void start();

      static void worker_loop(WriteThread* parent);

      bool quit = false;
      int socket_fd;

      std::queue<WriteJobParams> *write_queue;
      std::mutex *write_queue_mutex;
      std::condition_variable *cv_write_queue;

    private:
      std::thread *worker_thread;
  };
}

#endif
