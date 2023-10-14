#include "RequestHandler.h"

RequestHandler::RequestHandler() { _thread_pool.start(); }
RequestHandler::~RequestHandler() { _thread_pool.stop(); }
result_t RequestHandler::pushTaskRequest(functionptr_t function, int ordinal_number, int sleep_delay) {
   return _thread_pool.pushTask(function, ordinal_number, sleep_delay);
}

result_t RequestHandler::pushTaskRequest(functionptr1_t function, int* array, long left, long right, std::shared_ptr<std::vector<result_t>> results_ptr) {
  return result_t( _thread_pool.pushTask(function, array, left, right, std::move(results_ptr)));
}
