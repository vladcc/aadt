#include <stdio.h>
#include "bstree.h"

#define ITEMS 10

int compar(const void * d1, const void * d2);
void destroy(void * data);
void * make_node(int num);
int inorder(const BTreeNode * node);

int main(void)
{
	/* test the tree */
	BsTree bstr_, * bstr;
	bstr = &bstr_;
	
	bstree_init(bstr, compar, destroy);
	
	puts("inserting items 0-9");
	int i;
	for (i = 0; i < ITEMS; ++i)
		bstree_insert(bstr, make_node(i));
	
	printf("tree size: %d\n", bstr->size);
	printf("root: %d\n",*((int *)((AvlNode *)bstr->root->data)->data));
	puts("inorder:");
	inorder(bstr->root);
	putchar('\n');
	
	puts("looking for 9000");
	int * back;
	int * lup = (int *)make_node(9000);
	back = lup;
	printf("found: ");
	if (bstree_lookup(bstr, (const void **)&lup) == 0)
		puts("yes");
	else
		puts("no");
	free(back);
	
	puts("looking for 8");
	lup = (int *)make_node(8);
	back = lup;
	printf("found: ");
	if (bstree_lookup(bstr, (const void **)&lup) == 0)
		puts("yes");
	else
		puts("no");
	free(back);
	
	lup = (int *)make_node(8);
	back = lup;
	puts("removing 8...");
	if (bstree_remove(bstr, (void *)lup) == 0)
		puts("8 is now hidden");
	else
		puts("err: not removed");
	
	puts("looking for 8 again");
	printf("found: ");
	if (bstree_lookup(bstr, (const void **)&lup) == 0)
		puts("yes");
	else
		puts("no");
	free(back);
	
	printf("tree size: %d\n", bstr->size);
	printf("root: %d\n",*((int *)((AvlNode *)bstr->root->data)->data));
	puts("inorder:");
	inorder(bstr->root);
	putchar('\n');
	
	puts("destroying tree");
	bstree_destroy(bstr);
	printf("tree size: %d\n", bstr->size);
	
	puts("All tests passed successfully");
	
	return 0;
}

int inorder(const BTreeNode * node)
{
	/* first traverse left, then the root and right */
	if (node->left != NULL)
		inorder(node->left);
	
	if (node != NULL && (0 == ((AvlNode *)node->data)->hidden))
		printf("%d ", *((int *)((AvlNode *)node->data)->data));
		
	if (node->right != NULL)
		inorder(node->right);
	
	return 0;
}

int compar(const void * d1, const void * d2)
{
	/* classic compar */
	int * i1, * i2;
	i1 = (int *)d1;
	i2 = (int *)d2;

	if (*i1 < *i2)
		return -1;
	else if (*i1 > *i2)
		return 1;
	else
		return 0;
}

void destroy(void * data)
{
	/* free the memory */
	free(data);
	return;
}

void * make_node(int num)
{
	/* allocate new node */
	int * i;
	
	if ((i = (int *)malloc(sizeof(*i))) == NULL)
		return NULL;
	
	*i = num;
	return (void *)i;
}