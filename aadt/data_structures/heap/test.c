#include <stdio.h>
#include <stdlib.h>
#include "heap.h"

#define ITEMS 10
#define HEAP_SIZE	printf("heap size: %d\n", hp->size);
#define HEAP_HEAD	printf("heap head: %d\n", *((int *)(hp->tree[0])))

int compar(const void * d1, const void * d2);
void destroy(void * data);
void * make_node(int num);

int main(void)
{
	Heap hp_;
	Heap * hp = &hp_;
	puts("initializing heap:");
	heap_init(hp, compar, destroy);
	HEAP_SIZE;
	
	printf("adding %d items\n", ITEMS);
	int i;
	for (i = 0; i < ITEMS; ++i)
		heap_insert(hp, make_node(i));
	HEAP_SIZE;
	HEAP_HEAD;
	
	void * tmp;
	puts("extracting from heap");
	heap_extract(hp, &tmp);
	printf("got %d\n", *((int *)tmp));
	free(tmp);
	HEAP_SIZE;
	HEAP_HEAD;
	
	puts("destroying heap");
	heap_destroy(hp);
	HEAP_SIZE;
	
	puts("All tests have finished successfully");
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