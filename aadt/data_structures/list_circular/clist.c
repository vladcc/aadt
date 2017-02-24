/* clist.c -- circular singly linked list implementation */
/* v1.0 */
#include <stdlib.h> // for NULL
#include <string.h> // for memset()
#include <stdbool.h>
#include "clist.h"

void clist_init(CList * list, int (*compar)(const void * key1, const void * key2), 
			   void (*destroy)(void * data))
{
	/* initialize the list */
	list->size = 0;
	list->compar = compar;
	list->destroy = destroy;
	list->head = NULL;
	
	return;
}

void clist_destroy(CList * list)
{
	/* destroy the list */
	void * data;
	
	while (list->size > 0)
	{
		// remove every element and optionally call the user function
		if (clist_rem_next(list, list->head, (void **)&data) == 0 && list->destroy != NULL)
			list->destroy(data);
	}
	
	// zero out memory
	memset(list, 0, sizeof(CList));
	
	return;
}

int clist_ins_next(CList * list, CListElmt * element, const void * data)
{
	/* make and insert a new element right after element */
	CListElmt * new_element;
	
	// allocate memory
	if ((new_element = (CListElmt *)malloc(sizeof(CListElmt))) == NULL)
		return -1;
	
	new_element->data = (void *)data;
	
	// handle insertion in an empty list
	if (0 == list->size)
	{
		// first element points to self
		new_element->next = new_element;
		list->head = new_element;
	}
	else
	{
		// insert in non-empty list
		new_element->next = element->next;
		element->next = new_element;
	}	
	
	// adjust the size
	list->size++;
	
	return 0;
}

int clist_rem_next(CList * list, CListElmt * element, void ** data)
{
	/* remove the next element */
	
	// don't remove from an empty list
	if (0 == list->size)
		return -1;
	
	CListElmt * old_element;
	
	// remove the element
	*data = element->next->data;
	
	// in case of removing the last element
	if (element->next == element)
	{
		old_element = element->next;
		list->head = NULL;
	}
	else
	{
		old_element = element->next;
		element->next = element->next->next;
	}
	
	// free element pointer
	free(old_element);
	
	// adjust list size
	list->size--;
	
	return 0;
}

int clist_apply_elmt(CList * list, CListElmt * element, 
					void(*fun)(CListElmt * l_element, void * args), void * fun_args)
{
	/* applies fun() to element; if NULL == element, applies fun() to whole list */
	
	// go home if no fun()
	if (NULL == fun)
		return -1;
	
	if (NULL == element)
	{
		// apply to all elements
		CListElmt * tmp_element = list->head;
		int i = 0;
		while (i < list->size)
		{
			fun(tmp_element, fun_args);
			tmp_element = tmp_element->next;
			++i;
		}
	}
	else // apply for current element
		fun(element, fun_args);
	
	return 0;
}

int clist_sort(CList * list, int sort_up_down)
{
	/* bubble sort */
	if ( (list->size < 2) || (list == NULL) || (NULL == list->compar))
		return -1;
	
	if ( (sort_up_down != CLIST_SORT_UP) && (sort_up_down != CLIST_SORT_DOWN) )
		return -1;
	
	CListElmt * el;
	void * tmp;
	bool swapped = true;
	int i;
	
	while (swapped)
	{
		swapped = false;
		for (i = 1, el = list->head; i < list->size; el = el->next, ++i)
		{
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

CListElmt * clist_find_from(CList * list, CListElmt * start, void * key)
{
	/* linear search for key through list */
	if ((list->size < 1) || (NULL == list) || (NULL == list->compar))
		return NULL;
	
	CListElmt dummy = {.data = key, .next = NULL};
	CListElmt * el;
	
	if (NULL == start)
		start = list->head;
	
	el = start;
	
	do
	{
		if (list->compar(el, &dummy) == 0)
			return el;
		
		el = el->next;
		
	} while (el != list->head);
	
	return NULL;
}