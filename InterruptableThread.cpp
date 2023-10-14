#include "InterruptableThread.h"
#include "ThreadPool.h"

bool thread_local interrupt_thread {false};

InterruptableThread::InterruptableThread(ThreadPool* thread_pool, int thread_index)
    : _thread(&InterruptableThread::start, this, thread_pool, thread_index) {}

InterruptableThread::~InterruptableThread() {
  if (_thread.joinable()) _thread.join();
}

void InterruptableThread::start(ThreadPool* thread_pool, int thread_index) {
  if (thread_pool == nullptr) return;

  {
    std::lock_guard<std::mutex> lg(_mutex);
    _interrupt_thread = &interrupt_thread;
  }
  
  thread_pool->popExecTask(thread_index);

  {
    std::lock_guard<std::mutex> lg(_mutex);
    _interrupt_thread = nullptr;
  }
}

void InterruptableThread::interrupt() {
  std::lock_guard<std::mutex> lg(_mutex);
  if (_interrupt_thread) *_interrupt_thread = true;
}

bool InterruptableThread::checkInterrupted() { return interrupt_thread; }