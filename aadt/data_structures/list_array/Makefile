CC=gcc
CFLAGS=-s -Wall
OBJ=test.o arrlist.o
MAIN=test.exe

test: $(OBJ)
	$(CC) $(OBJ) -o $(MAIN) $(CFLAGS)

test.o: test.c
	$(CC) test.c -c -o test.o $(CFLAGS)
	
arrlist.o: arrlist.c arrlist.h
	$(CC) arrlist.c -c -o arrlist.o $(CFLAGS)
	
clean:
	del $(OBJ)
	del $(MAIN)