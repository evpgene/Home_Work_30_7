#pragma once

#include <functional>
#include <thread>
#include <vector>

#include "BlockedQueue.h"

using FuncType = void (*)(int, int);
using ThreadFunction = std::function<void()>;

class ThreadPool {
 public:
  ThreadPool();
  void start();
  void stop();
  void push_task(FuncType f, int id, int arg);
  void threadFunc(int qindex);

 private:
  int m_thread_count;                  // количество потоков
  std::vector<std::thread> m_threads;  // потоки
  std::vector<BlockedQueue<ThreadFunction>>
      m_thread_queues;  // очереди задач для потоков
  int m_index;  // для равномерного распределения задач
};
