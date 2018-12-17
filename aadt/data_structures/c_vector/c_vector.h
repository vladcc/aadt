/*  c_vector.h -- a dynamic array
    v1.11

    A dynamic array implementation, much like the C++ vector.
    It's implemented with memcpy(), so it copies whatever you provide it with
    inside itself. As such it can contain pointers to data allocated somewhere
    else, as well as actual copies of data.

    The structure keeps track of the number of elements it currently has, along
    with the maximum number of elements it can have before it has to grow,
    which is its capacity. When any random access operation is performed, the
    array checks if the requested index is in the range of 0 to the number of
    elements minus 1, the index of its current last element, including. When
    the capacity is reached, the array grows at the very next insert or push
    operation. The new size is determined by the result of the multiplication
    of its capacity with C_VECT_GROWTH_RATE.

    c_vector provides facilities for inserting, removing, copying, applying
    client-specified functions to its elements, a stack interface, sorting,
    linear and binary search, and online insertion. Even though anything that
    has to do with removal or insertion takes O(n) time, these are implemented
    with memmove(), so they can be surprisingly quick.

    Author: Vladimir Dinev
    vld.dinev@gmail.com
    2018-12-15
*/

#ifndef C_VECTOR_H
#define C_VECTOR_H

#include <stdbool.h>
#include <stddef.h>

#define C_VECT_DEFAULT_CAPACITY 16
#define C_VECT_GROWTH_RATE      2

// a three way comparison function like compar for qsort()
typedef int (*fcomp)(const void * one, const void * two);

// a callback that takes no additional arguments
typedef void (*fapply)(void * elem);

// a callback that takes additional arguments
typedef void (*fapply_args)(void * elem, void * args);

// Do not use directly
typedef struct c_vector {
    void * arr;
    fcomp compar;
    int elem_count;
    int elem_size;
    int capacity;
} c_vector;

void * c_vect_make_cap(
    c_vector * cv, int elem_size, fcomp compar, int capacity
    );
#define c_vect_make(cv, elem_size, compar)\
c_vect_make_cap((cv), (elem_size), (compar), C_VECT_DEFAULT_CAPACITY)
/*
Returns: cv on success, NULL otherwise.

Description: Creates a vector with 0 elements, allocates elem_size * capacity
bytes of memory. elem_size and capacity have to be positive numbers. On failure
the struct pointed to by cv is set to all 0s.

Complexity: O(1)
*/

void c_vect_destroy(c_vector * cv);
#define c_vect_destroy_null(cv)\
c_vect_destroy((cv)), (cv) = NULL
/*
Returns: Nothing.

Description: Frees the allocated memory and sets the struct pointed to by cv
to all 0s.

Complexity: O(1)
*/

void * c_vect_push(c_vector * cv, const void * what);
/*
Returns: A pointer to the appended element in the array on success,
NULL otherwise.

Description: Appends the data pointed to by what to the end of the array. If the
array is full at the time of the operation, it gets reallocated and grows
C_VECT_GROWTH_RATE times its current capacity before appending.

Complexity: O(1) amortized.
*/

void * c_vect_pop(c_vector * cv);
/*
Returns: cv on success, NULL if cv is empty.

Description: Removes the last element from the array by decrementing the element
counter by 1.

Complexity: O(1)
*/

void * c_vect_peek(c_vector * cv);
/*
Returns: A pointer to the last element of the array, NULL if the array is empty.

Description: Peeks at the last array element.

Complexity: O(1)
*/

void * c_vect_get(c_vector * cv, int index);
/*
Returns: A pointer to the element at index, NULL if index is out of range, or
the array is empty.

Description: Indexes into the array. index is out of range if it's less than 0,
or equal or greater than the number of elements currently in the array.

Complexity: O(1)
*/

void * c_vect_insert_at_index(c_vector * cv, int index, const void * key);
/*
Returns: A pointer to the inserted element inside the array on success,
NULL if index is out of range.

Description: Insert the element pointed to by key at index after first moving
all elements from index one to the right. index is out of range if it's less
than 0, or equal or greater than the number of elements currently in the array.

Complexity: O(n)
*/

void * c_vect_insert_online_ind(
    c_vector * cv, const void * key, int * out_index
);
#define c_vect_insert_online(cv, key)\
c_vect_insert_online_ind((cv), (key), NULL)
/*
Returns: A pointer to the inserted element in cv. If out_index is not NULL,
the variable pointed to by out_index contains the index of the inserted element.

Description: Inserts the element pointed to by key in its proper place in the
ordered array cv, therefore keeping cv sorted. Implemented with binary insertion
sort. Calling this function on an unsorted array results in undefined behavior.

Complexity: O(log n) for finding the insertion point, O(n) for the insertion.
*/

void * c_vect_write_at_index(c_vector * cv, int index, const void * key);
/*
Returns: A pointer to the element written inside the array on success,
NULL if index is out of range.

Description: Overwrites the element at index with the element pointed to by key.
index is out of range if it's less than 0, or equal or greater than the number
of elements currently in the array.

Complexity: O(1)
*/

void * c_vect_remove_at_index(c_vector * cv, int index);
/*
Returns: cv on success, NULL if index is out of range.

Description: Removes the element at index by moving all memory on the right of
that index one to the left. index is out of range if it's less than
0, or equal or greater than the number of elements currently in the array.

Complexity: O(n)
*/

