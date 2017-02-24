#include <stdio.h>
#include <stdlib.h>
#include "set.h"

#define SET_PRINT_SIZE(set)	printf("set size: %d\n", (set)->list_size)

#define SET_INITIALIZE(set) \
		do \
		{puts("initializing set"); \
		set_init((set), compar, destroy); \
		SET_PRINT_SIZE((set));} while (0)

#define SET_DESTROY(set) \
		do \
		{puts("destroying set"); \
		set_destroy((set)); \
		SET_PRINT_SIZE((set));} while (0)			

#define SET_PRINT(set) \
		do \
		{puts("printing set..."); \
		if (set_apply(set, print_elmnt, NULL) < 0) \
			puts("empty set");} while (0)
				
int * make_node(int a);
void destroy(void * data);
int compar(const void * key1, const void * key2);
void print_elmnt(Arrlist * list, int n_elmt, void * args);

int main(void)
{
	Set set_1, set_2, set_d, set_u, set_i;
	Set * set1 = &set_1;
	Set * set2 = &set_2;
	Set * setd = &set_d;
	Set * setu = &set_u;
	Set * seti = &set_i;
	int * el, i, size1 = 5;
	
	SET_INITIALIZE(set1);
	
	printf("adding %d elements\n", size1);
	
	for (i = 0; i < size1; ++i)
		set_insert(set1, make_node(5 + i));
	
	SET_PRINT(set1);
	
	printf("inserting an existing element %d\n", size1);
	i = set_insert(set1, (el = make_node(size1)));
	if (SET_MEMB_EXISTS == i)
	{
		puts("member exists");
		free(el);
	}	
	else if (i < 0)
	{
		puts("error while inserting");
		free(el);
	}
		
	
	size1 *= 2;
	printf("inserting a non-existing element %d\n", size1);
	i = set_insert(set1, (el = make_node(size1)));
	if (SET_MEMB_EXISTS == i)
	{
		puts("member exists");
		free(el);
	}
	else if (i < 0)
	{
		puts("error while inserting");
		free(el);
	}
	
	SET_PRINT(set1);
	
	puts("\ngetting union with the same set");
	set_union(setu, set1, set1);
	puts("printing union");
	SET_PRINT(setu);
	SET_DESTROY(setu);
	
	puts("\ngetting intersection of the same set");
	set_intersection(seti, set1, set1);
	puts("printing intersection");
	SET_PRINT(seti);
	SET_DESTROY(seti);
	
	puts("\ngetting difference of the same set");
	set_difference(setd, set1, set1);
	puts("printing difference");
	SET_PRINT(setd);
	SET_DESTROY(setd);
	
	el = make_node(6);
	int * el1 = el;
	printf("\nremoving existing element %d\n", *el);
	if (set_remove(set1, (void **)&el) == SET_SUCCESS)
		printf("success: removed %d\n", *el);
	
	free(el);
	free(el1);
	
	SET_PRINT(set1);
	
	el = make_node(17);
	printf("\nremoving non-existing element %d\n", *el);
	if (set_remove(set1, (void **)&el) < SET_SUCCESS)
		puts("err: elements not in set");
	
	free(el);
	
	SET_PRINT(set1);
	
	puts("init set2");
	SET_INITIALIZE(set2);
	el = make_node(17);
	printf("\nremoving %d from empty set\n", *el);
	if (set_remove(set1, (void **)&el) < SET_SUCCESS)
		puts("err: elements not in set");
	
	free(el);
	SET_PRINT(set2);
	
	puts("\ngetting union with empty set");
	set_union(setu, set1, set2);
	puts("printing union");
	SET_PRINT(setu);
	SET_DESTROY(setu);
	
	puts("\ngetting intersection with empty set");
	set_intersection(seti, set1, set2);
	puts("printing intersection");
	SET_PRINT(seti);
	SET_DESTROY(seti);
	
	puts("\ngetting difference with empty set");
	set_difference(setd, set1, set2);
	puts("printing difference");
	SET_PRINT(setd);
	SET_DESTROY(setd);
	
	printf("sets equal?: %d\n", set_is_equal(set1, set2));
	printf("set subset?: %d\n", set_is_subset(set1, set2));
	
	puts("\ndestroy set2");
	SET_DESTROY(set2);
	
	puts("\nreinit, make different");
	SET_INITIALIZE(set2);
	printf("adding %d elements\n", size1);
	
	for (i = 0; i < size1; ++i)
		set_insert(set2, make_node(30 + i));
	
	SET_PRINT(set2);
	
	puts("\ngetting union with different set");
	set_union(setu, set1, set2);
	puts("printing union");
	SET_PRINT(setu);
	SET_DESTROY(setu);
	
	puts("\ngetting intersection with different set");
	set_intersection(seti, set1, set2);
	puts("printing intersection");
	SET_PRINT(seti);
	SET_DESTROY(seti);
	
	puts("\ngetting difference with different set");
	set_difference(setd, set1, set2);
	puts("printing difference");
	SET_PRINT(setd);
	SET_DESTROY(setd);
	
	printf("sets equal?: %d\n", set_is_equal(set1, set2));
	printf("set subset?: %d\n", set_is_subset(set1, set2));
	
	puts("\ndestroy set2");
	SET_DESTROY(set2);
	
	puts("\nreinit, make with common");
	SET_INITIALIZE(set2);
	size1 = 5;
	printf("adding %d elements\n", size1);
	for (i = 0; i < size1; ++i)
		set_insert(set2, make_node(7 + i));
	
	SET_PRINT(set2);
	SET_PRINT(set1);
	
	puts("\ngetting union");
	set_union(setu, set1, set2);
	puts("printing union");
	SET_PRINT(setu);
	SET_DESTROY(setu);
	
	puts("\ngetting intersection");
	set_intersection(seti, set1, set2);
	puts("printing intersection");
	SET_PRINT(seti);
	SET_DESTROY(seti);
	
	puts("\ngetting difference");
	set_difference(setd, set1, set2);
	puts("printing difference");
	SET_PRINT(setd);
	
	printf("is 5 member of the difference set?: %d\n", 
	(set_is_member(setd, (el = make_node(5))) == 0) ? 1 : 0);
	free(el);
	printf("is 7 member of the difference set?: %d\n", 
	(set_is_member(setd, (el = make_node(7))) == 0) ? 1 : 0);
	free(el);
	SET_DESTROY(setd);
	
	printf("sets equal?: %d\n", set_is_equal(set1, set2));
	printf("set subset?: %d\n", set_is_subset(set1, set2));
	
	puts("\ndestroy set2");
	SET_DESTROY(set2);
	
	puts("\nreinit, as superset to set1");
	SET_INITIALIZE(set2);
	size1 = 10;
	printf("adding %d elements\n", size1);
	for (i = 0; i < size1; ++i)
		set_insert(set2, make_node(5 + i));
	
	SET_PRINT(set2);
	SET_PRINT(set1);
	
	printf("set1 subset?: %d\n", set_is_subset(set1, set2));
	printf("is 5 member of set1?: %d\n", 
	(set_is_member(set1, (el = make_node(5))) == 0) ? 1 : 0);
	free(el);
	printf("is 17 member of the set1?: %d\n", 
	(set_is_member(set1, (el = make_node(17))) == 0) ? 1 : 0);
	free(el);
	
	puts("\ndestroy sets");
	SET_DESTROY(set2);
	SET_DESTROY(set1);
	
	puts("\nreinit, equal");
	SET_INITIALIZE(set1);
	SET_INITIALIZE(set2);
	size1 = 5;
	printf("adding %d elements\n", size1);
	for (i = 0; i < size1; ++i)
	{
		set_insert(set1, make_node(5 + i));
		set_insert(set2, make_node(5 + i));
	}
		
	SET_PRINT(set2);
	SET_PRINT(set1);
	
	puts("\ngetting union");
	set_union(setu, set1, set2);
	puts("printing union");
	SET_PRINT(setu);
	SET_DESTROY(setu);
	
	puts("\ngetting intersection");
	set_intersection(seti, set1, set2);
	puts("printing intersection");
	SET_PRINT(seti);
	SET_DESTROY(seti);
	
	puts("\ngetting difference");
	set_difference(setd, set1, set2);
	puts("printing difference");
	SET_PRINT(setd);
	
	printf("set1 subset?: %d\n", set_is_subset(set1, set2));
	printf("sets equal?: %d\n", set_is_equal(set1, set2));
	
	SET_DESTROY(set1);
	SET_DESTROY(set2);
	
	puts("\nTesting is done");
	
	return 0;
}

int compar(const void * key1, const void * key2)
{
	/* compares two array list/set elements */
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