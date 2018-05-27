/*
    Dynamic array
    Allocates memory on the heap. Grows automatically. Copies the data inside itself.
    Memory operations are implemented with memcpy() and memmove().

    All functions return an int, even boolean ones so they can return error codes as well.
    Pointers to requested data are returned as out arguments. All error codes are < 0.

    All pointers to the array are valid as long as it doesn't grow or shrink.

    If DYN_ARR_NO_CHECKS is defined on compile time, function arguments are
    NOT checked at runtime and valid return codes are only 0, DYN_ARR_MEM_ERR, and
    DYN_ARR_NOT_FOUND.

    0 is returned on function success in non-boolean functions, or as boolean false.

    1 is returned as boolean true.

    DYN_ARR_NOT_FOUND is returned whenever the item you are looking for is not found.

    DYN_ARR_NULL_PTR is returned whenever a pointer passed to a dyn_arr function, or
    contained in the array structure, expected to NOT be NULL is NULL.
    e.g. a * darr is NULL, or compar is NULL when trying to sort or search.

    DYN_ARR_BAD_INPUT is returned whenever a passed value is outside of its valid range.
    e.g. creating an array with element_size < 1.

    DYN_ARR_BAD_ARRAY is returned whenever the state of the passed array is invalid
    for the requested action.
    e.g. passing a destroyed array, or trying to insert online in an unsorted one.

    DYN_ARR_MEM_ERR is returned whenever something goes wrong with memory allocation.

    DYN_ARR_INDEX_ERR is returned whenever an index is out of bounds, or otherwise invalid.
    e.g. trying to read pass the end, or from an empty array.

    If the array is sorted, searching is optimized by using binary search and an online insert
    is possible, keeping the array sorted as elements are added.
    Some functions mark the array as NOT sorted, even though it may be in sorted order.
    For example, dyn_arr_push_back() appends to the array and marks it NOT sorted, no matter if
    the added element is in its proper place or not. This is done in order to keep the operation
    general and cheap, as the user may not care about the sortedness of the array, or provide a
    function for comparing. If you use any of the functions that mark the array as NOT sorted,
    you have to use dyn_arr_sort() if you want binary search to be used, or if you want to use
    dyn_arr_insert_online(). In order to avoid sorting an already sorted array dyn_arr_sort()
    first checks if the array is sorted, which takes O(n) time.

    Functions that mark the array as sorted:
    dyn_arr_create()
    dyn_arr_reset()
    dyn_arr_sort()

    Functions that mark the array as NOT sorted:
    dyn_arr_data()
    dyn_arr_find_from()
    dyn_arr_apply_to()
    dyn_arr_append_array()
    dyn_arr_push_back()
    dyn_arr_write_at_index()
    dyn_arr_insert_at_index()

    Functions that access the array, but do not change the sorted state:
    dyn_arr_const_data()
    dyn_arr_const_find_from()
    dyn_arr_const_apply_to()
*/
/* v1.1 */

/* Author: Vladimir Dinev */

#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

//#define DYN_ARR_NO_CHECKS

#define DYN_ARR_DEFAULT_CAPACITY    16
#define DYN_ARR_APPLY_ALL           -1
#define DYN_ARR_START               0
#define DYN_ARR_GROWTH_RATE         2

enum {
    DYN_ARR_NOT_FOUND   = -7,
    DYN_ARR_NULL_PTR    = -6,
    DYN_ARR_BAD_INPUT   = -5,
    DYN_ARR_BAD_ARRAY   = -4,
    DYN_ARR_MEM_ERR     = -3,
    DYN_ARR_INDEX_ERR   = -2
};

typedef struct Dyn_arr {
    void * data;
    int (*compar)(const void * key1, const void * key2);
    void (*destroy)(void * elem);
    int capacity;
    int elm_size;
    int elements;
    int sorted;
} Dyn_arr;

int dyn_arr_create(Dyn_arr * darr, int capacity, int element_size,
                   int (*compar)(const void * key1, const void * key2),
                   void (*destroy)(void * elem));
/*
Returns: 0 on success, an error code otherwise.

Description: Creates an empty array and marks it as sorted.
Allocates capacity * element_size bytes of memory.
compar and destroy may be NULL if not needed.

Complexity: O(1)
*/

