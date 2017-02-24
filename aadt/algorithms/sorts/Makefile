CC=gcc
CFLAGS=-s -Wall -O2
OBJ=test.o insertion_sort.o quick_sort.o merge_sort.o counting_sort.o radix_sort.o bubble_sort.o
MAIN=test.exe

test: $(OBJ)
	$(CC) $(OBJ) -o $(MAIN) $(CFLAGS)

test.o: test.c
	$(CC) test.c -c -o test.o $(CFLAGS)

bubble_sort.o: bubble_sort.c bubble_sort.h
	$(CC) bubble_sort.c -c -o bubble_sort.o $(CFLAGS)	
	
radix_sort.o: radix_sort.c radix_sort.h
	$(CC) radix_sort.c -c -o radix_sort.o $(CFLAGS)	
	
counting_sort.o: counting_sort.c counting_sort.h
	$(CC) counting_sort.c -c -o counting_sort.o $(CFLAGS)
	
merge_sort.o: merge_sort.c merge_sort.h
	$(CC) merge_sort.c -c -o merge_sort.o $(CFLAGS)
	
quick_sort.o: quick_sort.c quick_sort.h
	$(CC) quick_sort.c -c -o quick_sort.o $(CFLAGS)
	
insertion_sort.o: insertion_sort.c insertion_sort.h
	$(CC) insertion_sort.c -c -o insertion_sort.o $(CFLAGS)

clean:
	del $(OBJ)
	del $(MAIN)