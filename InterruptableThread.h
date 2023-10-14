#pragma once

#include <mutex>
#include <thread>

class ThreadPool;

class InterruptableThread {
 public:
  std::thread _thread;

 private:
  std::mutex _mutex;
  bool* _interrupt_thread {nullptr};

 public:
  InterruptableThread() = delete;
  InterruptableThread(ThreadPool*, int thread_index);
  ~InterruptableThread();
  void start(ThreadPool*, int thread_index);
  void interrupt();
  static bool checkInterrupted();
};
