#include <stdio.h>
#include <stdlib.h>
#include "arrlist.h"

#define ARRLIST_PRINT_LSIZE	printf("list list size: %d\n", arrlist->list_size)
#define ARRLIST_PRINT_CSIZE	printf("list cont size: %d\n", arrlist->cont_size)
#define ARRLIST_PRINT_TAIL	printf("tail: %d\n", arrlist->tail)

#define ARRLIST_INITIALIZE \
		do \
		{puts("initializing list"); \
		arrlist_init(arrlist, compar, destroy); \
		ARRLIST_PRINT_LSIZE; \
		ARRLIST_PRINT_CSIZE;} while (0)
			
#define ARRLIST_DESTROY \
		do \
		{puts("destroying list"); \
		arrlist_destroy(arrlist); \
		ARRLIST_PRINT_LSIZE; \
		ARRLIST_PRINT_CSIZE;} while (0)
			
#define ARRLIST_INSERT(pos, el) \
		do \
		{ \
		int err = arrlist_insert(arrlist, (pos), (el)); \
		if (ARRLIST_MEM_ALLOC_FAILED == err) \
		{puts("err: mem alloc error"); getchar(); return -1;} \
		else if (ARRLIST_BAD_POSITION == err) \
			puts("err: bad insert position"); \
		} while (0)

#define ARRLIST_REMOVE(pos, pout) \
		do \
		{ int z; \
		z = arrlist_remove(arrlist, (pos), (pout)); \
		if (ARRLIST_EMPTY_LIST == z) \
			puts("err: list is empty"); \
		else if (ARRLIST_BAD_POSITION == z) \
			puts ("err: bad position"); \
		else if (ARRLIST_BAD_PTR_ARG == z) \
			puts("err: bad pointer"); \
		ARRLIST_PRINT_LSIZE; \
		ARRLIST_PRINT_CSIZE;} while (0)

#define ARRLIST_FILL(elements) \
		do \
		{printf("inserting %d elements\n", (elements)); \
		int i, * ip; \
		for (i = 0; i < (elements); ++i) \
		{ \
			ip = make_node(i + 5); \
			ARRLIST_INSERT(i, ip); \
		}\
		ARRLIST_PRINT_LSIZE; \
		ARRLIST_PRINT_CSIZE;} while (0)
			
#define ARRLIST_PRINT_LIST \
		do \
		{puts("printing list..."); \
		int z; \
		z = arrlist_apply_all(arrlist, print_elmnt, NULL); \
		if (ARRLIST_EMPTY_LIST == z) \
			puts("err: list is empty"); \
		else if (ARRLIST_BAD_POSITION == z) \
			puts ("err: bad position"); \
		else if (ARRLIST_BAD_PTR_ARG == z) \
			puts("err: bad pointer");} while (0)	

#define ARRLIST_SEARCH_RSLT \
		do \
		{if (ARRLIST_ELMT_NOT_FOUND == s) \
			puts("not found"); \
		else if (ARRLIST_BAD_PTR_ARG == s) \
			puts("err: no compar function"); \
		else if (ARRLIST_EMPTY_LIST == s) \
			puts("err: empty list"); \
		else \
			printf("index: %d, element: %d\n", s, *((int*)(arrlist_get_elmt(arrlist, s))));} while (0)			

#define ARRLIST_SORT \
		do \
		{puts("sorting list..."); \
		int s = arrlist_sort(arrlist); \
		if (ARRLIST_BAD_PTR_ARG == s) \
			puts("err: no compar function"); \
		else if (ARRLIST_EMPTY_LIST == s) \
			puts("err: list is empty"); \
		else \
			printf("index: %d, element: %d\n", s, *((int*)(arrlist_get_elmt(arrlist, s))));} while (0)					
				
int * make_node(int a);
void destroy(void * data);
int compar(const void * key1, const void * key2);
void print_elmnt(Arrlist * list, int n_elmt, void * args);

