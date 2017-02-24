/* insertion_sort.c -- insertion sort implementation */

#include <stdlib.h>
#include <string.h>
#include "insertion_sort.h"

int insertion_sort(void * arr, int arr_size, int elmt_size, 
					int (*compar)(const void * k1, const void * k2))
{
	char * a = arr;
	void * key;
	int i, j;
	
	// allocate storage for the key element
	if ((key = (char *)malloc(elmt_size)) == NULL)
		return -1;
	
	// repeatedly insert a key element among the sorted elements
	for (j = 1; j < arr_size; ++j)
	{
		// get the key
		memcpy(key, &a[j * elmt_size], elmt_size);
		
		i = j - 1;
		while (i >= 0 && compar(&a[i * elmt_size], key) > 0)
		{
			// move elements bigger than the key one to the right
			memcpy(&a[(i + 1) * elmt_size], &a[i * elmt_size], elmt_size);
			--i;
		}
		
		// insert element
		memcpy(&a[(i + 1) * elmt_size], key, elmt_size);
	}
	free(key);
	
	return 0;
}