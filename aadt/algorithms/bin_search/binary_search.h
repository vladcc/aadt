/* binary_search.h -- binary search public interface */
#ifndef BIN_SEARCH_H
#define BIN_SEARCH_H

int binary_search(void * sorted_arr, void * traget, int arr_size, int elmt_size,
					int (*compar)(const void * k1, const void * k2));
/*
returns: index of target if found, -1 otherwise

description: Uses binary search to find traget in the sorted array specified by
sorted_arr. The number of elements in sorted_arr is specified in arr_size, the
size in bytes for each element is specified in elmt_size. compar() is user defined
and should return 1 if k1 > k2, -1 if k1 < k2, and 0 if k1 == k2.

complexity: O(log arr_size)
*/
#endif