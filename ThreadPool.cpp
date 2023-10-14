
#include "ThreadPool.h"

#include <iostream>
#include <thread>

ThreadPool::ThreadPool()
    : _threads_number(std::thread::hardware_concurrency() != 0
                         ? std::thread::hardware_concurrency() - 1
                         : 4),
      _queues(_threads_number) {}

ThreadPool::~ThreadPool() {
  interrupt();
  for (auto& thread : _threads) {
    delete thread;
  }
};

void ThreadPool::start() {
  for (int i = 0; i < _threads_number; i++) {
    _threads.push_back(new InterruptableThread(this, i));
  }
};

void ThreadPool::stop() {
  static task_t empty_task;
  for (auto& quiue:_queues) {
    // кладем задачу-пустышку в каждую очередь для завершения потока
    empty_task = task_t();
    quiue.push(empty_task);

  }
  for (auto& thread : _threads) {
    if(thread->_thread.joinable())
      thread->_thread.join();
  }
};

result_t ThreadPool::pushTask(functionptr_t function, int ordinal_number, int sleep_delay) {
  int index = (_tasks_counter % _threads_number);
  _tasks_counter++;
  task_t task([=] {(*function)(ordinal_number, sleep_delay); }); // в теле лямбды вызываем функцию по указателю и передаём её параметры
  auto result = task.get_future();
  _queues[index].push(task);
  return result;
}

result_t ThreadPool::pushTask( functionptr1_t function, int* array, long left, long right, std::shared_ptr<std::vector<result_t>> results_ptr) {
  int index = (_tasks_counter % _threads_number);
  _tasks_counter++;

  std::promise<void> p;
  auto f = p.get_future();
  task_t task([=, p = std::move(p)]() mutable {
    (*function)(array, left, right, std::move(results_ptr));
    p.set_value();
  });
  _queues[index].push(task);
  return f;
}

// the given thread is retrieved and executes the task
void ThreadPool::popExecTask(int thread_index) {
  task_t task;
  bool pop_fast_success{false};
  while (true) {
    if (InterruptableThread::checkInterrupted()) {
      std::cout << "Thread : " << thread_index << " was interrupted"
                << std::endl;
      return;
    }
    pop_fast_success = false;
    int i = 0;
    for (; i < _threads_number; i++) {
      // тут используется побочный эффект, задача забирается в task
      if (pop_fast_success = _queues[(thread_index + i) % _threads_number].popFast(
              task))
        break;
    }
    if (!pop_fast_success) {
      _queues[thread_index].pop(task);
    }
    task();  // выполняем задачу
  }
}

void ThreadPool::interrupt() {
  for (auto& thread : _threads) {
    thread->interrupt();
  }
  for (auto& thread : _threads) {
    if (thread->_thread.joinable()) thread->_thread.join();
  }
}