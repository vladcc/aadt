CC=gcc
CFLAGS=-s -Wall
OBJ=test.o dlist.o
MAIN=test.exe

test: $(OBJ)
	$(CC) $(OBJ) -o $(MAIN) $(CFLAGS)

test.o: test.c
	$(CC) test.c -c -o test.o $(CFLAGS)
	
dlist.o: dlist.c dlist.h
	$(CC) dlist.c -c -o dlist.o $(CFLAGS)
	
clean:
	del $(OBJ)
	del $(MAIN)