/* bstree.c -- AVL tree implementation */
/* v1.0 */
#include <stdlib.h>
#include <string.h>
#include "btree.h"
#include "bstree.h"

#define is_eob(node) (NULL == (node))

static void destroy_left(BsTree * tree, BTreeNode * node);
static void destroy_right(BsTree * tree, BTreeNode * node);
static void rotate_left(BTreeNode ** node);
static void rotate_right(BTreeNode ** node);
static int insert(BsTree * tree, BTreeNode ** node, const void * data,
				  int * balanced);
static int hide(BsTree * tree, BTreeNode * node, const void * data);
static int lookup(BsTree * tree, BTreeNode * node, const void ** data);

static void rotate_left(BTreeNode ** node)
{
	/* LL and LR rotation */
	BTreeNode * left, * grandchild;
	
	left = (*node)->left;
	
	if (AVL_LEFT_HEAVY == ((AvlNode *)left->data)->factor)
	{
		// perform LL rotation
		(*node)->left = left->right;
		left->right = *node;
		((AvlNode *)(*node)->data)->factor = AVL_BALANCED;
		((AvlNode *)left->data)->factor = AVL_BALANCED;
		*node = left;
	}
	else
	{
		// perform LR rotation
		grandchild = left->right;
		left->right = grandchild->left;
		grandchild = left;
		(*node)->left = grandchild->right;
		grandchild->right = *node;
		
		switch (((AvlNode *)grandchild->data)->factor)
		{
			case AVL_LEFT_HEAVY:
				((AvlNode *)(*node)->data)->factor = AVL_RIGHT_HEAVY;
				((AvlNode *)left->data)->factor = AVL_BALANCED;
				break;
			case AVL_BALANCED:
				((AvlNode *)(*node)->data)->factor = AVL_BALANCED;
				((AvlNode *)left->data)->factor = AVL_BALANCED;
				break;
			case AVL_RIGHT_HEAVY:
				((AvlNode *)(*node)->data)->factor = AVL_BALANCED;
				((AvlNode *)left->data)->factor = AVL_LEFT_HEAVY;
				break;
			default:
				break;
		}
		((AvlNode *)grandchild->data)->factor = AVL_BALANCED;
		*node = grandchild;
	}
	
	return;
}

static void rotate_right(BTreeNode ** node)
{
	/* RR and RL rotation */
	BTreeNode * right, * grandchild;
	
	right = (*node)->right;
	
	if (AVL_RIGHT_HEAVY == ((AvlNode *)right->data)->factor)
	{
		// perform RR rotation
		(*node)->right = right->left;
		right->left = *node;
		((AvlNode *)(*node)->data)->factor = AVL_BALANCED;
		((AvlNode *)right->data)->factor = AVL_BALANCED;
		*node = right;
	}
	else
	{
		// perform RL rotation
		grandchild = right->left;
		right->left = grandchild->right;
		grandchild->right = right;
		(*node)->right = grandchild->left;
		grandchild->left = *node;
		
		switch (((AvlNode *)grandchild->data)->factor)
		{
			case AVL_LEFT_HEAVY:
				((AvlNode *)(*node)->data)->factor = AVL_BALANCED;
				((AvlNode *)right->data)->factor = AVL_RIGHT_HEAVY;
				break;
			case AVL_BALANCED:
				((AvlNode *)(*node)->data)->factor = AVL_BALANCED;
				((AvlNode *)right->data)->factor = AVL_BALANCED;
				break;
			case AVL_RIGHT_HEAVY:
				((AvlNode *)(*node)->data)->factor = AVL_LEFT_HEAVY;
				((AvlNode *)right->data)->factor = AVL_BALANCED;
				break;
			default:
				break;
		}
		((AvlNode *)grandchild->data)->factor = AVL_BALANCED;
		*node = grandchild;
	}
	
	return;
}

static void destroy_left(BsTree * tree, BTreeNode * node)
{
	/* destroys the left and right subtrees of tree, or the root if
	   node is NULL */
	BTreeNode ** position;
	
	// do not destroy empty tree
	if (0 == tree->size)
		return;
		
	// determine what node to destroy
	if (NULL == node)
		position = &tree->root;
	else
		position = &node->left;

	// destroy nodes
	if (*position != NULL)
	{
		destroy_left(tree, *position);
		destroy_right(tree, *position);
		
		if (tree->destroy != NULL)
		{
			// call user defined destroy
			tree->destroy(((AvlNode *)(*position)->data)->data);
		}
		
		// free AVL data in the node, then the node itself
		free((*position)->data);
		free(*position);
		*position = NULL;
		
		// update size
		tree->size--;
	}
	
	return;
}

static void destroy_right(BsTree * tree, BTreeNode * node)
{
	/* destroys the left and right subtrees of tree, or the root if
	   node is NULL */
	BTreeNode ** position;
	
	// do not destroy empty tree
	if (0 == tree->size)
		return;
	
	// determine what node to destroy
	if (NULL == node)
		position = &tree->root;
	else
		position = &node->right;
	
	// destroy nodes
	if (*position != NULL)
	{
		destroy_left(tree, *position);
		destroy_right(tree, *position);
		
		if (tree->destroy != NULL)
		{
			// call user defined destroy
			tree->destroy(((AvlNode *)(*position)->data)->data);
		}
		
		// free AVL data in the node, then the node itself
		free((*position)->data);
		free(*position);
		*position = NULL;
		
		// update size
		tree->size--;
	}
	
	return;
}

