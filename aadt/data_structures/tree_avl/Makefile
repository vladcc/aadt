CC=gcc
CFLAGS=-s -Wall
OBJ=test.o btree.o bstree.o
MAIN=test.exe

test: $(OBJ)
	$(CC) $(OBJ) -o $(MAIN) $(CFLAGS)

test.o: test.c
	$(CC) test.c -c -o test.o $(CFLAGS)

bstree.o: bstree.c bstree.h
	$(CC) bstree.c -c -o bstree.o $(CFLAGS)

btree.o: btree.c btree.h
	$(CC) btree.c -c -o btree.o $(FLAGS)
	
clean:
	del $(OBJ)
	del $(MAIN)