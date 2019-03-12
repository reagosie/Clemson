CC = g++
DEBUG = -g
CFLAGS = -Wall -std=gnu++11 $(DEBUG)
LFLAGS = -Wall $(DEBUG)

compile: lab03.out

lab03.out: lab03.cpp
	$(CC) $(CFLAGS) -o lab03.out lab03.cpp

run: lab03.out
	./lab03.out

clean:
	rm *.out