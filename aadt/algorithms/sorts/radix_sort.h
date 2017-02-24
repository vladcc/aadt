/* radix_sort.h -- radix sort interface */
#ifndef RADIX_SORT_H
#define RADIX_SORT_H
	
int radix_sort(int * arr, int arr_size, int digit_pos, int radix);
/*
returns: 0 on success, -1 on failure

description: Uses radix sor to sort an array of integers. digit_pos specifies the number of
digit positions in each integer. radix specifies the radix. Upon return, arr contains
the sorted numbers.

complexity: O(digit_pos*arr_size + digit_pos*radix)
*/	
#endif