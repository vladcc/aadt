/* set.c -- an implementation of a set as an array list */
/* v1.0 */
#include <stdlib.h> // for NULL
#include <string.h> // for memset()
#include "set.h"

int set_insert(Set * set, const void * data)
{
	/* insert at the end of the set */
	
	// no duplicates
	if (set_is_member(set, data) >= 0)
		return SET_MEMB_EXISTS;

	// insert at the tail
	return arrlist_insert(set, set->tail, (void *)data);
}

int set_remove(Set * set, void ** data)
{
	/* find data and remove from the set */
	int n = set_is_member(set, *data);
	
	if (n >= 0)
		return arrlist_remove(set, n, data);
	else
		return n;
}

int set_union(Set * setu, Set * set1, Set * set2)
{
	/* get union of set1 and set2 in setu */
	int ret;

	// initialize setu
	if ((ret = set_init(setu, set1->compar, NULL)) < 0)
		return ret; // return error
	
	// insert set1 in setu
	int i;
	for (i = 0; i <= set1->tail; ++i)
	{
		if ((ret = arrlist_insert(setu, setu->tail, set1->cont[i])) < 0)
			return ret; // return error
	}
		
	// insert set2 in setu
	for (i = 0; i <= set2->tail; ++i)
	{
		// add to setu only if not a member
		if (set_is_member(setu, set2->cont[i]) < 0)
		{
			if ((ret = arrlist_insert(setu, setu->tail, set2->cont[i])) < 0)
				return ret; // return error
		}
	}
	
	return ret;
}

int set_intersection(Set * seti, Set * set1, Set * set2)
{
	/* get the intersection of set1 and set2 in seti */
	int ret;
	
	// initialize seti
	if ((ret = set_init(seti, set1->compar, NULL)) < 0)
		return ret; // return error
	
	int i;
	for (i = 0; i <= set1->tail; ++i)
	{
		// add to seti only if member
		if (set_is_member(set2, set1->cont[i]) >= 0)
		{
			if ((ret = arrlist_insert(seti, seti->tail, set1->cont[i])) < 0)
				return ret; // return error
		}
	}

	return ret;
}

int set_difference(Set * setd, Set * set1, Set * set2)
{
	/* get the difference of set1 and set2 in setd */
	int ret;
	
	// initialize setd
	if ((ret = set_init(setd, set1->compar, NULL)) < 0)
		return ret; // return error
	
	int i;
	for (i = 0; i <= set1->tail; ++i)
	{
		// add to seti only if not a member
		if (set_is_member(set2, set1->cont[i]) < 0)
		{
			if ((ret = arrlist_insert(setd, setd->tail, set1->cont[i])) < 0)
				return ret; // return error
		}
	}
	
	return ret;
}

int set_is_member(const Set * set, const void * data)
{
	/* return result from linear search */
	return arrlist_lsearch((Arrlist *)set, data);
}

bool set_is_subset(const Set * set1, const Set * set2)
{
	/* check if set1 is a subset of set2 */
	if (set1->list_size > set2->list_size)
		return false;
	
	int i;
	for (i = 0; i <= set1->tail; ++i)
	{
		// return false if not a member
		if (set_is_member(set2, set1->cont[i]) < 0)
			return false;
	}
	
	return true;
}

bool set_is_equal(const Set * set1, const Set * set2)
{
	/* check if sets are equal */
	if (set1->list_size != set2->list_size)
		return false;
	
	return set_is_subset(set1, set2);
}