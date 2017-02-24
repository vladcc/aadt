/* clist.h -- circular singly linked list public interface */
/* v1.0 */
#ifndef CLIST_H
#define CLIST_H

#include <stdlib.h> // for NULL

#define CLIST_SORT_UP 	1
#define CLIST_SORT_DOWN -1

/* structure for the elements */
typedef struct CListElmt_ {
	void * data;
	struct CListElmt_ * next;
} CListElmt;

/* structure for the list */
typedef struct CList_ {
	int size;
	int (*compar)(const void * key1, const void * key2); // for sorting and searching
	void (*destroy)(void * data);
	CListElmt * head;
} CList;

/* public interface */
void clist_init(CList * list, int (*compar)(const void * key1, const void * key2), 
			   void (*destroy)(void * data));
/*
returns: nothing

description: Initializes the circular list specified by list. Before list can be used, it has to be
initialized. The compar argument is a function pointer to a data comparing function. The destroy 
argument is a function pointer to a function used to deallocate the elements of the list. Both function
pointers are optional and can be NULL.

complexity: O(1)
*/

void clist_destroy(CList * list);
/*
returns: nothing

description: Destroys the circular list specified by list, provided it is not NULL. After the list is
destroyed no other operations are permitted.

complexity: O(n)
*/

int clist_apply_elmt(CList * list, CListElmt * element, 
					void(*fun)(CListElmt * l_element, void * args), void * fun_args);
/*
returns: 0 on success, -1 on failure

description: Applies fun() to element; if element is NULL, fun() is applied from head to the last element.

complexity: O(1) to O(n)
*/

#define clist_apply_all(list, fun, args) clist_apply_elmt((list), NULL, (fun), (args))
/*
returns: 0 on success, -1 on failure

description: Macro for clist_apply_elmt(); applies fun() to all elements of list.

complexity: O(n)
*/

int clist_ins_next(CList * list, CListElmt * element, const void * data);
/*
returns: 0 on success, -1 on failure

description: Inserts an element after element in list. If element is NULL, the new one is inserted at the head of the list.
The new element contains a pointer to data, so the memory should remain valid as long as the element is in the list.
Memory managment is a responsibility of the caller.

complexity: O(1)
*/

#define clist_ins_ahead(list, data) clist_ins_next((list), (list)->head, (data))
/*
returns: 0 on success, -1 on failure

description: Macro for clist_ins_next(); inserts after the head of the list

complexity: O(1)
*/

int clist_rem_next(CList * list, CListElmt * element, void ** data);
/*
returns: 0 on success, -1 on failure

description: Removes the element after element from list. If element is NULL, the element at the head is removed.
Upon return, data points to the data stored in the element that was removed. The caller manages memory.

complexity: O(1)
*/

#define clist_rem_ahead(list, data) clist_rem_next((list), (list)->head, (data))
/*
returns: 0 on success, -1 on failure

description: Macro for clist_rem_next(); removes after the head of the list

complexity: O(1)
*/

#define clist_is_head(list, element) ((element) == (list)->head ? 1 : 0)
/*
returns: 1 if element is head of the list, 0 if not

description: Macro that determines whether the element is the head of a circular linked list.

complexity: O(1)
*/

int clist_sort(CList * list, int sort_up_down);
/*
returns: 0 on success, -1 of failure

description: Circular linked list sort function (bubble sort in this case). 1 sorts up, -1 sorts down.
Use CLIST_SORT_UP and CLIST_SORT_DOWN.

Note: Sorting begins always from the head!

complexity: O(n*n)
*/

CListElmt * clist_find_from(CList * list, CListElmt * start, void * key);
/*
returns: pointer to the element containing the key, NULL if not found or on error

description: Linear search through list looking for key. If start is NULL,
searching starts from the head. The search ends at the last element of the list.

Warning: If the element is found at the last node and you try to find it's next occurrence,
then the searching will begin again from the head! You should check if found_element->next == list->head

complexity: O(n)
*/

#define clist_find(list, key) clist_find_from((list), NULL, (key))
/*
returns: pointer to the element containing the key, NULL if not found or list is bad

description: Linear search through list looking for key starting from the head.

complexity: O(n)
*/
#endif