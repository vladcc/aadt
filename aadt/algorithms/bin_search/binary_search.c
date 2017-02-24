/* binary_search.c -- binary search implementation */
#include <stdlib.h>
#include <string.h>
#include "binary_search.h"

int binary_search(void * sorted_arr, void * target, int arr_size, int elmt_size,
					int (*compar)(const void * k1, const void * k2))
{
	int left, right, middle;
	
	// search until left and right cross
	left = 0;
	right = arr_size - 1;
	
	while (left <= right)
	{
		middle = (left + right) / 2;
		
		switch(compar( ((char *)sorted_arr + (elmt_size * middle)), target))
		{
			case 1:
				// search on the left of middle
				right = middle - 1;
				break;
			case -1:
				// search on the right of middle
				left = middle + 1;
				break;
			default:
				// found it
				return middle;
				break;
		}
	}
	
	// not found
	return -1;
}