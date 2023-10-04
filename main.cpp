#include <chrono>
#include <future>
#include <iostream>
#include <random>


#include <condition_variable>
// #include <future>
// #include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>



// using namespace std;


//using FuncType = void (*)(int, int);

std::mutex coutLocker;


bool make_thread = true;

void quicksort(int* array, long left, long right) {
  if (left >= right) return;
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

  if (make_thread && (right_bound - left > 10000)) {
    // если элементов в левой части больше чем 10000
    // вызываем асинхронно рекурсию для правой части
    auto f =
        std::async(std::launch::async, [&]() { quicksort(array, left, right_bound); });
    quicksort(array, left_bound, right);
  } else {
    // запускаем обе части синхронно
    quicksort(array, left, right_bound);
    quicksort(array, left_bound, right);
  }
}










// функция, выполняющая задачу
void taskFunc(int id, int delay){
	// имитируем время выполнения задачи
	std::this_thread::sleep_for(std::chrono::seconds(delay));
	// выводим информацию о завершении

	std::unique_lock<std::mutex> lock(coutLocker);
	std::cout << "task " << id << " made by thread_id " << std::this_thread::get_id() << std::endl;
}




/* int main() {
  RequestHandler rh;

  for (int i = 1; i <= 1'000'000; i++) {
    srand((int)time(NULL));
    rh.pushRequest(taskFunc, i, 1 + rand() % 3);
  }
  return 0;
} */









int main() {
  srand(0);
  long arr_size = 100000000;
  int* array = new int[arr_size];
  for (long i = 0; i < arr_size; i++) {
    array[i] = rand() % 500000;
  }

  time_t start, end;

  // многопоточный запуск
  time(&start);
  quicksort(array, 0, arr_size);
  time(&end);

  double seconds = difftime(end, start);
  printf("The time: %f seconds\n", seconds);

  for (long i = 0; i < arr_size - 1; i++) {
    if (array[i] > array[i + 1]) {
      std::cout << "Unsorted" << std::endl;
      break;
    }
  }

  for (long i = 0; i < arr_size; i++) {
    array[i] = rand() % 500000;
  }
  // однопоточный запуск
  make_thread = false;
  time(&start);
  quicksort(array, 0, arr_size);
  time(&end);
  seconds = difftime(end, start);
  printf("The time: %f seconds\n", seconds);
  delete[] array;
  return 0;
}