/* bubble_sort.h -- bubble sort public interface */
#ifndef BUBBLE_H
#define BUBBLE_H

int bubble_sort(void * arr, int arr_size, int elmt_size, 
					int (*compar)(const void * k1, const void * k2));
/*
returns: 0 on success, -1 otherwise

description: Uses bubble sort to sort the array of elements in arr. The number of
elements is specified by arr_size. The size of each element in bytes is specified by 
elmt_size. compar() is a user defined function returning 1 if k1 > k2, 0 if k1 == k2, 
and -1 if k1 < k2 for ascending sort. Upon return arr contains the sorted elements.

complexity: O(n*n)
*/
#endif