void * c_vect_find_ind(c_vector * cv, const void * key, int * out_index);
#define c_vect_find(cv, key)\
c_vect_find_ind((cv), (key), NULL)
/*
Returns: A pointer to the element inside the array if found, NULL otherwise.
If out_index is not NULL and NULL is returned, the value of the variable
pointed to by out_index is -1. If out_index is not NULL and key is found, it's
the index of the element found inside the array.

Description: Performs a liner search for key. If out_index is not used, it can
be set to NULL.

Complexity: O(n)
*/

void * c_vect_bsearch_ind(c_vector * cv, const void * key, int * out_index);
#define c_vect_bsearch(cv, key)\
c_vect_bsearch_ind((cv), (key), NULL)
/*
Returns: A pointer to the element inside the array if found, NULL otherwise.
If out_index is not NULL and NULL is returned, the value of the variable
pointed to by out_index is -1. If out_index is not NULL and key is found, it's
the index of the element found inside the array.

Description: Performs binary search on a sorted array. If there are several
elements with the same value inside the array, the pointer returned may point
to any one of them. If out_index is not used, it can be set to NULL. Calling
this function on an unsorted array results in undefined behavior.

Complexity: O(log n)
*/

void * c_vect_sort(c_vector * cv);
/*
Returns: cv

Description: Sorts the array if it is not sorted.

Complexity: O(n) if it's already sorted. Otherwise, as per the cstdlib's
qsort(), sorting is approximately O(n log n).
*/

void * c_vect_apply(c_vector * cv, fapply fun);
/*
Returns: cv

Description: Applies a function of type void (*f)(void) to all elements of the
array.

Complexity: O(n)
*/

void * c_vect_apply_args(c_vector * cv, fapply_args fun, void * args);
/*
Returns: cv

Description: Applies a function of type void (*f)(void * args) to all elements
of the array.

Complexity: O(n)
*/

void * c_vect_append_array(c_vector * cv, const void * arr, int len);
/*
Returns: cv on success, NULL if relocation failed.

Description: Grows cv if needed and appends len elements of the array pointed to
by arr.

Complexity: O(n) for potential relocation, O(n) for the append, where n is len.
*/

void * c_vect_copy(c_vector * dest, const c_vector * src);
/*
Returns: dest on success, NULL otherwise.

Description: Makes a deep copy of src in dest. dest is assumed to point to a
c_vector structure that has either not been initialized with c_vect_make(), or
c_vect_make_cap(), or it has been destroyed with c_vect_destroy(), but not
c_vect_destroy_null()

Complexity: O(n)
*/

void * c_vect_resize(c_vector * cv, int new_capacity);
/*
Returns: cv on success, NULL otherwise.

Description: Reallocates and resizes the array to new_capacity. new_capacity has
to be a positive number. If new_capacity is less than the current number of
elements in the array, the number of elements in the array is trimmed to the
value of new_capacity.

Complexity: Depends on realloc(), might be O(n)
*/

void * c_vect_trim(c_vector * cv);
/*
Returns: cv on success, NULL otherwise.

Description: Fits the array to match exactly the space needed for the number of
elements it currently holds. Relocates if necessary.

Complexity: Depends on realloc(), might be O(n)
*/

void * c_vect_zero_out(c_vector * cv);
/*
Returns: cv

Description: Fills with 0s the array memory of cv up to its capacity.

Complexity: O(n) where n is the number of bytes of cv's capacity.
*/

void * c_vect_set_length(c_vector * cv, int len);
/*
Returns: NULL if len is out of range, cv on success.

Description: Changes the number of elements in the array, effectively expanding
or shrinking the number of indices you have random access to. len is out of
range if it's less than 0, or larger than cv's capacity.

Complexity: O(1)
*/

void * c_vect_set_compar(c_vector * cv, fcomp compar);
/*
Returns: cv

Description: Changes the compar function in cv.

Complexity: O(1)
*/

void * c_vect_reset(c_vector * cv);
/*
Returns: cv

Description: After calling this function, cv would have exactly 0 elements.

Complexity: O(1)
*/

bool c_vect_is_empty(c_vector * cv);
/*
Returns: true if there are no elements in the array, false otherwise.

Description: Let's you know if cv is empty.

Complexity: O(1)
*/

void * c_vect_is_sorted(c_vector * cv);
/*
Returns: cv if the array is sorted, NULL otherwise.

Description: Checks if the array is sorted.

Complexity: O(n)
*/

void * c_vect_data(c_vector * cv);
/*
Returns: A pointer to the first element of cv.

Description: Gives you the start of the array.

Complexity: O(1)
*/

int c_vect_length(c_vector * cv);
/*
Returns: The number of elements in the array.

Description: Gets the size.

Complexity: O(1)
*/

int c_vect_elem_size(c_vector * cv);
/*
Returns: The size of an array element in byte.

Description: Gets the element size.

Complexity: O(1)
*/

int c_vect_capacity(c_vector * cv);
/*
Returns: The maximum number of elements the array can hold before having to
grow.

Description: Gets the capacity.

Complexity: O(1)
*/
#endif
