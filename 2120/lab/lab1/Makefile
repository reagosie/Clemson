CC = g++
DEBUG = -g
CFLAGS = -Wall -std=gnu++11 $(DEBUG)
LFLAGS = -Wall $(DEBUG)

compile: lab01.out

lab01.out: lab01.cpp Ring.h
	$(CC) $(CFLAGS) -o lab01.out lab01.cpp

run: lab01.out
	./lab01.out

clean:
	rm *.out
