#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

#define QUEUE_PRINT_SIZE	printf("queue size: %d\n", queue->size)

#define QUEUE_INITIALIZE \
		do \
		{puts("initializing queue"); \
		queue_init(queue, destroy); \
		QUEUE_PRINT_SIZE;} while (0)

#define QUEUE_DESTROY \
		do \
		{puts("destroying queue"); \
		queue_destroy(queue); \
		QUEUE_PRINT_SIZE;} while (0)

#define QUEUE_ENQ_ELMNTS(n) \
		do \
		{printf("queuing %d elements:\n", (n)); \
		int i, j, * el; \
		for (i = 0, j = (n); i < j; ++i) \
		{ \
			el = make_node(i + 1); \
			queue_enq(queue, el); \
		} \
		QUEUE_PRINT_SIZE;} while (0)
			
#define QUEUE_DEQ_ALL \
		do \
		{puts("dequeuing all elements..."); \
		int * el; \
		while (queue->size > 0) \
		{ \
			queue_deq(queue, (void *)&el); \
			printf("dequeued element is: %d\n", *el); \
			free (el); \
			QUEUE_PRINT_SIZE; \
		} } while (0)
			
int * make_node(int a);
void destroy(void * data);

int main(void)
{
	int * el;
	Queue queue_;
	Queue * queue = &queue_;
	
	QUEUE_INITIALIZE;
	
	QUEUE_ENQ_ELMNTS(10);
	
	printf("head is: %p\n", queue->head);
	printf("tail is: %p\n", queue->tail);
	
	el = queue_peek(queue);
	printf("peeked element is : %d\n", *el);
	QUEUE_PRINT_SIZE;
	
	QUEUE_DEQ_ALL;
	
	printf("head is: %p\n", queue->head);
	printf("tail is: %p\n", queue->tail);
	
	puts("\npeeking empty queue");
	if ((el = queue_peek(queue)) == NULL)
		puts("queue is empty");
	else
		printf("peeked element is : %d\n", *el);
	
	puts("\ndeq empty queue");
	if (queue_deq(queue, (void *)&el) < 0)
		puts("queue is empty");
	else
	{
		printf("dequeued element is: %d\n", *el);
		free (el);
		QUEUE_PRINT_SIZE;
	}
	
	QUEUE_ENQ_ELMNTS(1);
	QUEUE_DEQ_ALL;
	
	printf("head is: %p\n", queue->head);
	printf("tail is: %p\n", queue->tail);
	
	QUEUE_DESTROY;
	
	puts("\ninit without destroy");
	queue_init(queue, NULL);
	QUEUE_DESTROY;
	
	puts("\ninit without destroy");
	queue_init(queue, NULL);
	QUEUE_ENQ_ELMNTS(3);
	QUEUE_DEQ_ALL;
	
	printf("head is: %p\n", queue->head);
	printf("tail is: %p\n", queue->tail);
	
	QUEUE_DESTROY;
	
	puts("\ninit without destroy");
	queue_init(queue, NULL);
	QUEUE_ENQ_ELMNTS(5);
	
	printf("head is: %p\n", queue->head);
	printf("tail is: %p\n", queue->tail);
	
	QUEUE_DESTROY;
	
	printf("head is: %p\n", queue->head);
	printf("tail is: %p\n", queue->tail);
	
	puts("Testing is complete");
	
	return 0;
}

int * make_node(int a)
{
	int * out;
	if ( (out = malloc(sizeof(*out))) == NULL )
	{
		puts("Err: make_node() could not allocate memory");
		exit(EXIT_FAILURE);
	}
	
	*out = a;
	
	return out;
}

void destroy(void * data)
{
	free(data);
	return;
}