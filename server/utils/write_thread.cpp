#include "write_thread.h"
#include "../../shared/debug.h"

#include <iostream>

namespace Utils
{
  WriteThread::WriteThread(int socket_fd) : socket_fd(socket_fd)
  {
    write_queue = new std::queue<WriteJobParams>;
    write_queue_mutex = new std::mutex;
    cv_write_queue = new std::condition_variable;
  }

  WriteThread::~WriteThread()
  {
    quit = true;
    worker_thread->join();
    delete worker_thread;
  }

  void WriteThread::start()
  {
    worker_thread = new std::thread(worker_loop, this);
  }

  void WriteThread::worker_loop(WriteThread* parent)
  {
    while(!parent->quit)
    {
      WriteJobParams picked_job;
      std::unique_lock<std::mutex> locker(*parent->write_queue_mutex);

      parent->cv_write_queue->wait(locker);

      if (parent->quit)
        break;

      while(!parent->write_queue->empty())
      {
        picked_job = parent->write_queue->front();
        parent->write_queue->pop();
        locker.unlock();
        sendto(parent->socket_fd, picked_job.msg, picked_job.msg_len, 0,
               (struct sockaddr*) picked_job.sock_addr, picked_job.socklen);
        locker.lock();
      }
    }
  }
}
