// #pragma once
#include <condition_variable>
#include <mutex>
#include <queue>

template <class T>
class BlockedQueue {
 public:
  // void push(T& item);
  // void pop(T& item);
  // bool fast_pop(T& item);

  void push(T& item) {
  std::lock_guard<std::mutex> l(m_locker);
  m_task_queue.push(item);  // обычный потокобезопасный push
  m_notifier.notify_one();  // делаем оповещение, чтобы поток, вызвавший pop
                            // проснулся и забрал элемент из очереди
}


void pop(T& item) {
  std::unique_lock<std::mutex> l(m_locker);
  if (m_task_queue.empty())

    m_notifier.wait(l, [this] {
      return !m_task_queue.empty();
    });  // ждем, пока вызовут push
  item = m_task_queue.front();
  m_task_queue.pop();
}

// неблокирующий метод получения элемента из очереди
// возвращает false, если очередь пуста
bool fast_pop(T& item) {
  std::lock_guard<std::mutex> l(m_locker);
  if (m_task_queue.empty())
    // просто выходим
    return false;
  // забираем элемент
  item = m_task_queue.front();
  m_task_queue.pop();
  return true;
}

 private:
  std::mutex m_locker;
  std::queue<T> m_task_queue;          // очередь задач
  std::condition_variable m_notifier;  // уведомитель
};
