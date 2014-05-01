#include "thread_pool.h"
#include "../../shared/debug.h"

#include <iostream>

namespace Utils
{
  ThreadPool::ThreadPool(int threads_count) : threads_count(threads_count)
  {
    threads = new std::vector<std::thread*>;
    job_queue = new std::queue<Job>;
    for(int i = 0; i < threads_count; i++)
      threads->push_back(new std::thread(worker_loop, this));
  }

  ThreadPool::~ThreadPool()
  {
    exit = true;
    cv_queue.notify_all();
    for(auto thread : *threads)
    {
      thread->join();
      delete thread;
    }
  }

  void ThreadPool::add_job(Job job)
  {
    std::unique_lock<std::mutex> locker(lock_queue);
    job_queue->push(job);
    cv_queue.notify_one();
  }

  void ThreadPool::worker_loop(void* params)
  {
    ThreadPool *parent = (ThreadPool*) params;
    while(!parent->exit)
    {
      Job picked_job;
      std::unique_lock<std::mutex> locker(parent->lock_queue);

      parent->cv_queue.wait(locker);

      if (parent->exit)
        break;

      while(!parent->job_queue->empty())
      {
        picked_job = parent->job_queue->front();
        parent->job_queue->pop();
        locker.unlock();
        if (picked_job.func)
          picked_job.func(picked_job.params);
        locker.lock();
      }
    }
  }
}
