CC = g++
DEBUG = -g
CFLAGS = -Wall -std=gnu++11 $(DEBUG)
LFLAGS = -Wall $(DEBUG)

compile: lab05.out

lab05.out: lab05.cpp worldmap.h
	$(CC) $(CFLAGS) -o lab05.out lab05.cpp

run: lab05.out
	./lab05.out

clean:
	rm *.out