int dyn_arr_capacity(Dyn_arr * darr);
/*
Returns: The current capacity of darr, or an error code.

Description: Reports the maximum number of elements darr
can hold before having to grow.

Complexity: O(1)
*/

int dyn_arr_elements(Dyn_arr * darr);
/*
Returns: The number of elements in darr, or an error code.

Description: Reports the current number of elements in darr.

Complexity: O(1)
*/

int dyn_arr_element_size(Dyn_arr * darr);
/*
Returns: How big each element in darr is in bytes, or an error code.

Description: Reports the byte size of darr's elements.

Complexity: O(1)
*/

int dyn_arr_const_data(Dyn_arr * darr, const void ** data_out);
int dyn_arr_data(Dyn_arr * darr, void ** data_out);
/*
Returns: 0 on success and data_out is set to point to the data block
of darr, or an error code.

Description: dyn_arr_data() retrieves the pointer as non-const and
marks the array as NOT sorted, even if you don't write through data_out.
dyn_arr_const_data() does not change the current sorted state of darr
and makes you promise you won't change anything through data_out.

Complexity: O(1)
*/

int dyn_arr_is_sorted(Dyn_arr * darr);
/*
Returns: 1/true if darr is sorted, 0/false if not, or an error code.

Description: Reports if the array is sorted.

Complexity: O(1)
*/


int dyn_arr_is_empty(Dyn_arr * darr);
/*
Returns: 1/true if darr has 0 elements, or an error code.

Description: Reports if the array is empty.

Complexity: O(1)
*/

int dyn_arr_destroy(Dyn_arr * darr);
/*
Returns: 0 on success, an error code otherwise.

Description: If a destroy function was passed to darr when it was
created, then that function is applied to every element.
The data block allocated for darr is freed, the structure darr points to
is zeroed out.

Complexity: O(n) if there's a destroy function, O(1) otherwise.
*/


int dyn_arr_trim(Dyn_arr * darr);
/*
Returns: 0 on success, an error code otherwise.
Note: DYN_ARR_INDEX_ERR is returned if you're trying to trim an empty array.

Description: Shrinks the array capacity to exactly fit the number of elements.
e.g. if the array has space for 16 elements but is currently holding only 9
the unused memory pass the 9th element is freed.

Complexity: O(1) or O(n) depending on the implementation of realloc()
*/


int dyn_arr_reset(Dyn_arr * darr);
/*
Returns: 0 on success, an error code otherwise.

Description: "empties" the array by setting the element counter to 0,
allowing you to reuse the current memory block. Marks the array as sorted.

Complexity: O(1)
*/


int dyn_arr_append_array(Dyn_arr * darr, void * arr, int elem_size, int len);
/*
Returns: 0 on success, an error code otherwise.

Description: Appends an array of length len pointed to by arr to the end of darr.
elem_size is provided for flexibility and should be the same as the element size
of darr in most cases. Marks the array as NOT sorted.

Complexity: O(n) where n is the number of bytes to be appended.
*/


int dyn_arr_push_back(Dyn_arr * darr, const void * key);
/*
Returns: 0 on success, an error code otherwise.

Description: Copies the data pointed to by key and appends it to darr.
Marks the array as NOT sorted.

Complexity: O(1)
*/

int dyn_arr_pop_back(Dyn_arr * darr, void ** last_out);
/*
Returns: 0 on success, an error code otherwise.

Description: Sets last_out to point to the last element and removes it
from the array, decreasing the element counter by one.
Note: The data pointed to by last_out should be copied or used immediately,
as it's no longer considered a part of the array.

Complexity: O(1)
*/

int dyn_arr_write_at_index(Dyn_arr * darr, int index, const void * key);
/*
Returns: 0 on success, an error code otherwise.

Description: Copies the data pointed to by key to the specified index.
Marks the array as NOT sorted.

Complexity: O(1)
*/

int dyn_arr_insert_at_index(Dyn_arr * darr, int index, const void * key);
/*
Returns: 0 on success, an error code otherwise.

Description: Moves the element at index and those to it's right, if any,
one to the right and copies the data pointed to by key to the specified index.
Marks the array as NOT sorted.

Complexity: O(n) where n is the number of bytes to be moved to the right.
*/

int dyn_arr_remove_at_index(Dyn_arr * darr, int index);
/*
Returns: 0 on success, an error code otherwise.

Description: Removes the element at the specified index and moves
all elements, if any, on its right one to the left.

Complexity: O(n) where n is the number of elements to be moved to the left.
*/

