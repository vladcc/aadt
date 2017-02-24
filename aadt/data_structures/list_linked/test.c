#include <stdio.h>
#include <stdlib.h>
#include "list.h"

// recurring operations
#define PRINT_LIST_SIZE 	printf("list size is: %d\n", alphabet->size)
#define PRINT_FULL_LIST		list_apply_all(alphabet, print_node, NULL)
#define DESTROY_LIST		list_destroy(alphabet)
#define INITIALIZE_LIST		list_init(alphabet, compar_node, free_alpha)
#define PRINT_LIST_HEAD		list_apply_elmt(alphabet, alphabet->head, print_node, NULL)
#define PRINT_LIST_TAIL		list_apply_elmt(alphabet, alphabet->tail, print_node, NULL)

// something to insert in the list
typedef struct alpha {
	int letter;
	int number;
} alpha;

void free_alpha(void * a);
int compar_node(const void * key1, const void * key2);
void print_node(ListElmt * le, void * args);
alpha * make_alpha(char l, int n);

int main(void)
{
	int i;
	alpha * ap;
	List l_alphabet;
	List * alphabet = &l_alphabet;

	/* -------------------- test with one item -------------------- */
	puts("Test with one item");
	puts("initializing list:");
	INITIALIZE_LIST;
	PRINT_LIST_SIZE;
	
	puts("sorting empty list...");
	if (list_sort(alphabet, LIST_SORT_UP) < 0)
		puts("Error");
	
	puts("populating list with 1 item:");
	for (i = 'a'; i <= 'a'; ++i)
	{
		ap = make_alpha(i, i);
		list_ins_first(alphabet, ap);
	}
	PRINT_LIST_SIZE;
	
	puts("printing whole list:");
	PRINT_FULL_LIST;
	
	puts("sorting list...");
	if (list_sort(alphabet, LIST_SORT_UP) < 0)
		puts("Error");
	
	PRINT_FULL_LIST;
	
	puts("destroying list:");
	DESTROY_LIST;
	PRINT_LIST_SIZE;
	
	/* -------------------- test with two items -------------------- */
	puts("\nTest with two items");
	puts("initializing list:");
	INITIALIZE_LIST;
	
	puts("populating list with 2 items:");
	for (i = 'a'; i <= 'b'; ++i)
	{
		ap = make_alpha(i, i);
		list_ins_first(alphabet, ap);
	}
	PRINT_LIST_SIZE;
	
	puts("printing whole list:");
	PRINT_FULL_LIST;
	
	puts("sorting list...");
	if (list_sort(alphabet, LIST_SORT_UP) < 0)
		puts("Error");
	
	PRINT_FULL_LIST;
	
	puts("destroying list:");
	DESTROY_LIST;
	PRINT_LIST_SIZE;
	
	/* -------------------- test with same items -------------------- */
	puts("\nTest with same items");
	puts("initializing list:");
	INITIALIZE_LIST;
	PRINT_LIST_SIZE;
	
	puts("populating list with same items:");
	for (i = 'a'; i <= 'e'; ++i)
	{
		ap = make_alpha('a', 'a');
		list_ins_first(alphabet, ap);
	}
	PRINT_LIST_SIZE;
	
	puts("printing whole list:");
	PRINT_FULL_LIST;
	
	puts("sorting list up...");
	if (list_sort(alphabet, LIST_SORT_UP) < 0)
		puts("Error");
	PRINT_FULL_LIST;
	
	puts("sorting list down...");
	if (list_sort(alphabet, LIST_SORT_DOWN) < 0)
		puts("Error");
	PRINT_FULL_LIST;
	
	puts("destroying list:");
	DESTROY_LIST;
	PRINT_LIST_SIZE;
	
	/* -------------------- test with different items -------------------- */
	puts("\nTest with different items");
	puts("initializing list:");
	INITIALIZE_LIST;
	PRINT_LIST_SIZE;
	
	puts("populating list:");
	for (i = 'a'; i <= 'e'; ++i)
	{
		ap = make_alpha(i, i);
		list_ins_first(alphabet, ap);
	}
	PRINT_LIST_SIZE;
	
	puts("printing head:");
	PRINT_LIST_HEAD;
	
	puts("printing tail:");
	PRINT_LIST_TAIL;
	
	puts("printing whole list:");
	PRINT_FULL_LIST;
	
	puts("add one:");
	i = 1;
	list_apply_all(alphabet, print_node, &i);
	
	/* -------------------- test inserting -------------------- */
	puts("\nTest inserting");
	puts("insert after head");
	ap = make_alpha('x', 'x');
	list_ins_next(alphabet, alphabet->head, ap);
	PRINT_LIST_SIZE;
	PRINT_FULL_LIST;
	
	puts("insert in the middle");
	ap = make_alpha('y', 'y');
	list_ins_next(alphabet, alphabet->head->next->next, ap);
	PRINT_LIST_SIZE;
	PRINT_FULL_LIST;
	
	puts("insert after tail");
	ap = make_alpha('f', 'f');
	list_ins_next(alphabet, alphabet->tail, ap);
	PRINT_LIST_SIZE;
	PRINT_FULL_LIST;
	
	puts("sorting list up...");
	list_sort(alphabet, LIST_SORT_UP);
	PRINT_FULL_LIST;
	
	puts("sorting list down...");
	list_sort(alphabet, LIST_SORT_DOWN);
	PRINT_FULL_LIST;
	
	puts("sorting list with other value...");
	if (list_sort(alphabet, 2) < 0)
		puts("Error");
	
	/* -------------------- test removing -------------------- */
	puts("\nTest removing");
	puts("removing head");
	list_rem_first(alphabet, (void**)&ap);
	free_alpha(ap);
	PRINT_LIST_SIZE;
	PRINT_FULL_LIST;
	
	puts("removing fourth element");
	list_rem_next(alphabet, alphabet->head->next->next, (void**)&ap);
	free_alpha(ap);
	PRINT_LIST_SIZE;
	PRINT_FULL_LIST;
	
	puts("removing after tail");
	if (list_rem_next(alphabet, alphabet->tail, (void**)&ap) < 0)
		puts("Error");
	else
		free_alpha(ap);
	
	PRINT_LIST_SIZE;
	PRINT_FULL_LIST;
	
	/* -------------------- test searching -------------------- */
	puts("\nTest searching");
	ap = make_alpha('f', 'f');
	puts("Searching for 'f' from the start");
	ListElmt * el = list_find(alphabet, ap);
	if (NULL == el)
		puts("Element not found");
	else
		puts("Element found");
	
	puts("Searchig for second 'f'");
	el = list_find_from(alphabet, el->next, ap);
	if (NULL == el)
		puts("Element not found");
	else
		puts("Element found");
	
	puts("Adding 'o' after second 'f'");
	ap->letter = 'o';
	ap->number = 'o';
	list_ins_next(alphabet, el, ap);
	PRINT_LIST_SIZE;
	PRINT_FULL_LIST;
	
	/* from here on as has 'p', and ap has 'o' */
	/* don't mind the names */
	alpha * as = make_alpha('p', 'p');
	puts("Searching for 'p' not in the list");
	el = list_find(alphabet, as);
	if (NULL == el)
		puts("Element not found");
	else
		puts("Element found");
	
	puts("destroying list:");
	DESTROY_LIST;
	PRINT_LIST_SIZE;
	
	puts("initializing list:");
	INITIALIZE_LIST;
	PRINT_LIST_SIZE;
	
	ap = make_alpha('o', 'o');
	as = make_alpha('p', 'p');
	puts("Searching empty list for 'p'");
	el = list_find(alphabet, as);
	if (NULL == el)
		puts("Element not found");
	else
		puts("Element found");
	
	puts("Inserting 'p'");
	list_ins_first(alphabet, as);
	PRINT_LIST_SIZE;
	PRINT_FULL_LIST;
	puts("Searching list for 'p'");
	el = list_find(alphabet, as);
	if (NULL == el)
		puts("Element not found");
	else
		puts("Element found");
	
	puts("Changing 'p' to 'o'");
	list_rem_first(alphabet, (void **)&as);
	list_ins_first(alphabet, ap);
	PRINT_LIST_SIZE;
	PRINT_FULL_LIST;
	puts("Searching list for 'p'");
	
	el = list_find(alphabet, as);
	if (NULL == el)
		puts("Element not found");
	else
		puts("Element found");
	
	puts("Adding 'p' to front");
	list_ins_first(alphabet, as);
	PRINT_LIST_SIZE;
	PRINT_FULL_LIST;
	
	/* -------------------- test is_head is_tail macros -------------------- */
	puts("\nTest is_head is_tail macros");
	puts("is list head head?");
	if (list_is_head(alphabet, alphabet->head))
		puts("yes");
	else
		puts("no");
	
	puts("is list tail head?");
	if (list_is_head(alphabet, alphabet->tail))
		puts("yes");
	else
		puts("no");
	
	puts("is list 2nd element head?");
	if (list_is_head(alphabet, alphabet->head->next))
		puts("yes");
	else
		puts("no");
	
	puts("is list tail tail?");
	if (list_is_tail(alphabet, alphabet->tail))
		puts("yes");
	else
		puts("no");
	
	puts("is list head tail?");
	if (list_is_tail(alphabet, alphabet->head))
		puts("yes");
	else
		puts("no");
	
	puts("is list 2nd element tail?");
	if (list_is_tail(alphabet, alphabet->head->next))
		puts("yes");
	else
		puts("no");
	
	puts("adding third element");
	ap = make_alpha('l', 'l');
	list_ins_first(alphabet, ap);
	PRINT_LIST_SIZE;
	PRINT_FULL_LIST;
	
	puts("is list 2nd element tail?");
	if (list_is_tail(alphabet, alphabet->head->next))
		puts("yes");
	else
		puts("no");
	
	puts("is 3rd element tail?");
	if (list_is_tail(alphabet, alphabet->head->next->next))
		puts("yes");
	else
		puts("no");
	
	puts("is tail tail?");
	if (list_is_tail(alphabet, alphabet->tail))
		puts("yes");
	else
		puts("no");
	
	puts("destroying list:");
	DESTROY_LIST;
	PRINT_LIST_SIZE;
	
	/* -------------------- test with a bunch of data -------------------- */
	puts("\nStress test");
	puts("initializing list:");
	INITIALIZE_LIST;
	PRINT_LIST_SIZE;
	puts("adding elements...");
	for (i = 1; i <= 10000; ++i)
	{
		ap = make_alpha(' ', i);
		list_ins_first(alphabet, ap);
	}
	PRINT_LIST_SIZE;
	puts("head:");
	PRINT_LIST_HEAD;
	puts("tail");
	PRINT_LIST_TAIL;
	
	puts("sorting list up...");
	list_sort(alphabet, LIST_SORT_UP);
	puts("head:");
	PRINT_LIST_HEAD;
	puts("tail");
	PRINT_LIST_TAIL;
	
	puts("find 5");
	ap = make_alpha(' ', 5);
	if (list_find(alphabet, ap) == NULL)
		puts("not found");
	else
		puts("found");
	
	puts("destroying list");
	DESTROY_LIST;
	PRINT_LIST_SIZE;
	puts("\nTesting is done");
	
	return 0;
}

int compar_node(const void * key1, const void * key2)
{
	/* compares two list nodes of type alpha */
	const ListElmt * el1 = (ListElmt *)key1;
	const ListElmt * el2 = (ListElmt *)key2;
	const alpha * k1 = (alpha*)el1->data;
	const alpha * k2 = (alpha*)el2->data;
	
	if (k1->number < k2->number)
		return -1;
	else if (k1->number > k2->number)
		return 1;
	else
		return 0;
}

void print_node(ListElmt * le, void * args)
{
	/* prints an alpha node, optionally apllying an argument */
	alpha * a = (alpha *)le->data;
	if (args != NULL)
	{
		a->letter += *((int*)args);
		a->number += *((int*)args);
	}
	
	printf("%c - %d\n", a->letter, a->number);
	
	return;
}

void free_alpha(void * a)
{	
	/* frees node memory */
	free(a);
	return;
}

alpha * make_alpha(char l, int n)
{
	/* makes an alpha node */
	alpha * out;
	
	if ((out = malloc(sizeof(*out))) == NULL)
	{
		puts("Err: make_alpha() could not allocate memory");
		exit(EXIT_FAILURE);
	}
	
	out->letter = l;
	out->number = n;
	
	return out;
}