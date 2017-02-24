/* bubble_sort.c -- bubble sort implementataion */
#include <stdlib.h>
#include <string.h>
#include "bubble_sort.h"

typedef enum bool{false, true} bool;

int bubble_sort(void * arr, int arr_size, int elmt_size, 
					int (*compar)(const void * k1, const void * k2))
{
	bool swap = true;
	char * a = (char *)arr;
	void * key;
	int i;
	
	// allocate storage for the key element
	if ((key = (char *)malloc(elmt_size)) == NULL)
		return -1;
	
	while (swap)
	{
		swap = false;
		for (i = 1; i < arr_size; ++i)
		{
			if (compar(&a[(i-1) * elmt_size], &a[i * elmt_size]) > 0)
			{
				// get the key
				memcpy(key, &a[i * elmt_size], elmt_size);

				// swap
				memcpy(&a[i * elmt_size], &a[(i-1) * elmt_size], elmt_size);
				memcpy(&a[(i-1) * elmt_size], key, elmt_size);
				swap = true;
			}
		}
	}
	
	return 0;
}