store: implementation.o queue.o main.o
	g++ implementation.o queue.o main.o -o store

test: implementation.o queue.o tests.o
	g++ implementation.o queue.o tests.o -o test

implementation.o: implementation.cpp
	g++ -c implementation.cpp

queue.o: queue.cpp
	g++ -c queue.cpp

main.o: main.cpp
	g++ -c main.cpp

tests.o: tests.cpp
	g++ -c tests.cpp