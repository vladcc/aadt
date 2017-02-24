/* counting_sort.c -- counting sort implementation */
#include <stdlib.h>
#include <string.h>
#include "counting_sort.h"

int counting_sort(int * arr, int arr_size, int max_num)
{
	int * counts, * temp;
	int i, j;
	
	// allocate storage for counts
	if ((counts = (int *)malloc(max_num * sizeof(*counts))) == NULL)
		return -1;
	
	// alloca storage for the stored elements
	if ((temp = (int *)malloc(arr_size * sizeof(*temp))) == NULL)
	{
		free(counts);
		return -1;
	}
		
	// initialize the counts
	for (i = 0; i < max_num; ++i)
		counts[i] = 0;
	
	// count the occurrences of each element
	for (j = 0; j < arr_size; ++j)
		++counts[arr[j]];
	
	// adjust each count to reflect the counts before it
	for (i = 1; i < max_num; ++i)
		counts[i] += counts[i - 1];
		
	// use the counts to position each element where it belongs
	for (j = arr_size - 1; j >= 0; --j)
	{
		temp[counts[arr[j]] - 1] = arr[j];
		counts[arr[j]] = counts[arr[j]] - 1;
	}

	// prepare to bass back the stored data
	memcpy(arr, temp, arr_size * sizeof(arr[0]));

	// free sorting storage
	free(counts);
	free(temp);
	
	return 0;
}