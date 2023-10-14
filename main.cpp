#include <thread>
#include <chrono>
#include <utility>

#include "InterruptableThread.h"
#include "quickSort.h"

int main()
{

  long arr_size = 1000000;
  int* array = new int[arr_size];
  for (long i = 0; i < arr_size; i++) {
    array[i] = rand() % 500000;
  }
  const auto start = std::chrono::high_resolution_clock::now();
  

  quickSort(array, 0, arr_size - 1, results_ptr);

  while (results_ptr) {
    if (results_ptr.use_count() <= 1) {
      break;}
     else {
      std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
  }

if(results_ptr){
     for(auto& result: *results_ptr) {
       if( result.valid())
        result.wait();
    } ;
}

  bool sorted {true};
  for (long i = 0; i < arr_size - 1; i++) {
    if (array[i] > array[i + 1]) {
      std::cout << "Unsorted" << std::endl;
      sorted = false;
      break;
    }
  }

  const auto end = std::chrono::high_resolution_clock::now();

  const std::chrono::duration<double> diff = end - start;

  if (sorted) {
    std::cout << "SORTED" << std::endl;
  }

  std::cout << " Time for sorting with threads: " << diff.count() << std::endl;
  std::cout << std::endl;



make_thread = false;





 for (long i = 0; i < arr_size; i++) {
    array[i] = rand() % 500000;
  }
  const auto start2 = std::chrono::high_resolution_clock::now();
  

  quickSort(array, 0, arr_size - 1, results_ptr);

  while (results_ptr) {
    if (results_ptr.use_count() <= 1) {
      break;}
     else {
      std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
  }

if(results_ptr){
     for(auto& result: *results_ptr) {
       if( result.valid())
        result.wait();
    } ;
}

  sorted = true;
  for (long i = 0; i < arr_size - 1; i++) {
    if (array[i] > array[i + 1]) {
      std::cout << "Unsorted" << std::endl;
      sorted = false;
      break;
    }
  }

  const auto end2 = std::chrono::high_resolution_clock::now();

  const std::chrono::duration<double> diff2 = end2 - start2;

  if (sorted) {
    std::cout << "SORTED" << std::endl;
  }

  std::cout << " Time for sorting in one thread: " << diff2.count() << std::endl;
  std::cout << std::endl;






  delete array;
  return 0;
}