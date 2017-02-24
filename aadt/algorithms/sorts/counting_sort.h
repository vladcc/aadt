/* counting_sort.h -- counting sort interface */

#ifndef COUNT_SORT_H
#define COUNT_SORT_H

int counting_sort(int * arr, int arr_size, int max_num);
/*
returns: 0 on success, -1 on failure

description: Uses counting sort to sort arr. arr_size specifies the size of the array.
max_num specifies the maximum integer in the array + 1. Upon return, the elements in arr
are sorted.

complexity: O(arr_size + max_num)
*/
#endif