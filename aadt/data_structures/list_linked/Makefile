CC=gcc
CFLAGS=-s -Wall
OBJ=test.o list.o
MAIN=test.exe

test: $(OBJ)
	$(CC) $(OBJ) -o $(MAIN) $(CFLAGS)

test.o: test.c
	$(CC) test.c -c -o test.o $(CFLAGS)
	
list.o: list.c list.h
	$(CC) list.c -c -o list.o $(CFLAGS)
	
clean:
	del $(OBJ)
	del $(MAIN)