int dyn_arr_remove_by_value(Dyn_arr * darr, const void * key);
/*
Returns: 0 on success, DYN_ARR_NOT_FOUND if there's no match to the key,
or an error code otherwise.

Description: Looks for a match for the data pointed to by key, removing the
first one it comes across. All elements, if any, on the right of the match
are moved one to the left.

Complexity: O(n) where n is the number of elements to be moved to the left.
*/

int dyn_arr_read_at_index(Dyn_arr * darr, int index, const void ** read_out);
/*
Returns: 0 on success, an error code otherwise.

Description: Points read_out to the data specified by index.

Complexity: O(1)
*/

int dyn_arr_read_back(Dyn_arr * darr, const void ** back_out);
/*
Returns: 0 on success, an error code otherwise.

Description: Points back_out to the last element of the array.

Complexity: O(1)
*/

int dyn_arr_sort(Dyn_arr * darr);
/*
Returns: 0 on success, an error code otherwise.

Description: Sorts the array and marks it as sorted.

Complexity: O(1) if the array is sorted. O(n) if the array is marked as
NOT sorted but is already in sorted order. If it's not sorted, qsort()
is used. The complexity of that is unspecified by the standard, but expected
to be around O(n log n).
*/

int dyn_arr_const_find_from(Dyn_arr * darr, int index, const void * key, const void ** found);
#define dyn_arr_const_find(darr, key, found)      dyn_arr_const_find_from((darr), DYN_ARR_START, (key), (found))
int dyn_arr_find_from(Dyn_arr * darr, int index, const void * key, void ** found);
#define dyn_arr_find(darr, key, found)      dyn_arr_find_from((darr), DYN_ARR_START, (key), (found))
/*
Returns: If the element is found, found is set to point to the data matching the key, and
the return value is the index of the matching element inside the array. If no match is found,
the return value is DYN_ARR_NOT_FOUND and found is set to NULL. An error code is returned otherwise.

Description: The find functions use binary search if the array is marked as sorted, or linear
search if it's marked as NOT sorted.
dyn_arr_find_from() marks the array as NOT sorted, as there's no promise you won't write through
the found pointer. dyn_arr_const_find_from() does not change the sorted status of the darr.
The dyn_arr_find() and dyn_arr_const_find() macros are provided for convenience when you want to
search from the start of the array.

Complexity: O(n log n) if the array is sorted, O(n) otherwise.
*/


int dyn_arr_const_apply_to(Dyn_arr * darr, int elmnt_index,
                           void (*func)(const void * arr_elmnt, void * args),
                           void * func_args);
#define dyn_arr_const_apply(darr, f, fa)      dyn_arr_const_apply_to((darr), DYN_ARR_APPLY_ALL, (f), (fa))
int dyn_arr_apply_to(Dyn_arr * darr, int elmnt_index,
                     void (*func)(void * arr_elmnt, void * args),
                     void * func_args);
#define dyn_arr_apply(darr, f, fa)      dyn_arr_apply_to((darr), DYN_ARR_APPLY_ALL, (f), (fa))
/*
Returns: 0 on success, an error code otherwise.

Description: The apply functions apply the function pointed to by func to the element specified by
elmnt_index. func_args may be NULL if not used.
dyn_arr_apply_to() marks the array as NOT sorted, as there's no promise func() won't change the array
data. dyn_arr_const_apply_to() does not change the sorted status of the array.
dyn_arr_apply() and dyn_arr_const_apply() are provided for convenience when you want to apply func() to
the entire array.

Complexity: O(1) for a single element, O(n) for the whole array, not counting the complexity of func()
*/

int dyn_arr_insert_online(Dyn_arr * darr, const void * key);
/*
Returns: 0 on success, an error code otherwise.

Description: Uses binary insertion sort to insert the data pointed to by key,
keeping the array sorted as elements are added. The array must be marked as sorted
before using this function. Trying to insert online in an array marked as NOT sorted
is considered a runtime error and DYN_ARR_BAD_ARRAY is returned.

Complexity: Insertion sort runs in O(n*n), however the insertion sort here is optimized
to make O(log n) comparisons and move elements in one go with memmove().
*/
#endif // DYNAMIC_ARRAY_H
