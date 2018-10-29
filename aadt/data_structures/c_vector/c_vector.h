/*
	c_vector.h
    A dynamic array, also good for a stack.
    Written by Vladimir Dinev
    v1.0
*/

#ifndef C_VECTOR_H
#define C_VECTOR_H

#define C_VECT_DEFAULT_CAPACITY 16
#define C_VECT_GROWTH_RATE      2

// Do not use the struct directly
typedef struct c_vector {
    void * arr;
    int elem_count;
    int elem_size;
    int capacity;
} c_vector;

void * c_vect_make_cap(c_vector * cv, int elem_size, int capacity);
#define c_vect_make(cv, n) c_vect_make_cap((cv), (n), C_VECT_DEFAULT_CAPACITY)
/*
Returns: cv on success, NULL otherwise.

Description: Creates a vector of 0 elements, allocates elem_size*capacity bytes
of memory. elem_size and capacity have to be > 0. On failure this function sets
the cv struct to all 0s.
*/

void c_vect_destroy(c_vector * cv);
#define c_vect_destroy_null(cv) c_vect_destroy((cv)), (cv) = NULL
/*
Returns: Nothing.

Description: Frees the allocated memory and sets the cv struct to 0s.
*/

void * c_vect_push(c_vector * cv, void * what);
/*
Returns: A pointer to the appended element in the array on success,
NULL otherwise.

Description: Appends the data pointed to by the what argument to the end of the
array. If the array is full at the time of the operation, the array is
reallocated and grows C_VECT_GROWTH_RATE times before appending.
*/

void * c_vect_pop(c_vector * cv);
/*
Returns: cv on success, NULL otherwise.

Description: Removes an element from the array by decrementing its element
counter by 1 only if the array is not empty. If the array is empty, nothing
happens and NULL is returned.
*/

void * c_vect_peek(c_vector * cv);
/*
Returns: A pointer to the last element of the array on success, NULL otherwise.

Description: Peeks at the last array element/top of the stack, only if the array
is not empty. If the array is empty, NULL is returned.
*/

void * c_vect_get(c_vector * cv, int ind);
#define c_vect_data(cv) c_vect_get((cv), 0)
/*
Returns: A pointer the the element at index ind on success, NULL if ind is out
of bounds.

Description: Indexes into the array and checks bounds.
*/

void * c_vect_resize(c_vector * cv, int new_capacity);
/*
Returns: cv on success, NULL otherwise.

Description: Reallocates and resize the array to new_capacity. new_capacity has
to be > 0. If new_capacity is less than the current number of elements in the
array, the number of elements in the array is trimmed to the value of
new_capacity.
*/

void * c_vect_trim(c_vector * cv);
/*
Returns: cv on success, NULL otherwise.

Description: Fits the array size to match exactly the space needed for the
number of elements the array currently holds. Reallocates if necessary.
*/

int c_vect_length(c_vector * cv);
/*
Returns: The number of elements in the array.

Description: As above.
*/

int c_vect_elem_size(c_vector * cv);
/*
Returns: The size of an array element in byte.

Description: As above.
*/

int c_vect_capacity(c_vector * cv);
/*
Returns: The maximum number of elements the array can hold before having to
grow.

Description: As above.
*/

#endif
