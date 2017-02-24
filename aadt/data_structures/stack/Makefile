CC=gcc
CFLAGS=-s -Wall
OBJ=test.o stack.o
MAIN=test.exe

test: $(OBJ)
	$(CC) $(OBJ) -o $(MAIN) $(CFLAGS)

test.o: test.c
	$(CC) test.c -c -o test.o $(CFLAGS)
	
stack.o: stack.c stack.h
	$(CC) stack.c -c -o stack.o $(CFLAGS)
	
clean:
	del $(OBJ)
	del $(MAIN)