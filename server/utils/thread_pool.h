#ifndef __THREAD_POOL_H_
#define __THREAD_POOL_H_

#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <queue>

namespace Utils
{
  using JobFunc = std::function<void(void*)>;
  struct Job
  {
    Job() : func(nullptr), params(nullptr) {};
    Job(JobFunc func, void* params) : func(func), params(params) {};
    JobFunc func;
    void* params;
  };

  class ThreadPool
  {
    public:
      ThreadPool(int threads_count);
      ~ThreadPool();

      void add_job(Job job);

      static void worker_loop(void* params);

      bool exit = false;

      std::queue<Job> *job_queue;

      std::mutex lock_queue;
      std::condition_variable cv_queue;

    private:
      int threads_count;
      std::vector<std::thread*> *threads;
  };
}

#endif
