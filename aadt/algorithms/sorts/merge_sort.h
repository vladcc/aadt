/* merge_sort.h -- mergesort interface */

#ifndef MERGE_SORT_H
#define MERGE_SORT_H

int merge_sort(void * arr, int arr_size, int elmt_size, int i, int k,
					int (*compar)(const void * k1, const void * k2));
/*
returns: 0 on success, -1 otherwise

description: Uses mergesort to sort the arr pointed to by arr. The number of elements
in the array is specified by arr_size. The size of each element in bytes is specified
by elmt_size. The arguments i and k define the current division being sorted and
initially should be 0 and arr_size-1, respectively. compar() is a user defined function returning 1 if k1 > k2, 0 if k1 == k2, 
and -1 if k1 < k2 for ascending sort. Upon return, arr contains the sorted elements.

complexity: O(n log n)
*/
#endif