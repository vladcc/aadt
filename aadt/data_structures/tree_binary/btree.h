/* btree.h -- public interface for a binary tree */
/* v1.0 */
#ifndef BTREE_H
#define BTREE_H

#include <stdlib.h> // for NULL

/* the tree node structure */
typedef struct BTreeNode_ {
	void * data;
	struct BTreeNode_ * left;
	struct BTreeNode_ * right;
} BTreeNode;

/* the tree structure */
typedef struct BTree_ {
	int size;
	int (*compar)(const void * key1, const void * key2);
	void (*destroy)(void * data);
	BTreeNode * root;
} BTree;

void btree_init(BTree * tree, int (*compar)(const void * key1, const void * key2), 
				void (*destroy)(void * data));
/*
returns: nothing

description: Initializies the binary tree specified by tree. destroy provides a facility
for freeing the data held in the tree if the users whishes to do so. If not, destroy should
be set to NULL.

complexity: O(1)
*/

void btree_destroy(BTree * tree);
/*
returns: nothing

description: Destroys the binary tree specified by tree. If a destroy funcion was provided, the
memory for the data held in the nodes is deallocated.

complexity: O(n)
*/

int btree_ins_left(BTree * tree, BTreeNode * node, const void * data);
/*
returns: 0 on success, -1 on failure

description: Inserts a node as the left child of node. If there already is a left child,
-1 is returned. If node is NULL, the new node is inserted as the root node. This can happen
only if the tree is empty.

complexity: O(1)
*/

int btree_ins_right(BTree * tree, BTreeNode * node, const void * data);
/*
returns: 0 on success, -1 on failure

description: Works the same as btree_ins_left() but on the right.

complexity: O(1)
*/

void btree_rem_left(BTree * tree, BTreeNode * node);
/*
returns: nothing

description: Removes the subtree rooted at the left child of node. If node is NULL, all nodes
in the tree are removed. destroy is called for every node, provided destroy is not NULL.

complexity: O(n)
*/

void btree_rem_right(BTree * tree, BTreeNode * node);
/*
returns: nothing

description: Works the same as btree_rem_left() but on the right.

complexity: O(n)
*/

int btree_merge(BTree * merge, BTree * left, BTree * right, void * data);
/*
returns: nothing

description: Merges the two binary trees specified by left and right into the single binary
tree merge. After the merge, data is at the root of the new tree. left and right are left
as if btree_destroy() has been called on them.

complexity: O(1)
*/

#endif