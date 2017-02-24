/* preorder, inorder, and postorder traversals of fully
   parenthesized expression */
/* Use: test < input.txt */
/* To activate the merge test: test -m */ 
  
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "btree.h"

#define BUFF_SIZE 16
// holds current lexeme
char buff[BUFF_SIZE];
// holds current tree node address
BTreeNode * node_stack[BUFF_SIZE];
// node stack pointer
int nsp = 0;
// lexeme constants
enum {EOI, L_PAR, R_PAR, OP, NUM, ERR};

// gets next lexeme
int next_sym(void);

// node stack functionss
int push(BTreeNode * node);
BTreeNode * pop(void);
BTreeNode * peek(void);

// make node data buffer
char * make_node(void);

// destroy tree node function
void destroy(void * data);

// traversal functions
int preorder(const BTreeNode * node);
int inorder(const BTreeNode * node);
int postorder(const BTreeNode * node);


int main(int argc, char * argv[])
{
	/* test with fully parenthesized expressions */

	if (argv[1] && strcmp(argv[1], "-m") == 0)
	{
		puts("merge test:");
		BTree tr1_, tr2_, trm_;
		BTree * tr1 = &tr1_;
		BTree * tr2 = &tr2_;
		BTree * trm = &trm_;
		
		// initialize two trees
		btree_init(tr1, NULL, destroy);
		btree_init(tr2, NULL, destroy);

		// put some numbers in
		btree_ins_left(tr1, tr1->root, (void *)make_node());
		strcpy((char*)tr1->root->data, "20");	
		
		btree_ins_left(tr2, tr2->root, (void *)make_node());
		strcpy((char*)tr2->root->data, "30");	
		
		// make node with operator
		char * nd = make_node();
		strcpy(nd, "/");
		
		puts("input: 20 / 30");
		// merge
		btree_merge(trm, tr1, tr2, (void*)nd);
		
		// output
		puts("preorder:");
		preorder(trm->root);
		putchar('\n');
		
		puts("inorder:");
		inorder(trm->root);
		putchar('\n');
		
		puts("postorder:");
		postorder(trm->root);
		putchar('\n');
		btree_destroy(trm);
		
		return 0;
	}
	
	int lex;
	BTree exp_tr_;
	BTree * exp_tr = &exp_tr_;
	BTreeNode * curr_node;

	// initialize tree
	btree_init(exp_tr, NULL, destroy);
	btree_ins_left(exp_tr, exp_tr->root, (void *)make_node());
	
	// push root node
	push(exp_tr->root);
	
	// read input
	puts("input: ");
	while ((lex = next_sym()) != EOI)
	{
		printf("%s", buff);
		switch (lex)
		{
			case L_PAR:
				curr_node = peek();
				btree_ins_left(exp_tr, curr_node, (void *)make_node());
				push(curr_node->left);
				break;
			case OP:
				curr_node = peek();
				strcpy((char *)curr_node->data, buff);
				btree_ins_right(exp_tr, curr_node, (void *)make_node());
				push(curr_node->right);
				break;
			case NUM:
				curr_node = peek();
				strcpy((char *)curr_node->data, buff);
				pop();
				break;
			case R_PAR:
				pop();
				break;
			default:
				break;
			
		}
	}
	
	putchar('\n');
	puts("preorder:");
	preorder(exp_tr->root);
	putchar('\n');
	
	puts("inorder:");
	inorder(exp_tr->root);
	putchar('\n');
	
	puts("postorder:");
	postorder(exp_tr->root);
	putchar('\n');
	
	btree_destroy(exp_tr);
	
	return 0;
}

int push(BTreeNode * node)
{
	/* push tree node on stack */
	if ( (nsp + 1) > BUFF_SIZE )
		return -1;
	
	node_stack[nsp++] = node;
	return 0;
}

BTreeNode * pop(void)
{
	/* pop tree node from stack */
	if ( (nsp - 1) <  0 )
		return NULL;
	
	return node_stack[nsp--];
}

BTreeNode * peek(void)
{
	/* read top of the stack */
	return node_stack[nsp - 1];
}

int next_sym(void)
{
	/* get next symbol from input stream */
	int ch = getchar();
	int ret, i = 0;
	
	while (isspace(ch))
		ch = getchar();
	
	switch (ch)
	{
		case EOF: ret = EOI; break;
 		case '(': ret = L_PAR; break;
		case ')': ret = R_PAR; break;
		case '+':
		case '-':
		case '*':
		case '/': ret = OP; break;
		default:
			while (isdigit(ch) && i < BUFF_SIZE)
			{
				buff[i++] = ch;
				ch = getchar();
			}
			buff[i] = '\0';
			
			if (strchr(" ()+-*/", ch))
				ungetc(ch, stdin);
			else
			{
				printf("\nErr: syntax error < %c > is unrecognized.\n", ch);
				exit(EXIT_FAILURE);
			}
			
			return NUM;
			break;
	}
	
	buff[i++] = ch;
	buff[i] = '\0';
	
	return ret;
}

int preorder(const BTreeNode * node)
{
	/* first traverse the root, then left and right */
	if (node != NULL && *((char *)node->data) != '\0')
		printf("%s ", (char *)node->data);
	
	if (node->left != NULL)
		preorder(node->left);
	
	if (node->right != NULL)
		preorder(node->right);
	
	return 0;
}

int inorder(const BTreeNode * node)
{
	/* first traverse left, then the root and right */
	if (node->left != NULL)
		inorder(node->left);
	
	if (node != NULL && *((char *)node->data) != '\0')
		printf("%s ", (char *)node->data);
		
	if (node->right != NULL)
		inorder(node->right);
	
	return 0;
}

int postorder(const BTreeNode * node)
{
	/* first traverse left, then right and root */
	if (node->left != NULL)
		postorder(node->left);
	
	if (node->right != NULL)
		postorder(node->right);
	
	if (node != NULL && *((char *)node->data) != '\0')
		printf("%s ", (char *)node->data);
	
	return 0;
}

void destroy(void * data)
{
	/* free tree data */
	free(data);
	return;
}

char * make_node(void)
{
	/* allocate empty buffer */
	char * ret;
	
	if ( (ret = (char *)malloc(sizeof(*ret) * BUFF_SIZE)) == NULL )
	{
		puts("Err: make_node() - memory allocation failed.");
		exit(EXIT_FAILURE);
	}
	
	*ret = '\0';
	
	return ret;
}