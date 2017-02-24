/* heap.h -- heap public interface */
/* v1.0 */
#ifndef HEAP_H
#define HEAP_H

typedef struct Heap_ {
	int size;
	int (*compar)(const void * k1, const void * k2);
	void (*destroy)(void * data);
	void ** tree;
} Heap;

void heap_init(Heap * heap, int (*compar)(const void * k1, const void * k2),
			   void (*destroy)(void * data));
/*
returns: nothing

description: Initializes the heap. compar is mandatory. destroy can be NULL.

complexity: O(1)
*/

void heap_destroy(Heap * heap);
/*
returns: nothing

description: Destroys the heap. Calls destroy(), provided it's not NULL.

complexity: O(n)
*/

int heap_insert(Heap * heap, const void * data);
/*
returns: 0 on success, -1 otherwise

description: Inserts a node in the heap.

complexity: O(log n)
*/

int heap_extract(Heap * heap, void ** data);
/*
returns: 0 on success, -1 otherwise

description: Extracts the node at the top of the heap, and sets *data pointing
to it.

complexity: O(log n)
*/
#endif