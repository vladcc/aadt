CC=gcc
CFLAGS=-s -Wall -O2
OBJ=test.o binary_search.o
MAIN=test.exe

test: $(OBJ)
	$(CC) $(OBJ) -o $(MAIN) $(CFLAGS)

test.o: test.c
	$(CC) test.c -c -o test.o $(CFLAGS)

binary_search.o: binary_search.c binary_search.h
	$(CC) binary_search.c -c -o binary_search.o $(CFLAGS)	

clean:
	del $(OBJ)
	del $(MAIN)