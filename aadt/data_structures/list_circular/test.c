#include <stdio.h>
#include <stdlib.h>
#include "clist.h"

#define YES	puts("yes")
#define NO	puts("no")

#define CLIST_PRINT_SIZE	printf("list size: %d\n", clist->size)

#define CLIST_INITIALIZE \
		do \
		{puts("initializing list"); \
		clist_init(clist, compar, destroy); \
		CLIST_PRINT_SIZE;} while (0)
			
#define CLIST_DESTROY \
		do \
		{puts("destroying list"); \
		clist_destroy(clist); \
		CLIST_PRINT_SIZE;} while (0)
		
#define CLIST_PRINT_ELEMENT(element) \
		do \
		{puts("printing element:"); \
		clist_apply_elmt(clist, (element), print_elmnt, NULL);} while (0)
			
#define CLIST_PRINT_LIST \
		do \
		{puts("printing list:"); \
		clist_apply_all(clist, print_elmnt, NULL);} while (0) 
		
#define CL_SORT_UP \
		do \
		{puts("sorting up..."); \
		clist_sort(clist, CLIST_SORT_UP);} while(0)

#define CL_SORT_DOWN \
		do \
		{puts("sorting down..."); \
		clist_sort(clist, CLIST_SORT_DOWN);} while(0)

#define CL_FIND(key) \
		do \
		{printf("searching for %d from head\n", *((int*)(key))); \
		((cle = clist_find(clist, (key))) != NULL) ?\
		puts("found") : puts("not found");} while (0)
			
#define CL_FIND_NEXT(element, key) \
		do \
		{printf("searching for next %d\n", *((int*)(key))); \
		((cle = clist_find_from(clist, (element)->next, (key))) != NULL) ?\
		puts("found") : puts("not found");} while (0)
			
int * make_node(int a);
int compar(const void * key1, const void * key2);
void destroy(void * data);
void print_elmnt(CListElmt * l_element, void * args);

