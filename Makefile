#MAKEFILE 
#KARAVANGELI EFTYCHIA - 1115201800062

CC = gcc
CFLAGS = -g -Wall

all: test

.PHONY: all clean

test: Simulation.o ReplacementAlgorithms.o PageTable.o Stack_Queue.o
	$(CC) $(CFLAGS) -o test Simulation.o ReplacementAlgorithms.o PageTable.o Stack_Queue.o

Simulation.o: Simulation.c
	$(CC) $(CFLAGS) -c Simulation.c  

ReplacementAlgorithms.o: ReplacementAlgorithms.c
	$(CC) $(CFLAGS) -c ReplacementAlgorithms.c

Stack_Queue.o: Stack_Queue.c
	$(CC) $(CFLAGS) -c Stack_Queue.c

PageTable.o: PageTable.c
	$(CC) $(CFLAGS) -c PageTable.c

clean:
	rm -f test Simulation.o PageTable.o Stack_Queue.o ReplacementAlgorithms.o 

