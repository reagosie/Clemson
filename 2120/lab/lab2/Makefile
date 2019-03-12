CC = g++
DEBUG = -g
CFLAGS = -Wall -std=gnu++11 $(DEBUG)
LFLAGS = -Wall $(DEBUG)

compile: lab02.out

lab02.out: lab02.cpp Node.h List.h
	$(CC) $(CFLAGS) -o lab02.out lab02.cpp

run: lab02.out
	./lab02.out

clean:
	rm *.out


