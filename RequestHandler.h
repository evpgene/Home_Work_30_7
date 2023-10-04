#include "ThreadPool.h"

using FuncType = void (*)(int, int);

class RequestHandler {
	public:
	RequestHandler();
	~RequestHandler();
	void pushRequest(FuncType f, int id, int arg); // отправка запроса на выполнение

	private:
	// пул потоков
	ThreadPool threadPool;
};