CC = g++
DEBUG = -g
CFLAGS = -Wall -std=gnu++11 $(DEBUG)
LFLAGS = -Wall $(DEBUG)

compile: lab06.out

lab06.out: lab06.cpp nearest_neighbor.h point.h
	$(CC) $(CFLAGS) -o lab06.out lab06.cpp

run: lab06.out
	./lab06.out

clean:
	rm *.out
