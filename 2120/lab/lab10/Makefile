CC = g++
DEBUG = -g
CFLAGS = -Wall -std=gnu++11 $(DEBUG)
LFLAGS = -Wall $(DEBUG)

default: lab10.cpp permute.h
	$(CC) $(CFLAGS) -o lab10.out lab10.cpp

run: lab10.out
	./lab10.out

clean:
	rm *.out
