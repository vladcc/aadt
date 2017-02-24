CC=gcc
CFLAGS=-s -Wall
OBJ=test.o clist.o
MAIN=test.exe

test: $(OBJ)
	$(CC) $(OBJ) -o $(MAIN) $(CFLAGS)

test.o: test.c
	$(CC) test.c -c -o test.o $(CFLAGS)
	
dlist.o: clist.c clist.h
	$(CC) clist.c -c -o clist.o $(CFLAGS)
	
clean:
	del $(OBJ)
	del $(MAIN)