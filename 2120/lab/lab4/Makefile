CC = g++
DEBUG = -g
CFLAGS = -Wall -std=gnu++11 $(DEBUG)
LFLAGS = -Wall $(DEBUG)

compile: lab04.out

lab04.out: lab04.cpp Calc.h
	$(CC) $(CFLAGS) -o lab04.out lab04.cpp

run: lab04.out
	./lab04.out

clean:
	rm *.out