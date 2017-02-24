#include <stdio.h>
#include <stdlib.h>
#include "dlist.h"

#define YES	puts("yes")
#define NO	puts("no")

#define DLIST_PRINT_SIZE	printf("list size: %d\n", dlist->size)

#define DLIST_INITIALIZE \
		do \
		{puts("initializing list"); \
		dlist_init(dlist, compar, destroy); \
		DLIST_PRINT_SIZE;} while (0)
		
#define DLIST_DESTROY \
		do \
		{puts("destroying list"); \
		dlist_destroy(dlist); \
		DLIST_PRINT_SIZE;} while (0)
		
#define DLIST_PRINT_FORWARD(element) \
		do \
		{puts("printing forward"); \
		dlist_apply_forward(dlist, (element), print_elmnt, NULL);} while (0)
		
#define DLIST_PRINT_FROM_HEAD \
		do \
		{puts("printing from head"); \
		dlist_apply_forward(dlist, dlist->head, print_elmnt, NULL);} while (0)
		
#define DLIST_PRINT_BACKWARD(element) \
		do \
		{puts("printing backwards"); \
		dlist_apply_backward(dlist, (element), print_elmnt, NULL);} while (0)

#define DLIST_PRINT_FROM_TAIL \
		do \
		{puts("printing from tail"); \
		dlist_apply_backward(dlist, dlist->tail, print_elmnt, NULL);} while (0)
			
#define DLIST_PRINT_ELEMENT(element) \
		do \
		{puts("printring element"); \
		dlist_apply_elmt(dlist, (element), print_elmnt, NULL);} while (0)
			
#define DL_SORT_UP \
		do \
		{puts("sorting up..."); \
		dlist_sort(dlist, DLIST_SORT_UP);} while(0)

#define DL_SORT_DOWN \
		do \
		{puts("sorting down..."); \
		dlist_sort(dlist, DLIST_SORT_DOWN);} while(0)

#define DL_SEARCH_FORWARD(element, key) \
		do \
		{printf("seraching for %d forward\n", *((int*)(key))); \
		((dle = dlist_find_forward(dlist, (element), (key))) != NULL) ?\
		puts("found") : puts("not found");} while (0)
			
#define DL_SEARCH_BACKWARDS(element, key) \
		do \
		{printf("seraching for %d backwards\n", *((int*)(key))); \
		((dle = dlist_find_backward(dlist, (element), (key))) != NULL) ?\
		puts("found") : puts("not found");} while (0)
		
int * make_node(int a);
int compar(const void * key1, const void * key2);
void destroy(void * data);
void print_elmnt(DListElmt * l_element, void * args);

