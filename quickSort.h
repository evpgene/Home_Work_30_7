#pragma once
#include <thread>
#include <memory>
#include <future>
#include <utility>
#include "ThreadPool.h"
#include "RequestHandler.h"



bool make_thread{true};

//ThreadPool pool;
RequestHandler request_handler;
using results_vector_t = std::vector<result_t>;
// results_vector_t results;
std::shared_ptr<results_vector_t> results_ptr = std::make_shared<results_vector_t>(results_vector_t());

void quickSort(int* array, long left, long right, std::shared_ptr<std::vector<result_t>> results_ptr) {
  if (left >= right) { 
    return;}

  long left_bound = left;
  long right_bound = right;
  long middle = array[(left_bound + right_bound) / 2];

  do {
    while (array[left_bound] < middle) {
      left_bound++;
    }
    while (array[right_bound] > middle) {
      right_bound--;
    }

    // Меняем элементы местами
    if (left_bound <= right_bound) {
      std::swap(array[left_bound], array[right_bound]);
      left_bound++;
      right_bound--;
    }
  } while (left_bound <= right_bound);

  if (make_thread && (right_bound - left >=  10000 )) {
    auto f = request_handler.pushTaskRequest(quickSort, array, left, right_bound, results_ptr);
    if(results_ptr){
    results_ptr->push_back(std::move(f));}
    quickSort(array, left_bound, right, std::move(results_ptr));
  } else {
    // запускаем обе части синхронно
    quickSort(array, left, right_bound, results_ptr);
    quickSort(array, left_bound, right, std::move(results_ptr));
    return;
  }
}