static int insert(BsTree * tree, BTreeNode ** node, const void * data,
				  int * balanced)
{
	/* nest data into an AVL node and rotate if needed */
	AvlNode * avl_data;
	int cmp_val, ret_val;
	
	// insert the data into the tree
	if (is_eob(*node))
	{
		// insert in an empty tree
		if ((avl_data =  (AvlNode *)malloc(sizeof(*avl_data))) == NULL)
			return -1;
		
		avl_data->factor = AVL_BALANCED;
		avl_data->hidden = 0;
		avl_data->data = (void *)data;
		
		return btree_ins_left(tree, *node, avl_data);
	}
	else
	{
		// insert in non-empty tree
		cmp_val = tree->compar(data, ((AvlNode *)(*node)->data)->data);
		if (cmp_val < 0)
		{
			// move to the left
			if (is_eob((*node)->left) )
			{
				if ((avl_data = (AvlNode *)malloc(sizeof(*avl_data))) == NULL)
					return -1;
				
				avl_data->factor = AVL_BALANCED;
				avl_data->hidden = 0;
				avl_data->data = (void *)data;
				
				if (btree_ins_left(tree, *node, avl_data) != 0)
					return -1;
				
				*balanced = 0;
			}
			else
			{
				if ((ret_val = insert(tree, &((*node)->left), data, balanced))
					!= 0)
					return ret_val;
			}
			
			// ensure the tree remains balanced
			if (!(*balanced))
			{
				switch (((AvlNode *)(*node)->data)->factor)
				{
					case AVL_LEFT_HEAVY:
						rotate_left(node);
						*balanced = 1;
						break;
					case AVL_BALANCED:
						((AvlNode *)(*node)->data)->factor = AVL_LEFT_HEAVY;
						break;
					case AVL_RIGHT_HEAVY:
						((AvlNode *)(*node)->data)->factor = AVL_BALANCED;
						*balanced = 1;
						break;
					default:
						break;
				}
			}
		} /* if (cmp_val < 0) */
		else if (cmp_val > 0)
		{
			// move to the right
			if (is_eob((*node)->right) )
			{
				if ((avl_data =  (AvlNode *)malloc(sizeof(*avl_data))) == NULL)
					return -1;
				
				avl_data->factor = AVL_BALANCED;
				avl_data->hidden = 0;
				avl_data->data = (void *)data;
				
				if (btree_ins_right(tree, *node, avl_data) != 0)
					return -1;
				
				*balanced = 0;
			}
			else
			{
				if ((ret_val = insert(tree, &(*node)->right, data, balanced))
					!= 0)
					return ret_val;
			}
			// ensure the tree remains balanced
			if (!(*balanced))
			{
				switch (((AvlNode *)(*node)->data)->factor)
				{
					case AVL_LEFT_HEAVY:
						((AvlNode *)(*node)->data)->factor = AVL_BALANCED;
						*balanced = 1;
						break;
					case AVL_BALANCED:
						((AvlNode *)(*node)->data)->factor = AVL_RIGHT_HEAVY;
						break;
					case AVL_RIGHT_HEAVY:
						rotate_right(node);
						*balanced = 1;
						break;
					default:
						break;
				}
			}
			
		} /* if (cmp_val > 0) */
		else
		{
			// handle finding a copy of the data
			if (!((AvlNode *)(*node)->data)->hidden)
				return 1; // do nothing
			else
			{
				// insert and mark as not hidden
				if (tree->destroy != NULL)
					// destroy
					tree->destroy(((AvlNode *)(*node)->data)->data);
				
				// replace
				((AvlNode *)(*node)->data)->data = (void *)data;
				((AvlNode *)(*node)->data)->hidden = 0;
				
				*balanced = 1;
			}
		}
	} 
	
	return 0;
}

static int hide(BsTree * tree, BTreeNode * node, const void * data)
{
	/* lazy remove a node by hiding it */
	int cmp_val, ret_val;
	
	if (is_eob(node)) // data not found
		return -1;
		
	cmp_val = tree->compar(data, ((AvlNode *)node->data)->data);
	
	if (cmp_val < 0)
		// move to the left
		ret_val = hide(tree, node->left, data);
	else if (cmp_val > 0)
		// move to the right
		ret_val = hide(tree, node->right, data);
	else
	{
		// hide
		((AvlNode *)node->data)->hidden = 1;
		ret_val = 0;
	}
	
	return ret_val;
}

static int lookup(BsTree * tree, BTreeNode * node, const void ** data)
{
	/* lookup data in the tree */
	int cmp_val, ret_val;
	
	if (is_eob(node))
		// not found
		return -1;
	
	cmp_val = tree->compar(*data, ((AvlNode *)node->data)->data);
	
	if (cmp_val < 0)
		// move left
		ret_val = lookup(tree, node->left, data);
	else if (cmp_val > 0)
		// move right
		ret_val = lookup(tree, node->right, data);
	else
	{
		if (!((AvlNode *)node->data)->hidden)
		{
			// found it
			*data = ((AvlNode *)node->data)->data;
			ret_val = 0;
		}
		else
			// not found
			return -1;
	}
	
	return ret_val;
}

void bstree_destroy(BsTree * tree)
{
	/* destroy the AVL tree */
	destroy_left(tree, NULL);
	memset(tree, 0, sizeof(*tree));
	
	return;
}

int bstree_insert(BsTree * tree, const void * data)
{
	/* insert in the AVL tree */
	int balanced = 0;
	
	return insert(tree, &(tree->root), data, &balanced);
}

int bstree_remove(BsTree * tree, const void * data)
{
	/* lazy remove by hiding */
	return hide(tree, tree->root, data);
}

int bstree_lookup(BsTree * tree, const void ** data)
{
	/* lookup data in the tree */
	return lookup(tree, tree->root, data);
}