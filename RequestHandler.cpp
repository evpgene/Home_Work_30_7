#include "RequestHandler.h"

RequestHandler::RequestHandler() { threadPool.start(); }
RequestHandler::~RequestHandler() { threadPool.stop(); }
void RequestHandler::pushRequest(FuncType f, int id, int arg) {
  threadPool.push_task(f, id, arg);
}  // отправка запроса на выполнение