int main(void)
{
	int i, j, * ip;
	DList d_list;
	DList * dlist = &d_list;
	DListElmt * dle;
	
	/* ---------- testing with multiple items ---------- */
	puts("\nTesting with multiple items");
	
	DLIST_INITIALIZE;
	j = 5;
	printf("adding %d items\n", j);
	puts("inserting after head");
	for (i = 0; i < j; ++i)
	{
		ip = make_node(i);
		dlist_ins_next(dlist, dlist->head, ip);
	}
	DLIST_PRINT_SIZE;
	DLIST_PRINT_FROM_HEAD;
	DLIST_DESTROY;

	DLIST_INITIALIZE;
	j = 5;
	printf("adding %d items\n", j);
	puts("inserting after tail");
	for (i = 0; i < j; ++i)
	{
		ip = make_node(i);
		dlist_ins_next(dlist, dlist->tail, ip);
	}
	DLIST_PRINT_SIZE;
	DLIST_PRINT_FROM_HEAD;
	DLIST_DESTROY;
	
	DLIST_INITIALIZE;
	j = 5;
	printf("adding %d items\n", j);
	puts("inserting before tail");
	for (i = 0; i < j; ++i)
	{
		ip = make_node(i);
		dlist_ins_prev(dlist, dlist->tail, ip);
	}
	
	DLIST_PRINT_SIZE;
	DLIST_PRINT_FROM_HEAD;
	DLIST_DESTROY;
	
	DLIST_INITIALIZE;
	j = 5;
	printf("adding %d items\n", j);
	puts("inserting before head");
	for (i = 0; i < j; ++i)
	{
		ip = make_node(i);
		dlist_ins_prev(dlist, dlist->head, ip);
	}
	
	DLIST_PRINT_SIZE;
	DLIST_PRINT_FROM_HEAD;
	DLIST_DESTROY;
	
	DLIST_INITIALIZE;
	j = 5;
	printf("adding %d items\n", j);
	puts("\ninserting after head");
	for (i = 0; i < j; ++i)
	{
		ip = make_node(i);
		dlist_ins_next(dlist, dlist->head, ip);
	}
	DLIST_PRINT_SIZE;
	DLIST_PRINT_FROM_HEAD;
	DLIST_PRINT_FROM_TAIL;
	DL_SORT_UP;
	DLIST_PRINT_FROM_HEAD;
	DL_SORT_DOWN;
	DLIST_PRINT_FROM_HEAD;
	
	puts("print forward from second element");
	DLIST_PRINT_FORWARD(dlist->head->next);
	
	puts("print forward from tail");
	DLIST_PRINT_FORWARD(dlist->tail);
	
	puts("print forward from NULL");
	DLIST_PRINT_FORWARD(NULL);
	
	DLIST_PRINT_FROM_TAIL;
	
	puts("print backwards from second to last");
	DLIST_PRINT_BACKWARD(dlist->tail->prev);
	
	puts("print backwards from head");
	DLIST_PRINT_BACKWARD(dlist->head);
	
	puts("print backwards from NULL");
	DLIST_PRINT_BACKWARD(NULL);
	
	puts("print third element from head");
	DLIST_PRINT_ELEMENT(dlist->head->next->next);
	
	puts("print third element from tail");
	DLIST_PRINT_ELEMENT(dlist->tail->prev->prev);
	
	puts("print NULL");
	DLIST_PRINT_ELEMENT(NULL);
	
	int * k = make_node(2);
	puts("search forward from head");
	DL_SEARCH_FORWARD(dlist->head,(void *)k);
	
	puts("search backwards from head");
	DL_SEARCH_BACKWARDS(dlist->head, (void *)k);
	
	puts("search forward from tail");
	DL_SEARCH_FORWARD(dlist->tail,(void *)k);
	
	puts("search backwards from tail");
	DL_SEARCH_BACKWARDS(dlist->tail, (void *)k);
	
	// dle holds the return value from searches
	DLIST_PRINT_ELEMENT(dle);
	
	puts("searching forward from NULL");
	DL_SEARCH_FORWARD(NULL,(void *)k);
	
	puts("searching backwards from NULL");
	DL_SEARCH_BACKWARDS(NULL,(void *)k);
	
	DLIST_DESTROY;
	
	DLIST_INITIALIZE;
	j = 1;
	printf("adding %d items\n", j);
	puts("inserting after tail");
	for (i = 0; i < j; ++i)
	{
		ip = make_node(i + 1);
		dlist_ins_next(dlist, dlist->tail, ip);
	}
	DLIST_PRINT_SIZE;
	DLIST_PRINT_FROM_HEAD;
	puts("Sorting one item");
	DL_SORT_UP;
	DL_SORT_DOWN;
	DLIST_PRINT_FROM_TAIL;
	
	free(k);
	k = make_node(0);
	printf("searching %d forward from head\n", *k);
	DL_SEARCH_FORWARD(dlist->head,(void *)k);
	printf("searching %d backward from head\n", *k);
	DL_SEARCH_BACKWARDS(dlist->head,(void *)k);
	
	printf("searching %d forward from tail\n", *k);
	DL_SEARCH_FORWARD(dlist->tail,(void *)k);
	printf("searching %d backward from tail\n", *k);
	DL_SEARCH_BACKWARDS(dlist->tail,(void *)k);
	
	*k = 1;
	printf("searching %d forward from head\n", *k);
	DL_SEARCH_FORWARD(dlist->head,(void *)k);
	printf("searching %d backward from head\n", *k);
	DL_SEARCH_BACKWARDS(dlist->head,(void *)k);
	
	printf("searching %d forward from tail\n", *k);
	DL_SEARCH_FORWARD(dlist->tail,(void *)k);
	printf("searching %d backward from tail\n", *k);
	DL_SEARCH_BACKWARDS(dlist->tail,(void *)k);
	
	printf("is %d element head: %d\n", *k, dlist_is_head(dlist, dle));
	printf("is %d element tail: %d\n", *k, dlist_is_tail(dlist, dle));
	
	puts("inserting after head");
	k = make_node(1);
	dlist_ins_next(dlist, dlist->head, k);
	puts("inserting after head");
	k = make_node(1);
	dlist_ins_next(dlist, dlist->head, k);
	DLIST_PRINT_FROM_HEAD;
	
	printf("searching %d forward from head\n", *k);
	DL_SEARCH_FORWARD(dlist->head,(void *)k);
	printf("searching %d forward from found\n", *k);
	DL_SEARCH_FORWARD(dle->next,(void *)k);
	puts("inserting after 2nd found");
	k = make_node(2);
	dlist_ins_next(dlist, dle, k);
	DLIST_PRINT_FROM_HEAD;
	
	printf("searching %d backwards from tail\n", *((int *)dle->data));
	DL_SEARCH_BACKWARDS(dlist->tail,(void *)dle->data);
	printf("searching %d backwards from found\n", *((int *)dle->data));
	DL_SEARCH_BACKWARDS(dle->prev,(void *)dle->data);
	puts("inserting before 2nd found");
	k = make_node(3);
	dlist_ins_prev(dlist, dle, k);
	DLIST_PRINT_FROM_HEAD;
	
	puts("remove found:");
	dlist_remove(dlist, dle, (void *)&k);
	printf("item removed: %d\n", *k);
	free(k);
	DLIST_PRINT_FROM_HEAD;
	
	puts("removing head:");
	dlist_remove(dlist, dlist->head, (void *)&k);
	printf("item removed: %d\n", *k);
	free(k);
	DLIST_PRINT_FROM_HEAD;
	
	puts("removing tail:");
	dlist_remove(dlist, dlist->tail, (void *)&k);
	printf("item removed: %d\n", *k);
	free(k);
	DLIST_PRINT_FROM_HEAD;
	DLIST_PRINT_FROM_TAIL;
	
	puts("removing NULL:");
	dlist_remove(dlist, NULL, (void *)&k);
	DLIST_PRINT_FROM_HEAD;
	
	DLIST_DESTROY;
	
	DLIST_INITIALIZE;
	puts("removing from empty list:");
	dlist_remove(dlist, NULL, (void *)&k);
	
	puts("sorting empty list:");
	DL_SORT_UP;
	DL_SORT_DOWN;
	
	j = 5;
	printf("adding %d items\n", j);
	puts("inserting after tail");
	for (i = 0; i < j; ++i)
	{
		ip = make_node(9);
		dlist_ins_next(dlist, dlist->tail, ip);
	}
	DLIST_PRINT_SIZE;
	DLIST_PRINT_FROM_HEAD;
	
	puts("sorting same items:");
	DL_SORT_UP;
	DL_SORT_DOWN;
	DLIST_PRINT_SIZE;
	DLIST_PRINT_FROM_HEAD;
	
	puts("inserting prev NULL");
	k = make_node(8);
	dlist_ins_prev(dlist, NULL, k);
	DLIST_PRINT_SIZE;
	DLIST_PRINT_FROM_HEAD;
	
	printf("is %s %s?: %d\n", "head", "head", dlist_is_head(dlist, dlist->head));
	printf("is %s %s?: %d\n", "tail", "head", dlist_is_head(dlist, dlist->tail));
	printf("is %s %s?: %d\n", "middle", "head", dlist_is_head(dlist, dlist->head->next));

	printf("is %s %s?: %d\n", "tail", "tail", dlist_is_tail(dlist, dlist->tail));
	printf("is %s %s?: %d\n", "head", "tail", dlist_is_tail(dlist, dlist->head));
	printf("is %s %s?: %d\n", "middle", "tail", dlist_is_tail(dlist, dlist->tail->prev));
	
	DLIST_DESTROY;
	
	puts("initialize with no destroy and compar");
	dlist_init(dlist, NULL, NULL);
	DLIST_PRINT_SIZE;
	
	j = 5;
	printf("adding %d items\n", j);
	puts("inserting after tail");
	for (i = 0; i < j; ++i)
	{
		ip = make_node(i);
		dlist_ins_next(dlist, dlist->tail, ip);
	}
	DLIST_PRINT_SIZE;
	DLIST_PRINT_FROM_HEAD;
	k = make_node(9);
	puts("inserting after head");
	dlist_ins_next(dlist, dlist->head, (void *)k);
	DLIST_PRINT_SIZE;
	DLIST_PRINT_FROM_HEAD;
	
	puts("searching with no compar:");
	DL_SEARCH_FORWARD(dlist->head, (void *)k);
	puts("sorting with no compar:");
	DL_SORT_UP;
	DL_SORT_DOWN;
	DLIST_PRINT_SIZE;
	DLIST_PRINT_FROM_HEAD;
	
	puts("destroying with no destroy:");
	DLIST_DESTROY;
	
	// print last and second to last element
	
	puts("printing the last and second to last inserted elements:");
	printf("%d %d\n", *k, *ip);
	
	puts("initialize with only compar");
	dlist_init(dlist, compar, NULL);
	j = 5;
	printf("adding %d items\n", j);
	puts("inserting after tail");
	for (i = 0; i < j; ++i)
	{
		ip = make_node(i);
		dlist_ins_next(dlist, dlist->tail, ip);
	}
	DLIST_PRINT_SIZE;
	DLIST_PRINT_FROM_HEAD;
	DLIST_DESTROY;
	
	
	puts("initialize with only destroy");
	dlist_init(dlist, NULL, destroy);
	j = 5;
	printf("adding %d items\n", j);
	puts("inserting after tail");
	for (i = 0; i < j; ++i)
	{
		ip = make_node(i);
		dlist_ins_next(dlist, dlist->tail, ip);
	}
	DLIST_PRINT_SIZE;
	DLIST_PRINT_FROM_HEAD;
	
	DLIST_DESTROY;
	
	int foo = 10000;
	printf("Stress testing - %d elements\n", foo);
	
	DLIST_INITIALIZE;
	printf("adding %d items\n", foo);
	puts("inserting after head");
	for (i = 0; i < foo; ++i)
	{
		ip = make_node(i);
		dlist_ins_next(dlist, dlist->head, ip);
	}
	DLIST_PRINT_SIZE;

	puts("head:");
	print_elmnt(dlist->head, NULL);
	puts("second element:");
	print_elmnt(dlist->head->next, NULL);
	puts("tail:");
	print_elmnt(dlist->tail, NULL);
	puts("element no. 3000");
	
	DListElmt * bz = dlist->head;
	for (i = 0; i < 3000; ++i)
		bz = bz->next;

	print_elmnt(bz, NULL);
	
	/*
	// print when redirected
	fprintf(stderr, "Print whole list? (y/n): ");
	int ch;
	
	if ((ch = getchar()) == 'y')
		DLIST_PRINT_FROM_HEAD;
	*/
	
	DL_SORT_UP;
	puts("head:");
	print_elmnt(dlist->head, NULL);
	puts("second element:");
	print_elmnt(dlist->head->next, NULL);
	puts("tail:");
	print_elmnt(dlist->tail, NULL);
	puts("element no. 3000");
	
	bz = dlist->head;
	for (i = 0; i < 3000; ++i)
		bz = bz->next;

	print_elmnt(bz, NULL);
	
	k = make_node(5938);
	DL_SEARCH_FORWARD(dlist->head, (void *)k);
	DLIST_PRINT_ELEMENT(dle);
	
	DLIST_DESTROY;
	
	puts("Testing is complete");
	
	return 0;
}

void print_elmnt(DListElmt * l_element, void * args)
{
	int * ip = (int *)l_element->data;
	
	printf("%d\n", *ip);
	
	return;
}

int * make_node(int a)
{
	int * out;
	if ( (out = malloc(sizeof(*out))) == NULL )
	{
		puts("Err: make_node() could not allocate memory");
		exit(EXIT_FAILURE);
	}
	
	*out = a;
	
	return out;
}

int compar(const void * key1, const void * key2)
{
	const DListElmt * dlk1 = key1;
	const DListElmt * dlk2 = key2;
	const int * k1 = (const int *)dlk1->data;
	const int * k2 = (const int *)dlk2->data;
	
	if (*k1 > *k2)
		return 1;
	else if (*k1 < *k2)
		return -1;
	else
		return 0;
}

void destroy(void * data)
{
	free(data);
	return;
}