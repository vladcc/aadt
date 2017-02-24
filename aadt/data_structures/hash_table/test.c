#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "list.h"
#include "chtbl.h"

#define BUFF_SIZE 128
#define BUCKETS 26 // letters of the alphabet

/* 	Note: run with redirection!
	test.exe < text_file.txt */

static char inputbuff[BUFF_SIZE];
static char wordbuff[BUFF_SIZE];

char * make_node(int len);
int compar(const void * key1, const void * key2);
void destroy(void * data);
void print_elmnt(ListElmt * l_element, void * args);
int hash(const void * key);

int main(void)
{
	int i;
	char * node;
	CHTbl htbl_1;
	CHTbl * htbl = &htbl_1;
	
	puts("initializing hash table");
	if (chtbl_init(htbl, BUCKETS, hash, compar, destroy) < 0)
	{
		puts("err: initializatoin failed");
		return -1;
	}
	printf("table size is now: %d\n", htbl->size);
	
	puts("reading words");
	i = 0;
	while (scanf("%127s", inputbuff) == 1)
	{
		// get words only
		int c, j, k;
		for (j = k = 0; inputbuff[k] != '\0'; ++k)
		{
			c = inputbuff[k];
			// add only letters and make them lowercase
			if (isalpha(c))
				wordbuff[j++] = tolower(c);
		}
		// terminate word
		wordbuff[j] = '\0';
		
		node = make_node(strlen(wordbuff) + 1);
		chtbl_insert(htbl, node);
		++i;
	}
	puts("done");
	printf("%d words processed\n", i);
	printf("table size/unique words: %d\n", htbl->size);
	
	puts("sorting lists...");
	for (i = 0; i < htbl->buckets; ++i)
		list_sort(&htbl->table[i], LIST_SORT_UP);
	
	puts("\nprint words");
	for (i = 0; i < BUCKETS; ++i)
	{
		if (htbl->table[i].head != NULL)
		{
			printf("bucket no. %d:\n", i);
			list_apply_all(&htbl->table[i], print_elmnt, NULL);
			putchar('\n');
		}
	}
	
	// look for last added word
	char * tmp = node = make_node(strlen(wordbuff) + 1);
	printf("\nlooking for %s\n", tmp);
	if (chtbl_lookup(htbl, (void **)&tmp) == -1)
		puts("not found");
	else
		puts("found");
	
	// removing it
	tmp = node;
	puts("removing it");
	if (chtbl_remove(htbl, (void **)&tmp) == 0)
		puts("removed");
	else
		puts("not there to remove");
	
	// look for last added word again
	tmp = node;
	printf("\nlooking for %s again\n", tmp);
	if (chtbl_lookup(htbl, (void **)&tmp) == -1)
		puts("not found");
	else
		puts("found");
	
	// removing it again
	tmp = node;
	puts("removing it");
	if (chtbl_remove(htbl, (void **)&tmp) == 0)
		puts("removed");
	else
		puts("not there to remove");
	
	puts("\nprint words");
	for (i = 0; i < BUCKETS; ++i)
	{
		if (htbl->table[i].head != NULL)
		{
			printf("bucket no. %d:\n", i);
			list_apply_all(&htbl->table[i], print_elmnt, NULL);
			putchar('\n');
		}
	}
	printf("table size: %d\n", htbl->size);
	
	free(node);
	
	puts("destroying table");
	chtbl_destroy(htbl);
	puts("testing is done");
	return 0;
}

int hash(const void * key)
{
	/* hash for alphabet order */
	return tolower(*((char *)key)) - 'a';
}

void print_elmnt(ListElmt * l_element, void * args)
{
	/* print list element */
	puts((char *)l_element->data);
	return;
}

char * make_node(int len)
{
	/* allocate memory for every word */
	char * out;
	if ( (out = (char *)malloc(sizeof(*out) * len)) == NULL )
	{
		puts("Err: make_node() could not allocate memory");
		exit(EXIT_FAILURE);
	}
	strcpy(out, wordbuff);
	return out;
}

int compar(const void * key1, const void * key2)
{
	/* for sorting and searching words */
	return strcmp((const char *)key1, (const char *)key2);
}

void destroy(void * data)
{
	/* deallocate word memory */
	free(data);
	return;
}