int main(void)
{
	int * el;
	Arrlist arrlist_;
	Arrlist * arrlist = &arrlist_;
	
	ARRLIST_INITIALIZE;
	
	ARRLIST_FILL(5);
	
	ARRLIST_PRINT_LIST;
	
	puts("\ninserting at the head");
	el = make_node(99);
	ARRLIST_INSERT(arrlist->head, el);
	ARRLIST_PRINT_LSIZE;
	ARRLIST_PRINT_CSIZE;
	ARRLIST_PRINT_TAIL;
	ARRLIST_PRINT_LIST;
	
	puts("\ninserting at the tail");
	el = make_node(98);
	ARRLIST_INSERT(arrlist->tail, el);
	ARRLIST_PRINT_LSIZE;
	ARRLIST_PRINT_CSIZE;
	ARRLIST_PRINT_TAIL;
	ARRLIST_PRINT_LIST;
	
	
	puts("\ninserting after the tail");
	el = make_node(97);
	ARRLIST_INSERT(arrlist->tail + 10, el);
	ARRLIST_PRINT_LSIZE;
	ARRLIST_PRINT_CSIZE;
	ARRLIST_PRINT_TAIL;
	ARRLIST_PRINT_LIST;
	
	printf("\ninserting at position %d\n", arrlist->list_size / 2);
	el = make_node(96);
	ARRLIST_INSERT(arrlist->list_size / 2, el);
	ARRLIST_PRINT_LSIZE;
	ARRLIST_PRINT_CSIZE;
	ARRLIST_PRINT_TAIL;
	ARRLIST_PRINT_LIST;
	
	puts("\ninserting before the head");
	el = make_node(76);
	ARRLIST_INSERT(arrlist->head - 1000, el);
	ARRLIST_PRINT_LSIZE;
	ARRLIST_PRINT_CSIZE;
	ARRLIST_PRINT_TAIL;
	ARRLIST_PRINT_LIST;
	
	puts("\nremoving head");
	ARRLIST_REMOVE(arrlist->head, (void *)&el);
	printf("element removed: %d\n", *el);
	free(el);
	ARRLIST_PRINT_TAIL;
	ARRLIST_PRINT_LIST;
	
	puts("\nremoving tail");
	ARRLIST_REMOVE(arrlist->tail, (void *)&el);
	printf("element removed: %d\n", *el);
	free(el);
	ARRLIST_PRINT_TAIL;
	ARRLIST_PRINT_LIST;
	
	printf("\nremoving from position %d\n", arrlist->list_size / 2 + 1);
	ARRLIST_REMOVE(arrlist->tail / 2 + 1,(void *)&el);
	printf("element removed: %d\n", *el);
	free(el);
	ARRLIST_PRINT_TAIL;
	ARRLIST_PRINT_LIST;
	
	puts("\nremoving before head");
	ARRLIST_REMOVE(arrlist->head - 12, (void *)&el);
	ARRLIST_PRINT_TAIL;
	ARRLIST_PRINT_LIST;
	
	puts("\nremoving after tail");
	ARRLIST_REMOVE(arrlist->tail + 10, (void *)&el);
	ARRLIST_PRINT_TAIL;
	ARRLIST_PRINT_LIST;
	
	puts("\nlinear search:");
	puts("searching for head:");
	el = arrlist_get_elmt(arrlist, arrlist->head);
	int s;
	s = arrlist_lsearch(arrlist, (void *)el);
	ARRLIST_SEARCH_RSLT;
	
	puts("searching for tail:");
	el = arrlist_get_elmt(arrlist, arrlist->tail);
	s = arrlist_search_from(arrlist, 3, (void *)el);
	ARRLIST_SEARCH_RSLT;
	
	int x = 3;
	printf("searching for element at index %d:\n", x);
	el = arrlist_get_elmt(arrlist, x);
	s = arrlist_search_from(arrlist, 3, (void *)el);
	// s = arrlist_lsearch(arrlist, (void *)el);
	ARRLIST_SEARCH_RSLT;
	
	int k = 215;
	printf("searching for element at index %d from position %d:\n", x, k);
	el = arrlist_get_elmt(arrlist, x);
	s = arrlist_search_from(arrlist, k, (void *)el);
	if (ARRLIST_BAD_POSITION == s)
		puts("err: bad positoin");
	
	k = -20;
	printf("searching for element at index %d from position %d:\n", x, k);
	el = arrlist_get_elmt(arrlist, x);
	s = arrlist_search_from(arrlist, k, (void *)el);
	if (ARRLIST_BAD_POSITION == s)
		puts("err: bad positoin");
	
	x = 1923;
	printf("searching for %d not in the list:\n", x);
	el = make_node(x);
	s = arrlist_lsearch(arrlist, (void *)el);
	ARRLIST_SEARCH_RSLT;
	free(el);
	
	puts("\nsorting");
	ARRLIST_SORT;
	ARRLIST_PRINT_LSIZE;
	ARRLIST_PRINT_CSIZE;
	ARRLIST_PRINT_TAIL;
	ARRLIST_PRINT_LIST;
	
	puts("get before the head");
	el = arrlist_get_elmt(arrlist, arrlist->head - 10);
	if (NULL == el)
		puts("not found");
	puts("get after the tail");
	el = arrlist_get_elmt(arrlist, arrlist->tail - 10);
	if (NULL == el)
		puts("not found");
	
	puts("\nbinary searching for the head");
	el = arrlist_get_elmt(arrlist, arrlist->head);
	printf("searching for: %d\n", *el);
	s = arrlist_bsearch(arrlist, (void *)el);
	ARRLIST_SEARCH_RSLT;
	
	x = 3;
	printf("binary searching for element at index %d:\n", x);
	el = arrlist_get_elmt(arrlist, x);
	s = arrlist_bsearch(arrlist, (void *)el);
	ARRLIST_SEARCH_RSLT;
	
	puts("binary searching for tail:");
	el = arrlist_get_elmt(arrlist, arrlist->tail);
	s = arrlist_bsearch(arrlist, (void *)el);
	ARRLIST_SEARCH_RSLT;
	
	x = 1923;
	printf("binary searching for %d not in the list:\n", x);
	el = make_node(x);
	s = arrlist_lsearch(arrlist, (void *)el);
	ARRLIST_SEARCH_RSLT;
	free(el);
	
	ARRLIST_DESTROY;
	
	ARRLIST_INITIALIZE;
	
	puts("\nremoving from empty list");
	ARRLIST_REMOVE(123, (void *)el);
	ARRLIST_PRINT_LIST;
	ARRLIST_SORT;
	puts("\nbinary searching for the head");
	el = arrlist_get_elmt(arrlist, arrlist->head);
	s = arrlist_bsearch(arrlist, (void *)el);
	ARRLIST_SEARCH_RSLT;
	puts("linear searching for tail:");
	el = arrlist_get_elmt(arrlist, arrlist->tail);
	s = arrlist_search_from(arrlist, 3, (void *)el);
	ARRLIST_SEARCH_RSLT;
	
	ARRLIST_DESTROY;
	
	ARRLIST_INITIALIZE;
	
	int i = 1000000;
	printf("\nadding %d elements; adding to tail\n", i);
	for (; i > 0; --i)
	{
		el = make_node(i);
		ARRLIST_INSERT(arrlist->tail, el);
	}
	print_elmnt(arrlist, arrlist->head, NULL);
	print_elmnt(arrlist, arrlist->tail / 2, NULL);
	print_elmnt(arrlist, arrlist->tail, NULL);
	ARRLIST_PRINT_LSIZE;
	ARRLIST_PRINT_CSIZE;

	ARRLIST_SORT;
	
	print_elmnt(arrlist, arrlist->head, NULL);
	print_elmnt(arrlist, arrlist->tail / 2, NULL);
	print_elmnt(arrlist, arrlist->tail, NULL);

	fprintf(stderr, "\nDone\nPress enter to destroy and exit\n");
	getchar();
	ARRLIST_DESTROY;

	puts("Testing is done");
	
	return 0;
}

int compar(const void * key1, const void * key2)
{
	/* compares two array list elements */
	int ** ippa = (int **)key1;
	int ** ippb = (int **)key2;
	int * ipa = *ippa;
	int * ipb = *ippb;
	
	if (*ipa < *ipb)
		return -1;
	else if (*ipa > *ipb)
		return 1;
	else
		return 0;
}

void print_elmnt(Arrlist * list, int n_elmt, void * args)
{
	/* print array list element */
	printf("element no. %d is: %d\n", n_elmt, 
	*((int *)arrlist_get_elmt(list, n_elmt)));
	
	return;
}

int * make_node(int a)
{
	/* make element */
	int * out;
	if ( (out = malloc(sizeof(*out))) == NULL )
	{
		puts("Err: make_node() could not allocate memory");
		exit(EXIT_FAILURE);
	}
	
	*out = a;
	
	return out;
}

void destroy(void * data)
{
	/* free element memory */
	free(data);
	return;
}