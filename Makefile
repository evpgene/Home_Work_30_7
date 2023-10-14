.PHONY: all clean install uninstall

LINK = sudo g++ -std=c++20 -D_DEBUG -o
COMPILE = sudo g++ -std=c++20 -g -D_DEBUG -c -o 


all: main

TaskQueue.o: TaskQueue.cpp TaskQueue.h
	$(COMPILE) TaskQueue.o TaskQueue.cpp

RequestHandler.o: RequestHandler.cpp RequestHandler.h
	$(COMPILE) RequestHandler.o RequestHandler.cpp

ThreadPool.o : ThreadPool.cpp ThreadPool.h
	$(COMPILE) ThreadPool.o ThreadPool.cpp

InterruptableThread.o: InterruptableThread.cpp InterruptableThread.h
	$(COMPILE) InterruptableThread.o InterruptableThread.cpp

quickSort.o: quickSort.cpp quickSort.h
	$(COMPILE) quickSort.o quickSort.cpp

main.o: main.cpp
	$(COMPILE) main.o main.cpp

main: main.o ThreadPool.o InterruptableThread.o RequestHandler.o TaskQueue.o quickSort.o Makefile 
	$(LINK) main main.o ThreadPool.o InterruptableThread.o RequestHandler.o TaskQueue.o 
	
clean:
	rm -f *.o







