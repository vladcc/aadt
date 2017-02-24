#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "insertion_sort.h"
#include "quick_sort.h"
#include "merge_sort.h"
#include "counting_sort.h"
#include "radix_sort.h"
#include "bubble_sort.h"

#define NUM_ALGS	7
#define PRINT		5

void print_arr(int * arr, int size);
int compar(const void * k1, const void * k2);

int main(int argc, char * argv[])
{
	int * arr;
	int arr_size;
	clock_t begin, end;
	double time_spent;
	int i, j, k, items;
	
	if (argc < 2)
	{
		puts("test [number of items to sort as signed int > 0]");
		return -1;
	}
		
	sscanf(argv[1], "%d", &items);
	arr_size = items;
	
	if (items > 20000)
		i = 2;
	else
		i = 0;
	
	for (; i < NUM_ALGS; ++i)
	{
		
		
		arr = malloc(items * sizeof(*arr));
		if (arr == NULL)
		{
			puts("Err: Memory allocation failed");
			return -1;
		}
		
		for (k = 0, j = items-1; k < items; --j, ++k)
			arr[k] = j;
		
		print_arr(arr, PRINT);
		begin = clock();
		switch (i)
		{
			case 0: 
				puts("Bubble sort");
				bubble_sort(arr, arr_size, sizeof(*arr), compar); 
				break;
			case 1:
				puts("Insertion sort");
				insertion_sort(arr, arr_size, sizeof(arr[0]), compar);
				break;
			case 2:
				puts("qsort() from stdlib");
				qsort(arr, arr_size, sizeof(arr[0]), compar);
				break;
			case 3:
				puts("Quick sort");
				quick_sort(arr, arr_size, sizeof(arr[0]), 0, arr_size - 1, compar);
				break;
			case 4:
				puts("Merge sort");
				merge_sort(arr, arr_size, sizeof(arr[0]), 0, arr_size - 1, compar);
				break;
			case 5:
				puts("Radix sort");
				radix_sort(arr, arr_size, 9, 10);
				break;
			case 6:
				puts("Counting sort");
				counting_sort(arr, arr_size, items+1);
				break;
			default: 
				break;
		}
		end = clock();
		print_arr(arr, PRINT);
		time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
		printf("Time: %.4f seconds\n", time_spent);
		free(arr);
	}
	
	return 0;
}

void print_arr(int * arr, int size)
{
	int i;
	for (i = 0; i < size; ++i)
		printf("%d ", arr[i]);
	
	putchar('\n');
	return;
}

int compar(const void * k1, const void * k2)
{
	if (*((int *)k1) > *((int *)k2))
		return 1;
	else if (*((int *)k1) < *((int *)k2))
		return -1;
	else
		return 0;
}