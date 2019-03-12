CC = g++
DEBUG = -g
CFLAGS = -Wall -std=gnu++11 $(DEBUG)
LFLAGS = -Wall $(DEBUG)

default: mylab11.cpp search.h
	$(CC) $(CFLAGS) -o mylab11.out mylab11.cpp

run: mylab11.out
	./mylab11.out

clean:
	rm *.out
