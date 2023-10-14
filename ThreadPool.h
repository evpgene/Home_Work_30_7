#pragma once

#include <functional>
#include <thread>
#include <vector>
#include <future>

#include "TaskQueue.h"
#include "InterruptableThread.h"

using result_t = std::future<void>;
using functionptr_t = void (*)(int, int); // задаём трафарет функции
using functionptr1_t = void (*)(int*, long, long, std::shared_ptr<std::vector<result_t>>); // задаём трафарет функции
using task_t = std::packaged_task<void()>;

class InterruptableThread; // предварительное объявление класса

class ThreadPool {
 private:
  int _threads_number{0};  // количество потоков
  int _tasks_counter{0};  // счётчик всех добавленных в пулл задач
  std::vector<InterruptableThread*> _threads;  //указатели на потоки
  std::vector<TaskQueue<task_t>> _queues;  // очереди задач для каждого потока


 public:
  ThreadPool();
  ~ThreadPool();
  void start();
  void stop();
  result_t pushTask(functionptr_t function, int ordinal_number, int sleep_delay);
  result_t pushTask(functionptr1_t function, int* array, long left, long right, std::shared_ptr<std::vector<result_t>> results_ptr);

  void popExecTask(int thread_index);
  void interrupt();

};
