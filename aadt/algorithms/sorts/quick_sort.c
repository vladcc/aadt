/* quick_sort.c -- quicksort implementation */
#include <stdlib.h>
#include <string.h>
#include "quick_sort.h"
#include "insertion_sort.h"

static int compare_int(const void * int1, const void * int2)
{
	/* used for median-of-three partitioning */
	if (*(const int *)int1 > *(const int *)int2)
		return 1;
	else if (*(const int *)int1 < *(const int *)int2)
		return -1;
	else
		return 0;
}

static int partition(void * arr, int elmt_size, int i, int k, 
						int (*compar)(const void * k1, const void * k2))
{
	char * a = arr;
	void * pval, * temp;
	int r[3];
	
	// allocate storage for partition value and swapping
	if ((pval = malloc(elmt_size)) == NULL)
		return -1;
	
	if ((temp = malloc(elmt_size)) == NULL)
	{
		free(pval);
		return -1;
	}
	// median of three
	r[0] = (rand() % (k - i + 1)) + i;
	r[1] = (rand() % (k - i + 1)) + i;
	r[2] = (rand() % (k - i + 1)) + i;
	insertion_sort(r, 3, sizeof(r[0]), compare_int);
	memcpy(pval, &a[r[1] * elmt_size], elmt_size);
	
	// create two partitions around the partition value
	--i; ++k;
	while (1)
	{
		// move left until an element is found in the wrong place
		do 
		{
			--k;
		} while (compar(&a[k * elmt_size], pval) > 0);
		
		// similar for right
		do
		{
			++i;
		} while (compar(&a[i * elmt_size], pval) < 0);
	
		// stop partitioning when the counters cross
		if (i >= k)
			break;
		else
		{
			// swap the unordered elements
			memcpy(temp, &a[i * elmt_size], elmt_size);
			memcpy(&a[i * elmt_size], &a[k * elmt_size], elmt_size);
			memcpy(&a[k * elmt_size], temp, elmt_size);
		}
	}
	free(pval);
	free(temp);
	
	// return the point of division
	return k;
}

int quick_sort(void * arr, int arr_size, int elmt_size, int i, int k,
					int (*compar)(const void * k1, const void * k2))
{
	int j;
	// stop the recursion when we can't partition further
	while (i < k)
	{
		// determine where to partition
		if ((j = partition(arr, elmt_size, i, k, compar)) < 0)
			return -1;
		
		// recursively sort the left side
		if (quick_sort(arr, arr_size, elmt_size, i, j, compar) < 0)
			return -1;
		// iterate and sort for the right
		i = j + 1;
	}
	
	return 0;
}