#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>

template <class T>
class TaskQueue {

 private:
  std::condition_variable _condition;
  std::mutex _mutex;
  std::queue<T> _queue; // очередь задач

 public:
  void push(T& task) {
    std::lock_guard<std::mutex> lock_guard(_mutex);
    _queue.push(std::move(task));
    _condition.notify_one();
  }

  void pop(T& task) {
    std::unique_lock<std::mutex> unique_lock(_mutex);
    if (_queue.empty())
      _condition.wait(unique_lock, [this] { return !_queue.empty(); });
    task = std::move(_queue.front());
    _queue.pop();
  }

  // неблокирующий метод получения элемента из очереди
  bool popFast(T& task) {
    std::lock_guard<std::mutex> lock_guard(_mutex);
    if (_queue.empty()) return false;
    task = std::move(_queue.front());
    _queue.pop();
    return true;
  }


};
