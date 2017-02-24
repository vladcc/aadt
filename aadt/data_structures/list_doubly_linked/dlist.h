/* dlist.h -- double linked list public interface */
/* v1.0 */
#ifndef DLINK_H
#define DLINK_H

#define DLIST_APPLY_ONCE		0
#define DLIST_APPLY_FORWARD		1
#define DLIST_APPLY_BACKWARD	2

#define DLIST_SORT_UP			1	// do not change
#define DLIST_SORT_DOWN		   -1	// do not change

#define DLIST_SEARCH_FORWARD	0
#define DLIST_SEARCH_BACKWARD	1

/* dlist element */
typedef struct DListElmt_ {
	void * data;
	struct DListElmt_ * prev;
	struct DListElmt_ * next;
} DListElmt;

/* the doubly-linked list */
typedef struct DList_ {
	int size;
	int (*compar)(const void * key1, const void * key2); // for sorting and searching
	void (*destroy)(void * data); // for deallocating pointed-to data
	DListElmt * head;
	DListElmt * tail;
} DList;

/* the public interface */
void dlist_init(DList * list, int (*compar)(const void * key1, const void * key2), 
			   void (*destroy)(void * data));
/*
return: nothing

description: Initializes the doubly-linked list specified by list. The destroy and compar arguments
provides a way to compare the data of two nodes, the destroy to deallocate the data's memory when
dlist_destroy() is called, respectively.

complexity: O(1)
*/

void dlist_destroy(DList * list);
/*
returns: nothing

description: Destroys the doubly-linked liste specified by list. No other operations are permited until
the list is initialized again. All list elements are removed but the data in them is not deallocated,
unless the user has specified a destroy function.

complexity: O(n)
*/

int dlist_ins_next(DList * list, DListElmt * element, const void * data);
/*
returns: 0 on success, -1 on failure

description: Inserts an element after element in the doubly-link list specified by list. When inserting
in an empty list, element should be NULL. The user manages storage associated with data.

complexity: O(1)
*/

int dlist_ins_prev(DList * list, DListElmt * element, const void * data);
/*
returns: 0 on success, -1 on failure

description: Inserts an element before element in the doubly-linked list specified by list. When inserting
in an empty list, element should be NULL. The user manages storage associated with data.

complexity: O(1)
*/

int dlist_remove(DList * list, DListElmt * element, void ** data);
/*
returns: 0 on success, -1 on failure

description: Removes element from list. A pointer to the data of the removed element is returned in data.

complexity: O(1)
*/

#define dlist_is_head(list, element) ((element)->prev == NULL ? 1 : 0)
/*
returns: 1 if elements is the head of list, 0 if not

description: This macro determines if element is head of the list. The list parameter is only
for clarity and it's not used.

complexity: O(1)
*/

#define dlist_is_tail(list, element) ((element)->next == NULL ? 1 : 0)
/*
returns: 1 if elements is the tail of list, 0 if not

description: This macro determines if element is tail of the list. The list parameter is only
for clarity and it's not used.

complexity: O(1)
*/

int dlist_apply(DList * list, DListElmt * element, 
					 void(*fun)(DListElmt * l_element, void * args), void * fun_args,
					 int direction);
/*
returns: 0 on success, -1 on failure

description: Applies the fun function either only to element, from element to the tail including,
or from element to head including. DLIST_APPLY_ONCE, DLIST_APPLY_FORWARD, and DLIST_APPLY_BACKWARD
should be used respectively.

complexity: O(1) to O(n)
*/				

#define dlist_apply_elmt(list, element, fun, args) dlist_apply((list), (element), (fun), (args), DLIST_APPLY_ONCE)
/*
returns: 0 on success, -1 on failure

description: This macro applies the function pointed to by fun only for the element specified by element.

complexity: O(1)
*/

#define dlist_apply_forward(list, element, fun, args) dlist_apply((list), (element), (fun), (args), DLIST_APPLY_FORWARD)
/*
returns: 0 on success, -1 on failure

description: This macro applies the function pointed to by fun from the element specified by element
to the end of the list.

complexity: O(n)
*/

#define dlist_apply_backward(list, element, fun, args) dlist_apply((list), (element), (fun), (args), DLIST_APPLY_BACKWARD)
/*
returns: 0 on success, -1 on failure

description: This macro applies the function pointed to by fun from the element specified by element
to the beginning of the list.

complexity: O(n)
*/

int dlist_sort(DList * list, int sort_up_down);
/*
returns: 0 on success, -1 of failure

description: Linked list sort function (bubble sort in this case). 1 sorts up, -1 sorts down.
Use DLIST_SORT_UP and DLIST_SORT_DOWN.

complexity: O(n*n)
*/

DListElmt * dlist_find_from(DList * list, DListElmt * start, void * key, int direction);
/*
returns: pointer to the element containing the key, NULL if not found or on error

description: Linear search through list looking for key. DLIST_SEARCH_FORWARD and
DLIST_SEARCH_BACKWARD specify the direction of the search.

complexity: O(n)
*/

#define dlist_find_forward(list, element, key) dlist_find_from((list), (element), (key), DLIST_SEARCH_FORWARD)
/*
returns: pointer to the element containing the key, NULL if not found or on error

description: Macro for searching the list forward.

complexity: O(n)
*/

#define dlist_find_backward(list, element, key) dlist_find_from((list), (element), (key), DLIST_SEARCH_BACKWARD)
/*
returns: pointer to the element containing the key, NULL if not found or on error

description: Macro for searching the list backwards.

complexity: O(n)
*/

#endif