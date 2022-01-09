test: interface.o test.o hash-table.o md5.o
	g++ interface.o test.o hash-table.o md5.o -o test

interface.o: interface.cpp
	g++ -c interface.cpp

hash-table.o: hash-table.cpp
	g++ -c hash-table.cpp

md5.o: md5.cpp
	g++ -c md5.cpp

test.o: test.cpp
	g++ -c test.cpp