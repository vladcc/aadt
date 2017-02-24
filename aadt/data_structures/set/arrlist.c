/* arrlist.c -- array list implementation */
/* v1.0 */
/* See arrlist.h for description. */
#include <stdlib.h> // for NULL, qsort()
#include <string.h> // for memset()
#include "arrlist.h"

static ArrlistElmt * arrlist_grow(Arrlist * list);

int arrlist_init(Arrlist * list, int (*compar)(const void * key1, const void * key2), 
			   void (*destroy)(void * data))
{	
	/* initialize the list */
	
	// allocate container
	if ((list->cont = (ArrlistElmt *)malloc(sizeof(ArrlistElmt) * ARRLIST_DEF_SIZE)) == NULL)
		return ARRLIST_MEM_ALLOC_FAILED;
	
	list->list_size = 0;
	list->cont_size = ARRLIST_DEF_SIZE;
	list->compar = compar;
	list->destroy = destroy;
	list->head = -1;
	list->tail = -1;

	return ARRLIST_SUCCESS;
}

void arrlist_destroy(Arrlist * list)
{	
	/* destroy the array list */
	
	// apply the destroy function if there is one
	if (list->list_size > 0 && list->destroy != NULL)
	{
		int i;
		for (i = 0; i < list->list_size; ++i)
			list->destroy(list->cont[i]);
	}
	
	// free the container memory
	free(list->cont);
	
	// zero out list memory
	memset(list, 0, sizeof(*list));
	
	return;
}

int arrlist_insert(Arrlist * list, int position, void * data)
{
	/* insert an element in the array list */
	
	// grow if needed
	if ( (list->list_size + 1) == list->cont_size)
	{
		if (arrlist_grow(list) == NULL)
			return ARRLIST_MEM_ALLOC_FAILED;
	}
	
	// check position
	if (list->list_size != 0 && position < list->head )
		return ARRLIST_BAD_POSITION;
	
	// insert first element
	if (list->list_size == 0)
	{
		list->head = 0;
		list->cont[0] = data;
	}
	else if (position < list->tail)
	{
		// insert elsewhere than the tail
		// move elements one to the left
		int i;
		for (i = list->list_size - 1; i >= position; --i)
			list->cont[i + 1] = list->cont[i];
		
		// insert element at position
		list->cont[position] = data;
	}
	else if (position >= list->tail)
	{
		// insert at the tail
		list->cont[list->tail + 1] = data;
	}
	
	// update tail
	list->tail = list->list_size;
	
	// update size
	list->list_size++;
	
	return ARRLIST_SUCCESS;
}

int arrlist_remove(Arrlist * list, int position, void ** data)
{
	/* remove an element from the list */
	
	// don't remove from empty list
	 if (0 == list->list_size)
		 return ARRLIST_EMPTY_LIST;
	 
	 // go home if bad position
	 if (position < list->head || position > list->tail)
		 return ARRLIST_BAD_POSITION;
	
	// go home if bad pointer
	if (NULL == data)
		return ARRLIST_BAD_PTR_ARG;
	
	// remove the element
	*data = list->cont[position];
	list->cont[position] = NULL;
	
	
	// get NULL to the back
	int i;
	ArrlistElmt * pae;
	for (i = position; i <= list->tail; ++i)
	{
		// swap elements
		pae = list->cont[i];
		list->cont[i] = list->cont[i + 1];
		list->cont[i + 1] = pae;
	}
	
	// update tail
	list->tail--;
	
	// update size
	list->list_size--;
	
	return ARRLIST_SUCCESS;
}

void * arrlist_get_elmt(Arrlist * list, int n_elmt)
{
	/* return pointer to data of the e_elmt element */
	if (n_elmt < list->head || n_elmt > list->tail)
		return NULL;
	
	return list->cont[n_elmt];
}

int arrlist_apply_elmt(Arrlist * list, int element, 
					void(*fun)(Arrlist * list, int n_elmt, void * args), void * fun_args)
{
	/* applies fun() to the element no. element of the list; applies to all elements if
	 *  element == ARRLIST_APPLY_ALL */
	 
	 // go home if list is empty
	 if (0 == list->list_size)
		 return ARRLIST_EMPTY_LIST;
	 
	 // go home if bad element
	 if ( (element < list->head && element != ARRLIST_APPLY_ALL) || (element > list->tail)  )
		 return ARRLIST_BAD_POSITION;
	 
	 // go home if there's no fun()
	 if (NULL == fun)
		 return ARRLIST_BAD_PTR_ARG;
	
	 if (ARRLIST_APPLY_ALL == element)
	 {
		 // apply to all elements
		 int i;
		 for (i = list->head; i <= list->tail; ++i)
			 fun(list, i, fun_args);
	 }
	 else
	 {
		 // apply once
		 fun(list, element, fun_args);
	 }
	 
	 return ARRLIST_SUCCESS;
}

int arrlist_sort(Arrlist * list)
{
	/* sort the list */
	if (NULL == list->compar)
		return ARRLIST_BAD_PTR_ARG;
	
	if (0 == list->list_size)
		return ARRLIST_EMPTY_LIST;
	
	qsort(list->cont, list->list_size, sizeof(list->cont[0]), list->compar);
	return ARRLIST_SUCCESS;
}

int arrlist_bsearch(Arrlist * list, const void * key)
{
	/* binary search the array list */
	if (NULL == list->compar)
		return ARRLIST_BAD_PTR_ARG;
	
	if (0 == list->list_size)
		return ARRLIST_EMPTY_LIST;
	
	int low, high, mid, cmp;
	
	low = list->head;
	high = list->tail;
	
	while (low <= high)
	{
		mid = low + ((high - low) / 2);
		cmp = list->compar(&key, &(list->cont[mid]));
		
		if (-1 == cmp)
			high = mid - 1;
		else if (1 == cmp)
			low = mid + 1;
		else
			return mid;	// found; return index
	}
	
	return ARRLIST_ELMT_NOT_FOUND;
}

int arrlist_search_from(Arrlist * list, int position, const void * key)
{
	/* linear search through the array list */
	
	// go home if no compar
	if (NULL == list->compar)
		return ARRLIST_BAD_PTR_ARG;
	
	// go home if list is empty
	if (0 == list->list_size)
		return ARRLIST_EMPTY_LIST;
	
	// go home if bad position
	 if (position < list->head || position > list->tail)
		 return ARRLIST_BAD_POSITION;
	
	int i;
	for (i = position; i < list->list_size; ++i)
	{
		if (list->compar(&key, &(list->cont[i])) == 0)
			return i; // found; return index
	}
	
	return ARRLIST_ELMT_NOT_FOUND;
}

static ArrlistElmt * arrlist_grow(Arrlist * list)
{
	/* make the array twice as big */
	ArrlistElmt * new_cont;
	int size = list->cont_size  * 2 * sizeof(*new_cont);
	
	new_cont = (ArrlistElmt *)realloc(list->cont, size);
	if (NULL == new_cont)
		return NULL;
	else
	{
		list->cont = new_cont;
		list->cont_size = size /  sizeof(*new_cont);
	}
		
	return list->cont;
}