int main(void)
{
	int i, j, * ip, * k;
	CList c_list;
	CList * clist = &c_list;
	CListElmt * cle;
	
	CLIST_INITIALIZE;
	CLIST_PRINT_LIST;
	CL_SORT_UP;
	CL_SORT_DOWN;
	
	k = make_node(1);
	CL_FIND((void *)k);
	
	j = 5;
	for (i = 0; i < j; ++i)
	{
		ip = make_node(i);
		clist_ins_next(clist, clist->head, ip);
	}
	CLIST_PRINT_SIZE;
	CLIST_PRINT_LIST;
	CL_FIND((void *)k);
	CL_FIND_NEXT(cle, (void *)k);
	
	CL_SORT_UP;
	CLIST_PRINT_LIST;
	CL_SORT_DOWN;
	CLIST_PRINT_LIST;
	
	puts("inserting after head:");
	clist_ins_ahead(clist, (void *)k);
	CLIST_PRINT_SIZE;
	CLIST_PRINT_LIST;
	
	CL_FIND((void *)k);
	CLIST_PRINT_ELEMENT(cle);
	CL_FIND_NEXT(cle, (void *)k);
	
	printf("inserting after second %d\n", *k);
	int * z = k;
	k = make_node(9);
	clist_ins_next(clist, cle, (void *)k);
	CLIST_PRINT_SIZE;
	CLIST_PRINT_LIST;
	CL_FIND_NEXT(cle, (void *)z);
	
	CL_SORT_UP;
	CLIST_PRINT_LIST;
	
	CL_FIND((void *)z);
	CL_FIND_NEXT(cle, (void *)z);
	int * rmd;
	printf("removing after second %d\n", *z);
	clist_rem_next(clist, cle, (void *)&rmd);
	free(rmd);
	CLIST_PRINT_SIZE;
	CLIST_PRINT_LIST;
	
	puts("removing after head");
	clist_rem_ahead(clist, (void *)&rmd);
	free(rmd);
	CLIST_PRINT_SIZE;
	CLIST_PRINT_LIST;
	
	z = make_node(1);
	k = make_node(8);
	CL_FIND((void *)z);
	puts("inserting after it");
	clist_ins_next(clist, cle, (void *)k);
	CLIST_PRINT_SIZE;
	CLIST_PRINT_LIST;
	printf("is head head?: %d\n", clist_is_head(clist, clist->head));
	printf("is something else head?: %d\n", clist_is_head(clist, cle));
	CL_FIND_NEXT(cle, (void *)z);
	
	CLIST_DESTROY;
	
	CLIST_INITIALIZE;
	z = make_node(1);
	k = make_node(8);
	clist_ins_next(clist, clist->head, k);
	CLIST_PRINT_SIZE;
	CLIST_PRINT_LIST;
	CL_SORT_DOWN;
	CL_SORT_UP;
	CL_FIND(k);
	CL_FIND(z);
	CLIST_PRINT_SIZE;
	CLIST_PRINT_LIST;
	
	CLIST_DESTROY;
	
	puts("initializing list no destroy");
	clist_init(clist, compar, NULL);
	puts("adding items:");
	j = 5;
	for (i = 0; i < j; ++i)
	{
		ip = make_node(i);
		clist_ins_next(clist, clist->head, ip);
	}
	CLIST_PRINT_SIZE;
	CLIST_PRINT_LIST;

	CLIST_DESTROY;
	
	puts("initializing list no compar");
	clist_init(clist, NULL, destroy);
	puts("adding items:");
	j = 5;
	for (i = 0; i < j; ++i)
	{
		ip = make_node(i);
		clist_ins_next(clist, clist->head, ip);
	}
	CLIST_PRINT_SIZE;
	CLIST_PRINT_LIST;
	z = make_node(1);
	CL_FIND(z);
	CL_SORT_UP;
	CL_SORT_DOWN;
	CLIST_PRINT_SIZE;
	CLIST_PRINT_LIST;
	
	CLIST_DESTROY;
	
	puts("initializing list no compar and no destroy");
	clist_init(clist, NULL, NULL);
	puts("adding items:");
	j = 5;
	for (i = 0; i < j; ++i)
	{
		ip = make_node(i);
		clist_ins_next(clist, clist->head, ip);
	}
	CLIST_PRINT_SIZE;
	CLIST_PRINT_LIST;
	z = make_node(1);
	CL_FIND(z);
	CL_SORT_UP;
	CL_SORT_DOWN;
	CLIST_PRINT_SIZE;
	CLIST_PRINT_LIST;
	
	CLIST_DESTROY;
	
	int bz = 10000;
	printf("testing with %d elements\n", bz);
	CLIST_INITIALIZE;
	puts("adding items:");
	for (i = 0; i < bz; ++i)
	{
		ip = make_node(i);
		clist_ins_next(clist, clist->head, ip);
	}
	
	puts("printing head:");
	CLIST_PRINT_ELEMENT(clist->head);
	bz = 3000;
	printf("printing %d element\n", bz);
	CListElmt * elp = clist->head;
	for (i = 0; i < bz; ++i)
		elp = elp->next;
	
	CLIST_PRINT_ELEMENT(elp);
	
	puts("printing last element");
	elp = clist->head;
	while (elp->next != clist->head)
		elp = elp->next;
	
	CLIST_PRINT_ELEMENT(elp);
	
	CL_SORT_UP;
	
	puts("printing head:");
	CLIST_PRINT_ELEMENT(clist->head);
	printf("printing %d element\n", bz);
	elp = clist->head;
	for (i = 0; i < bz; ++i)
		elp = elp->next;
	
	CLIST_PRINT_ELEMENT(elp);
	
	puts("printing last element");
	elp = clist->head;
	while (elp->next != clist->head)
		elp = elp->next;
	
	CLIST_PRINT_ELEMENT(elp);
	
	CL_SORT_DOWN;
	
	puts("printing head:");
	CLIST_PRINT_ELEMENT(clist->head);
	printf("printing %d element\n", bz);
	elp = clist->head;
	for (i = 0; i < bz; ++i)
		elp = elp->next;
	
	CLIST_PRINT_ELEMENT(elp);
	
	puts("printing last element");
	elp = clist->head;
	while (elp->next != clist->head)
		elp = elp->next;
	
	CLIST_PRINT_ELEMENT(elp);
	
	CLIST_DESTROY;
	
	puts("Testing is done");
	
	return 0;
}

void print_elmnt(CListElmt * l_element, void * args)
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
	const CListElmt * clk1 = key1;
	const CListElmt * clk2 = key2;
	const int * k1 = (const int *)clk1->data;
	const int * k2 = (const int *)clk2->data;
	
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