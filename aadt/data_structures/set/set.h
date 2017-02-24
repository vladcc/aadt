/* set.h -- public interface for a set implemented as an array list */
/* v1.0 */
#ifndef SET_H
#define SET_H

#include <stdlib.h>
#include <stdbool.h>
#include "arrlist.h"

typedef Arrlist Set;

enum {	SET_SUCCESS = 0,
		SET_MEMB_EXISTS = 1};

/* public interface */
#define set_init(set_pointer, compar, destroy) arrlist_init((set_pointer), (compar), (destroy))
/* 
returns: SET_SUCCESS on success, less than 0 on failure (see arrlist.h)

description: Initializes the set specified by set. Must be called before the set can be used. 
destroy provides a user defined way to free memory when set_destroy is called. destroy can be NULL.
compar must be present for all sets.

complexity: O(1) 
*/

#define set_destroy(set_pointer) arrlist_destroy((set_pointer))
/*
returns: nothing

description: Destroys the set by calling destroy from set_init, provided it's not NULL. 
This function sets the size to 0 and frees the set memory.

complexity: O(1) to O(n)
*/

int set_insert(Set * set, const void * data);
/*
returns: 
SET_SUCCESS on success
SET_MEMB_EXISTS if member is already in the set
less than 0 on failure (see arrlist.h)

description: Inserts a member in the set specified by set. The reference to data should be valid
as long as the member remains in the set.

complexity: O(n)
*/

int set_remove(Set * set, void ** data);
/*
returns: SET_SUCCESS on success, less than 0 on failure (see arrlist.h)

description: Removes the member matching data from the set. On return data points to the data stored
in the member that was removed.

complexity: O(n)
*/

int set_union(Set * setu, Set * set1, Set * set2);
/*
returns: SET_SUCCESS on success, less than 0 on failure (see arrlist.h)

description: Builds a set that is the union of set1 and set2. On return setu contains the union. Because
setu points to data in set1 and set2, the data in set1 and set2 must remain valid throughout the lifetime
of setu. setu should not be initialized prior to the call.

complexity: O(m*n)
*/

int set_intersection(Set * seti, Set * set1, Set * set2);
/*
returns: SET_SUCCESS on success, less than 0 on failure (see arrlist.h)

description: Builds a set that is the intersection of set1 and set2. On return seti contains the intersection. 
Because seti points to data in set1 and set2, the data in set1 and set2 must remain valid throughout the lifetime
of seti. seti should not be initialized prior to the call.

complexity: O(m*n)
*/

int set_difference(Set * setd, Set * set1, Set * set2);
/*
returns: SET_SUCCESS on success, less than 0 on failure (see arrlist.h)

description: Builds a set that is the difference of set1 and set2. On return setd contains the difference. 
Because setd points to data in set1 and set2, the data in set1 and set2 must remain valid throughout the lifetime
of setd. setd should not be initialized prior to the call.

complexity: O(m*n)
*/

int set_is_member(const Set * set, const void * data);
/*
returns: the index of the member if it's in the set (>= 0), less than 0 if not (see arrlist.h)

description: Determines whether the data pointed to by data matches a member of the set.

complexity: O(n)
*/

bool set_is_subset(const Set * set1, const Set * set2);
/*
returns: true if the set1 is a subset of set2, false otherwise

description: Determines whether set1 is a subset of set2.

complexity: O(m*n)
*/

bool set_is_equal(const Set * set1, const Set * set2);
/*
returns: true if the set1 is equal to set2, false otherwise

description: Determines whether set1 is equal to set2.

complexity: O(m*n)
*/

#define set_apply arrlist_apply_all
/*
returns: SET_SUCCESS on success, less than 0 on failuer (see arrlist.h)

description: Macro for arrlist_apply_all, which is a macro for 
arrlist_apply_elmt((list), ARRLIST_APPLY_ALL, (fun), (args)).
Applies fun() to all elements of the set. The function must be of type 
void(*fun)(Arrlist * list, int n_elmt, void * args), args is a void pointer to any fun() arguments.
If none, set it to NULL.

complexity: O(n)
*/

#endif