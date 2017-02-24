/* dlist.c -- double linked list implementation */
/* v1.0 */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "dlist.h"

void dlist_init(DList * list, int (*compar)(const void * key1, const void * key2), 
			   void (*destroy)(void * data))
{
	/* initialize the list */
	list->size = 0;
	list->compar = compar;
	list->destroy = destroy;
	list->head = NULL;
	list->tail = NULL;
	
	return;
}

void dlist_destroy(DList * list)
{
	/* destroy the list */
	void * data;
	
	while (list->size > 0)
	{
		if ( (dlist_remove(list, list->head, (void**)&data) == 0) && (list->destroy != NULL) )
			list->destroy(data);
	}
	
	// zero out memory
	memset(list, 0, sizeof(*list)); 
	
	return;
}

int dlist_ins_next(DList * list, DListElmt * element, const void * data)
{
	/* insert after element */
	DListElmt * new_element;
	
	// no NULL unless the list is empty
	if (NULL == element && list->size != 0)
		return -1;
	
	// allocate memory
	if ( (new_element = (DListElmt *)malloc(sizeof(*new_element))) == NULL)
		return -1;
	
	// insert in the list
	new_element->data = (void *)data;
	if (0 == list->size)
	{
		// insert as head in empty list
		list->head = new_element;
		list->head->prev = NULL;
		list->head->next = NULL;
		list->tail = new_element;
	}
	else
	{
		// insert elsewhere
		new_element->next = element->next;
		new_element->prev = element;
		
		// update tail
		if (NULL == element->next)
			list->tail = new_element;
		else
			element->next->prev = new_element;
		
		element->next = new_element;
	}
	
	// update size
	list->size++;
		
	return 0;
}

int dlist_ins_prev(DList * list, DListElmt * element, const void * data)
{
	/* insert before element */
	DListElmt * new_element;
	
	// do not allow NULL unless the list is empty
	if (NULL == element && list->size != 0)
		return -1;
	
	// allocate memory
	if ( (new_element = (DListElmt *)malloc(sizeof(*new_element))) == NULL)
		return -1;
	
	// insert
	new_element->data = (void *)data;
	
	if (0 == list->size)
	{
		// insert as head in an empty list
		list->head = new_element;
		list->head->prev = NULL;
		list->head->next = NULL;
		list->tail = new_element;
	}
	else
	{
		// insert elsewhere
		new_element->next = element;
		new_element->prev = element->prev;
		
		//update head
		if (NULL == element->prev)
			list->head = new_element;
		else
			element->prev->next = new_element;
		
		element->prev = new_element;
	}

	// update size
	list->size++;
	
	return 0;
}

int dlist_remove(DList * list, DListElmt * element, void ** data)
{
	/* remove element */
	
	// do not remove NULL or remove from empty list
	if (NULL == element || 0 == list->size)
		return -1;
	
	// give data back
	*data = element->data;
	
	// remove from the head
	if (element == list->head)
	{
		list->head = element->next;
		if (NULL == list->head)
			list->tail = NULL;
		else
			element->next->prev = NULL;
	}
	else
	{
	// remove elsewhere
		element->prev->next = element->next;
		// update tail
		if (NULL == element->next)
			list->tail = element->prev;
		else
			element->next->prev = element->prev;
	}
	
	free(element);
	
	// update size
	list->size--;
	
	return 0;
}

int dlist_apply(DList * list, DListElmt * element, 
					 void(*fun)(DListElmt * l_element, void * args), void * fun_args,
					 int direction)
{
	/* applies fun to either element, from element until the end of the list
	 * or from element until the beginning of the list */

	 // if fun is bad go home
	 if (NULL == fun || NULL == element)
		 return -1;

	 // if direction is bad go home
	 if (direction != DLIST_APPLY_ONCE && direction != DLIST_APPLY_FORWARD &&
		 direction != DLIST_APPLY_BACKWARD)
		 return -1;
	
	// apply in place
	if (DLIST_APPLY_ONCE == direction)
		fun(element, fun_args);
	else if (DLIST_APPLY_FORWARD == direction)
	{
		// apply forward
		while (element != NULL)
		{
			fun(element, fun_args);
			element = element->next;
		}
	}
	else if (DLIST_APPLY_BACKWARD == direction)
	{
		// apply backward
		while (element != NULL)
		{
			fun(element, fun_args);
			element = element->prev;
		}
	}
		
	return 0;
}

int dlist_sort(DList * list, int sort_up_down)
{
	/* bubble sort */
	if ( (list->size < 2) || (list == NULL) || (NULL == list->compar))
		return -1;
	
	if ( (sort_up_down != DLIST_SORT_UP) && (sort_up_down != DLIST_SORT_DOWN) )
		return -1;
	
	DListElmt * el;
	void * tmp;
	bool swapped = true;
	int i;
	
	while (swapped)
	{
		swapped = false;
		for (i = 1, el = list->head; i < list->size; el = el->next, ++i)
		{
			// compare data
			if (list->compar(el, el->next) == sort_up_down)
			{
				// swap data pointers
				tmp = el->data;
				el->data = el->next->data;
				el->next->data = tmp;
				swapped = true;
			}
		}
	}
	
	return 0;
}

DListElmt * dlist_find_from(DList * list, DListElmt * start, void * key, int direction)
{
	/* linear search for key through list */
	if ( (list->size < 1) || (NULL == list) || (NULL == list->compar) || (NULL == start) )
		return NULL;
	
	if (direction != DLIST_SEARCH_FORWARD && direction != DLIST_SEARCH_BACKWARD)
		return NULL;

	DListElmt dummy = {.data = key, .next = NULL};
	DListElmt * el;
	
	el = start;
	while (el != NULL)
	{
		if (list->compar(el, &dummy) == 0)
			return el;
		
		if (DLIST_SEARCH_FORWARD == direction)
			el = el->next;
		else
			el = el->prev;
	}
	
	return NULL;
}