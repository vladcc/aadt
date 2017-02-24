CC=gcc
CFLAGS=-s -Wall
OBJ=test.o list.o chtbl.o
MAIN=test.exe

test: $(OBJ)
	$(CC) $(OBJ) -o $(MAIN) $(CFLAGS)

test.o: test.c
	$(CC) test.c -c -o test.o $(CFLAGS)

chtbl.o: chtbl.c chtbl.h
	$(CC) chtbl.c -c -o chtbl.o $(CFLAGS)
	
list.o: list.c list.h
	$(CC) list.c -c -o list.o $(CFLAGS)
	
clean:
	del $(OBJ)
	del $(MAIN)