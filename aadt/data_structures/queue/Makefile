CC=gcc
CFLAGS=-s -Wall
OBJ=test.o queue.o
MAIN=test.exe

test: $(OBJ)
	$(CC) $(OBJ) -o $(MAIN) $(CFLAGS)

test.o: test.c
	$(CC) test.c -c -o test.o $(CFLAGS)
	
queue.o: queue.c queue.h
	$(CC) queue.c -c -o queue.o $(CFLAGS)
	
clean:
	del $(OBJ)
	del $(MAIN)