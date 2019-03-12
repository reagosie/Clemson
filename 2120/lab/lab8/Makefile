CC = g++
DEBUG = -g
CFLAGS = -Wall -std=gnu++11 $(DEBUG)
LFLAGS = -Wall $(DEBUG)

compile: lab08.out

lab08.out: lab08.cpp optimizers.h
	$(CC) $(CFLAGS) -o lab08.out lab08.cpp

run: lab08.out
	./lab08.out

clean:
	rm *.out