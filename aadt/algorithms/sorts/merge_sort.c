/* merge_sort.c -- mergesort implementation */
#include <stdlib.h>
#include <string.h>
#include "merge_sort.h"

static int merge(void * arr, int elmt_size, int i, int j, int k,
					int (*compar)(const void * k1, const void * k2))
{
	char * a = arr, * m;
	int ipos, jpos, mpos;
	
	// initialize the counters for merging
	ipos = i;
	jpos = j + 1;
	mpos = 0;
	
	// allocate storage for merged elements
	if ((m = (char * )malloc(elmt_size * ((k - i) + 1))) == NULL)
		return -1;
	
	// continue while there are elements to merge
	while (ipos <= j || jpos <= k)
	{
		if (ipos > j)
		{
			// the left division has no more elements
			while (jpos <= k)
			{
				memcpy(&m[mpos * elmt_size], &a[jpos * elmt_size], elmt_size);
				++jpos;
				++mpos;
			}
			continue;
		}
		else if (jpos > k)
		{
			// the right division has no more elements
			while (ipos <= j)
			{
				memcpy(&m[mpos * elmt_size], &a[ipos * elmt_size], elmt_size);
				++ipos;
				++mpos;
			}
			continue;
		}
		// append the next ordered element to the merged list
		if (compar(&a[ipos * elmt_size], &a[jpos * elmt_size]) < 0)
		{
			memcpy(&m[mpos * elmt_size], &a[ipos * elmt_size], elmt_size);
			++ipos;
			++mpos;
		}
		else
		{
			memcpy(&m[mpos * elmt_size], &a[jpos * elmt_size], elmt_size);
			++jpos;
			++mpos;
		}
	}
	// prepare to pass back the merged data
	memcpy(&a[i * elmt_size], m, elmt_size * ((k - i) + 1));
	free(m);
	
	return 0;
}

int merge_sort(void * arr, int arr_size, int elmt_size, int i, int k,
					int (*compar)(const void * k1, const void * k2))
{
	int j;
	
	// stop the recursion when no more divisions can be made
	if (i < k)
	{
		// determine where to divide
		j = (int)(((i + k - 1)) / 2);
		
		// recursively sort the divisions
		if (merge_sort(arr, arr_size, elmt_size, i, j, compar) < 0)
			return -1;
		
		if (merge_sort(arr, arr_size, elmt_size, j + 1, k, compar) < 0)
			return -1;
		
		// merge the two sorted divisions into one
		if (merge(arr, elmt_size, i, j, k, compar) < 0)
			return -1;
	}

	return 0;
}