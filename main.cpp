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


using FuncType = void (*)(int, int);

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







template <class T>
class BlockedQueue {
 public:
  void push(T& item) {
    std::lock_guard<std::mutex> l(m_locker);
    // обычный потокобезопасный push
    m_task_queue.push(item);
    // делаем оповещение, чтобы поток, вызвавший
    // pop проснулся и забрал элемент из очереди
    m_notifier.notify_one();
  }
  // блокирующий метод получения элемента из очереди
  void pop(T& item) {
    std::unique_lock<std::mutex> l(m_locker);
    if (m_task_queue.empty())
      // ждем, пока вызовут push
      m_notifier.wait(l, [this] { return !m_task_queue.empty(); });
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
  // очередь задач
  std::queue<T> m_task_queue;
  // уведомитель
  std::condition_variable m_notifier;
};

class ThreadPool {
 public:
  ThreadPool()
      : m_thread_count(std::thread::hardware_concurrency() != 0
                           ? std::thread::hardware_concurrency()
                           : 4),
        m_thread_queues(m_thread_count){

        };
  // запуск:
  void start() {
    for (int i = 0; i < m_thread_count; i++) {
      m_threads.emplace_back(&ThreadPool::threadFunc, this, i);
    }
  };
  // остановка:
  void stop() {
    for (int i = 0; i < m_thread_count; i++) {
      // кладем задачу-пустышку в каждую очередь
      // для завершения потока
      std::function<void()> empty_task;
      m_thread_queues[i].push(empty_task);
    }
    for (auto& t : m_threads) {
      t.join();
    }
  };
  // проброс задач
  void push_task(FuncType f, int id, int arg) {
    // вычисляем индекс очереди, куда положим задачу
    int queue_to_push = m_index++ % m_thread_count;
    // формируем функтор
    std::function<void()> task = [=] { f(id, arg); };
    // кладем в очередь
    m_thread_queues[queue_to_push].push(task);
  }
  // функция входа для потока
  void threadFunc(int qindex) {
    while (true) {
      // обработка очередной задачи
      std::function<void()> task_to_do;
      bool res;
      int i = 0;
      for (; i < m_thread_count; i++) {
        // попытка быстро забрать задачу из любой очереди, начиная со своей
        if (res = m_thread_queues[(qindex + i) % m_thread_count].fast_pop(
                task_to_do))
          break;
      }

      if (!res) {
        // вызываем блокирующее получение очереди
        m_thread_queues[qindex].pop(task_to_do);
      } else if (!task_to_do) {
        // чтобы не допустить зависания потока
        // кладем обратно задачу-пустышку
        m_thread_queues[(qindex + i) % m_thread_count].push(task_to_do);
      }
      if (!task_to_do) {
        return;
      }
      // выполняем задачу
      task_to_do();
    }
  }

 private:
  // количество потоков
  int m_thread_count;
  // потоки
  std::vector<std::thread> m_threads;
  // очереди задач для потоков
  std::vector<BlockedQueue<std::function<void()>>> m_thread_queues;
  // для равномерного распределения задач
  int m_index;
};


class RequestHandler {
	public:
	RequestHandler() { threadPool.start(); }
	~RequestHandler() { threadPool.stop(); }
	// отправка запроса на выполнение
	void pushRequest(FuncType f, int id, int arg) {
	threadPool.push_task(f, id, arg);
	}

	private:
	// пул потоков
	ThreadPool threadPool;
};

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

  for (int i = 1; i <= 1000000; i++) {
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