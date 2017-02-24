/* heap.c -- heap implementation as an array */
/* v1.0 */
#include <stdlib.h>
#include <string.h>
#include "heap.h"

#define parent(npos)	((int)(((npos)-1)/2))
#define left(npos)		(((npos)*2)+1)
#define right(npos)		(((npos)*2)+2)

void heap_init(Heap * heap, int (*compar)(const void * k1, const void * k2),
			   void (*destroy)(void * data))
{
	/* initialize the heap */
	heap->size = 0;
	heap->compar = compar;
	heap->destroy = destroy;
	heap->tree = NULL;
	
	return;
}

void heap_destroy(Heap * heap)
{
	/* destroy the heap */
	if (heap->destroy != NULL)
	{
		int i;
		for (i = 0; i < heap->size; ++i)
			heap->destroy(heap->tree[i]);
	}
	
	free(heap->tree);
	memset(heap, 0, sizeof(*heap));
	
	return;
}

int heap_insert(Heap * heap, const void * data)
{
	/* insert in the heap */
	void * temp;
	int ipos, ppos;
	
	// allocate memory for the node
	if ((temp = (void **)realloc(heap->tree, (heap->size+1)*sizeof(temp)))
		 == NULL)
			 return -1;
	
	heap->tree = temp;
	
	// insert after the last node
	heap->tree[heap->size] = (void *)data;
	
	// heapify by pushing the contents of the new node up
	ipos = heap->size;
	ppos = parent(ipos);
	
	while (ipos > 0 && heap->compar(heap->tree[ppos], heap->tree[ipos]) < 0)
	{
		// swap the current node and it's parent
		temp = heap->tree[ppos];
		heap->tree[ppos] = heap->tree[ipos];
		heap->tree[ipos] = temp;
		
		// move up one level
		ipos = ppos;
		ppos = parent(ipos);
	}
	
	// update size
	heap->size++;
	
	return 0;
}

int heap_extract(Heap * heap, void ** data)
{
	/* remove the head of the heap and reheapify */
	void * save, * temp;
	int ipos, lpos, rpos, mpos;
	
	// don't extract from an empty heap
	if (0 == heap->size)
		return -1;
	
	// get top of the heap
	*data = heap->tree[0];
	
	// adjust the storage
	save = heap->tree[heap->size-1];
	if (heap->size-1 > 0)
	{
		if ((temp = (void **)realloc(heap->tree, (heap->size-1)*sizeof(temp)))
			== NULL)
			return -1;
	
		heap->tree = temp;
		
		// update size
		heap->size--;
	}
	else
	{
		// if the last node is extracted
		free(heap->tree);
		heap->tree = NULL;
		heap->size = 0;
		return 0;
	}
	
	// copy the last node on top
	heap->tree[0] = save;
	
	// heapify by pushing the new top down
	ipos = 0;
	
	while (1)
	{
		lpos = left(ipos);
		rpos = right(ipos);
		
		if (lpos < heap->size && heap->compar(heap->tree[lpos],
			heap->tree[ipos]) > 0)
			mpos = lpos;
		else
			mpos = ipos;
		
		if (rpos < heap->size && heap->compar(heap->tree[rpos],
			heap->tree[mpos]) > 0)
			mpos = rpos;
		
		// when theese are the same the heap property has been restored
		if (mpos == ipos)
			break;
		else
		{
			// swap
			temp = heap->tree[mpos];
			heap->tree[mpos] = heap->tree[ipos];
			heap->tree[ipos] = temp;
			
			// move down one level
			ipos = mpos;
		}
	}
	
	return 0;
}