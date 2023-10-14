#pragma once

#include <future>
#include <iostream>

#include "ThreadPool.h"

using functionptr_t = void (*)(int, int); // тип указателя на функцию

class RequestHandler {
 private:
  ThreadPool _thread_pool;
  
 public:
  RequestHandler();
  ~RequestHandler();
  result_t pushTaskRequest(functionptr_t function, int ordinal_number,
                           int sleep_delay);  // отправка запроса на выполнение
  result_t pushTaskRequest(functionptr1_t function, int* array, long left,
                           long right,
                           std::shared_ptr<std::vector<result_t>>
                               results_ptr);  // отправка запроса на выполнение
};