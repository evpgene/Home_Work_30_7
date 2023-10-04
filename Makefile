.PHONY: all clean install uninstall

LINK = sudo g++ -std=c++20 -D_DEBUG -o
COMPILE = sudo g++ -std=c++20 -g -D_DEBUG -c -o 


all: main

BlockedQueue.o: BlockedQueue.cpp BlockedQueue.h
	$(COMPILE) BlockedQueue.o BlockedQueue.cpp

ThreadPool.o: ThreadPool.cpp ThreadPool.h
	$(COMPILE) ThreadPool.o ThreadPool.cpp

RequestHandler.o: RequestHandler.cpp RequestHandler.h
	$(COMPILE) RequestHandler.o RequestHandler.cpp

main.o: main.cpp
	$(COMPILE) main.o main.cpp

main: main.o RequestHandler.o ThreadPool.o BlockedQueue.o 
	$(LINK) main main.o RequestHandler.o ThreadPool.o BlockedQueue.o





