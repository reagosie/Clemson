CC = g++
DEBUG = -g
CFLAGS = -Wall -std=gnu++11 $(DEBUG)
LFLAGS = -Wall $(DEBUG)

compile: lab09.out

lab09.out: lab09.cpp graphalgs.h
	$(CC) $(CFLAGS) -o lab09.out lab09.cpp

run: lab09.out
	./lab09.out

clean:
	rm *.out