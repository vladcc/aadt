/* radix_sort.c -- radix sort implementation */
#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "radix_sort.h"

int radix_sort(int * arr, int arr_size, int digit_pos, int radix)
{
	int * counts, * temp;
	int index, pval, i, j, n;
	
	// allocate storage
	if ((counts = (int *)malloc(radix * sizeof(*counts))) == NULL)
		return -1;
	
	if ((temp = (int *)malloc(arr_size * sizeof(*temp))) == NULL)
	{
		free(counts);
		return -1;
	}
		
	
	// sort from the least significant poisition to the most significant
	for (n = 0; n < digit_pos; ++n)
	{
		// initialize the counts
		for (i = 0; i < radix; ++i)
			counts[i] = 0;
		
		pval = (int)pow((double)radix, (double)n);
		// count the occurrences of each digit value
		for (j = 0; j < arr_size; ++j)
		{
			index = (int)(arr[j] / pval) % radix;
			++counts[index];
		}
	
		// adjust each count to reflect the counts before it
		for (i = 1; i < radix; ++i)
			counts[i] += counts[i-1];
		
		// use the counts to position each element where it belongs
		for (j = arr_size - 1; j >= 0; --j)
		{
			index = (int)(arr[j] / pval) % radix;
			temp[counts[index] - 1] = arr[j];
			--counts[index];
		}
		
		// copy sorted integers
		memcpy(arr, temp, arr_size * sizeof(arr[0]));
	}
	
	// free memory
	free(counts);
	free(temp);
	
	return 0;
}