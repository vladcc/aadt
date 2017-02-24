/* btree.c -- binary tree implementation */
/* v1.0 */
#include <stdlib.h>
#include <string.h>
#include "btree.h"

void btree_init(BTree * tree, int (*compar)(const void * key1, const void * key2), 
				void (*destroy)(void * data))
{
	/* initialize the tree */
	tree->size = 0;
	tree->compar = compar;
	tree->destroy = destroy;
	tree->root = NULL;
	
	return;
}

void btree_destroy(BTree * tree)
{
	/* destroy the tree */
	
	// remove all nodes
	btree_rem_left(tree, NULL);
	
	// zero out memory
	memset(tree, 0, sizeof(*tree));
	
	return;
}

int btree_ins_left(BTree * tree, BTreeNode * node, const void * data)
{
	/* insert as a left child */
	BTreeNode * new_node, ** position;
	
	if (node == NULL)
	{
		// insert as root only in an empty tree
		if (tree->size > 0)
			return -1;
		
		position = &tree->root;
	}
	else
	{
		// allow insertion at the end of branch
		if (node->left != NULL)
			return -1;
		
		position = &node->left;
	}
	
	// allocate memory
	if ( (new_node = (BTreeNode *)malloc(sizeof(BTreeNode))) == NULL)
		return -1;
	
	// insert the node
	new_node->data = (void *)data;
	new_node->left = NULL;
	new_node->right = NULL;
	*position = new_node;
	
	// update size
	tree->size++;
	
	return 0;
}

int btree_ins_right(BTree * tree, BTreeNode * node, const void * data)
{
	/* insert as a right child */
	BTreeNode * new_node, ** position;
	
	if (node == NULL)
	{
		// insert as root only in an empty tree
		if (tree->size > 0)
			return -1;
		
		position = &tree->root;
	}
	else
	{
		// allow insertion at the end of branch
		if (node->right != NULL)
			return -1;
		
		position = &node->right;
	}
	
	// allocate memory
	if ( (new_node = (BTreeNode *)malloc(sizeof(BTreeNode))) == NULL)
		return -1;
	
	// insert the node
	new_node->data = (void *)data;
	new_node->left = NULL;
	new_node->right = NULL;
	*position = new_node;
	
	// update size
	tree->size++;
	
	return 0;
}

void btree_rem_left(BTree * tree, BTreeNode * node)
{
	/* remove the subtree rooted on the left child node */
	BTreeNode ** position;
	
	// do not remove from empty tree
	if (tree->size == 0)
		return;
	
	// determine where to remove
	if (node == NULL)
		position = &tree->root;
	else
		position = &node->left;
	
	// remove nodes
	if (*position != NULL)
	{
		btree_rem_left(tree, *position);
		btree_rem_right(tree, *position);
		
		// call destroy if any
		if (tree->destroy != NULL)
			tree->destroy((*position)->data);
		
		free(*position);
		*position = NULL;
		
		// update size
		tree->size--;
	}
	
	return;
}

void btree_rem_right(BTree * tree, BTreeNode * node)
{
	/* remove the subtree rooted on the right child node */
	BTreeNode ** position;
	
	// do not remove from empty tree
	if (tree->size == 0)
		return;
	
	// determine where to remove
	if (node == NULL)
		position = &tree->root;
	else
		position = &node->right;
	
	// remove nodes
	if (*position != NULL)
	{
		btree_rem_left(tree, *position);
		btree_rem_right(tree, *position);
		
		// call destroy if any
		if (tree->destroy != NULL)
			tree->destroy((*position)->data);
		
		free(*position);
		*position = NULL;
		
		// update size
		tree->size--;
	}
	
	return;
}

int btree_merge(BTree * merge, BTree * left, BTree * right, void * data)
{
	/* merge left and right into merge */
	
	// initialize the merge tree
	btree_init(merge, left->compar, left->destroy);
	
	// insert data as the root node of merge
	if (btree_ins_left(merge, NULL, data) != 0)
	{
		btree_destroy(merge);
		return -1;
	}
	
	// merge
	merge->root->left = left->root;
	merge->root->right = right->root;
	
	// update size
	merge->size += (left->size + right->size);
	
	// dispose of the original trees
	left->root = NULL;
	left->size = 0;
	right->root = NULL;
	right->size = 0;
	
	return 0;
}