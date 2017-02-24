CC=gcc
CFLAGS=-s -Wall
OBJ=test.o btree.o
MAIN=test.exe

test: $(OBJ)
	$(CC) $(OBJ) -o $(MAIN) $(CFLAGS)

test.o: test.c
	$(CC) test.c -c -o test.o $(CFLAGS)

btree.o: btree.c btree.h
	$(CC) btree.c -c -o btree.o $(CFLAGS)
	
clean:
	del $(OBJ)
	del $(MAIN)