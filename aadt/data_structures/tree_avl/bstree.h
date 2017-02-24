/* bstree.h -- AVL tree public interface */
/* v1.0 */
#ifndef BSTREE_H
#define BSTREE_H

#include "btree.h"

// balance factors
#define AVL_LEFT_HEAVY	1
#define AVL_BALANCED	0
#define AVL_RIGHT_HEAVY	-1

typedef struct AvlNode_ {
	void * data;
	int hidden;
	int factor;
} AvlNode;

// implement the search tree as a binary tree
typedef BTree BsTree;

#define bstree_init(tree, compar, destroy) btree_init((tree), (compar), (destroy))

void bstree_destroy(BsTree * tree);
/*
returns: nothing

description: destroys the binary search tree

complexity: O(n)
*/

int bstree_insert(BsTree * tree, const void * data);
/*
returns: 0 on success, -1 otherwise

description: Recursively insert a node in the tree, adjusting it's balance
on the way.

complexity: O(log n)
*/

int bstree_remove(BsTree * tree, const void * data);
/*
returns: 0 on success, -1 otherwise

description: Lazy removes (hides) a tree node.

complexity: O(log n)
*/

int bstree_lookup(BsTree * tree, const void ** data);
/*
returns: 0 on success, -1 otherwise

description: Looks in tree for a node containing *(*data). If found, *data has
the address of the data in the tree.

complexity: O(log n)
*/

#endif