/* arrlist.h -- public interface for array list */
/* v1.01 for graph */
/*	Description: this array list data structure  is implemented as a dynamic array
	containing void pointers, allowing for storage of generic data. Elements are reached
	by indexing into the array list using arrlist_get_elmt(list, element). The first element 
	is always at index 0. When an element is removed, the pointer in the array list is set
	to NULL and moved to the back right after the tail, so the list is never fragmented. 
	arrlist_sort(list) provides sorting with the qsort() standard function. 
	arrlist_bsearch(list, key) provides binary search through the list and returns not a pointer, 
	but the index of the element if it is found. arrlist_search_from(list, position, key) provides 
	linear search through the list starting from a specified position. 
	arrlist_lsearch(list, key) is a macro for arrlist_search_from() and performs
	linear search through the whole list starting from the head. A user defined function can be applied 
	to elements of the list with arrlist_apply_elmt() and arrlist_apply_all(). 
	By providing a destroy function the user is able to deallocate memory allocated for the nodes 
	when the list is being destroyed. If no destroy function is set, the data pointed to by the
	list is left untouched. Providing a compar function is a must for searching and sorting. 
	When the array fills up it's reallocated and grows twice in size. */

/* Author: Vladimir Dinev */
#ifndef ARRLIST_H
#define ARRLIST_H

#include <stdlib.h> // for NULL

#define ARRLIST_DEF_SIZE	256 // default size
#define ARRLIST_APPLY_ALL	-2

// return values
enum {ARRLIST_SUCCESS = 0,
	  ARRLIST_EMPTY_LIST = -1, 
	  ARRLIST_BAD_PTR_ARG = -2, 
	  ARRLIST_BAD_POSITION = -3,
	  ARRLIST_ELMT_NOT_FOUND = -4,
	  ARRLIST_MEM_ALLOC_FAILED = -5};

/* the elements of the array list */
typedef void * ArrlistElmt;

/* the array list structure */
typedef struct Arrlis_ {
	int list_size;		// the number of elements currently in the list
	int cont_size;		// the number of elements the list can hold before having to grow
	int (*compar)(const void * key1, const void * key2); // for sorting and searching
	void (*destroy)(void * data);
	ArrlistElmt * cont; // the container; pointer to the array of elements
	int head;			// first element index
	int tail;			// last element index
} Arrlist;

/* public interface */
/* Note: values returned on failure are different from one another, but are all less than 0 */

int arrlist_init(Arrlist * list, int (*compar)(const void * key1, const void * key2), 
			   void (*destroy)(void * data));
/* 
returns: 
ARRLIST_SUCCESS, 
ARRLIST_MEM_ALLOC_FAILED on failure

description: Initializes the array list specified by list. Must be called before list can be used. 
destroy provides a user defined way to free memory when list_destroy is called. destroy can be NULL.

complexity: O(1) 
*/

void arrlist_destroy(Arrlist * list);
/*
returns: nothing

description: Destroys list by calling destroy from arrlist_init, provided it's not NULL. 
This function sets the size to 0 and frees the container memory.

complexity: O(1) to O(n)
*/

void * arrlist_get_elmt(Arrlist * list, int n_elmt);
/*
returns: a pointer to the data in list->cont[n_elmt] or NULL if n_elmt is not in the boundaries of the list

description: Checks the array boundaries and returns a pointer to the data at the specified index.

complexity: O(1)
*/

int arrlist_insert(Arrlist * list, int position, void * data);
/*
returns: 
ARRLIST_SUCCESS, 
ARRLIST_MEM_ALLOC_FAILED if grow failed, 
ARRLIST_BAD_POSITION if bad position

description: Inserts an element at a position specified by position. If position is > than list->tail,
the element is inserted after the tail.

complexity: O(1) for inserting at the tail, O(n) for inserting at the head
*/

int arrlist_remove(Arrlist * list, int position, void ** data);
/*
returns: 
ARRLIST_SUCCESS on success, 
ARRLIST_EMPTY_LIST if list is empty, 
ARRLIST_BAD_POSITION if position is bad, 
ARRLIST_BAD_PTR_ARG if data is NULL

description: Removes the element at the position specified by position. The removed element's pointer is
set to NULL and moved to the back of the list right after the tail. A pointer to the data is returned in
data.

complexity: O(1) for removing the tail, O(n) for removing the head
*/

int arrlist_apply_elmt(Arrlist * list, int element, 
					void(*fun)(Arrlist * list, int n_elmt, void * args), void * fun_args);
/*
returns: 
ARRLIST_SUCCESS on success, 
ARRLIST_EMPTY_LIST if list is empty, 
ARRLIST_BAD_POSITION if bad position, 
ARRLIST_BAD_PTR_ARG fun() is NULL

description: Applies fun() to element; if element is ARRLIST_APPLY_ALL, fun() is applied from head to tail.

complexity: O(1)
*/

#define arrlist_apply_all(list, fun, args) arrlist_apply_elmt((list), ARRLIST_APPLY_ALL, (fun), (args))
/*
returns: same as arrlist_apply_elmt()

description: Macro for arrlist_apply_elmt(); applies fun() to all elements of list.

complexity: O(n)
*/

int arrlist_sort(Arrlist * list);
/*
returns:
ARRLIST_SUCCESS on success, 
ARRLIST_BAD_PTR_ARG if no compar function,
ARRLIST_EMPTY_LIST if list is empty

description: Sorts the array list. Uses the standard qsort().

complexity: Unspecified by the standard; O(n log n) if quicksort is used;
*/

int arrlist_bsearch(Arrlist * list, const void * key);
/*
returns: 
the index of the key in the array list if key is found,
ARRLIST_ELMT_NOT_FOUND if key is not found,
ARRLIST_BAD_PTR_ARG if no compar function,
ARRLIST_EMPTY_LIST if list is empty

description: Binary searches for a key in the list. If there are multiple instances of the key
the index returned is not necessary the first first instance. Works only if the array list is sorted.

complexity: O(log n)
*/

int arrlist_search_from(Arrlist * list, int position, const void * key);
/*
returns:
the index of the key in the array list if key is found,
ARRLIST_ELMT_NOT_FOUND if key is not found,
ARRLIST_BAD_PTR_ARG if no compar function,
ARRLIST_EMPTY_LIST if list is empty

description: Performs a linear search for key in list starting from position.

complexity: O(n)
*/

#define arrlist_lsearch(list, key) arrlist_search_from((list), (list)->head, (key))
/*
returns: same as arrlist_search_from()

description: Macro; performs a linear search for key in list starting from the start of the list.

complexity: O(n)
*/

#endif