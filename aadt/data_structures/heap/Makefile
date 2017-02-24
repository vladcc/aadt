CC=gcc
CFLAGS=-s -Wall
OBJ=test.o heap.o
MAIN=test.exe

test: $(OBJ)
	$(CC) $(OBJ) -o $(MAIN) $(CFLAGS)

test.o: test.c
	$(CC) test.c -c -o test.o $(CFLAGS)

heap.o: heap.c heap.h
	$(CC) heap.c -c -o heap.o $(CFLAGS)
	
clean:
	del $(OBJ)
	del $(MAIN)