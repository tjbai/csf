CC = g++
CFLAGS = -std=c++11 -Wall -Wextra -pedantic -g 

csim: main.o cache.o 
	$(CC) -o csim main.o cache.o 

main.o: main.cpp cache.h 
	$(CC) $(CFLAGS) -c main.cpp 

cache.o: cache.cpp cache.h 
	$(CC) $(CFLAGS) -c cache.cpp 

clean:
	rm -rf csim *.o