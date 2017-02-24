#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

#define STACK_PRINT_SIZE	printf("stack size: %d\n", stack->size)

#define STACK_INITIALIZE \
		do \
		{puts("initializing stack"); \
		stack_init(stack, destroy); \
		STACK_PRINT_SIZE;} while (0)

#define STACK_DESTROY \
		do \
		{puts("destroying stack"); \
		stack_destroy(stack); \
		STACK_PRINT_SIZE;} while (0)

#define STACK_PUSH_ELMNTS(n) \
		do \
		{printf("pushing %d elements:\n", (n)); \
		int i, j, * el; \
		for (i = 0, j = (n); i < j; ++i) \
		{ \
			el = make_node(i + 1); \
			stack_push(stack, el); \
		} \
		STACK_PRINT_SIZE;} while (0)

#define STACK_POP_ALL \
		do \
		{puts("popping all elements..."); \
		int * el; \
		while (stack->size > 0) \
		{ \
			stack_pop(stack, (void *)&el); \
			printf("popped element is: %d\n", *el); \
			free (el); \
			STACK_PRINT_SIZE; \
		} } while (0)
			
int * make_node(int a);
void destroy(void * data);

int main(void)
{
	int * el;
	Stack stack_;
	Stack * stack = &stack_;
	
	STACK_INITIALIZE;
	
	STACK_PUSH_ELMNTS(10);
	
	puts("peeking");
	el = stack_peek(stack);
	printf("top element is: %d\n", *el);
	STACK_PRINT_SIZE;
	
	puts("popping top element");
	stack_pop(stack, (void *)&el);
	printf("popped element is: %d\n", *el);
	
	STACK_PRINT_SIZE;
	
	puts("popping top element");
	stack_pop(stack, (void *)&el);
	printf("popped element is: %d\n", *el);
	
	printf("stack head is: %p\n", stack->head);
	
	STACK_POP_ALL;
	
	puts("popping top element");
	if (stack_pop(stack, (void *)&el) < 0)
		puts("stack is empty");
	else
		printf("popped element is: %d\n", *el);
	
	puts("peeking...");
	if ((el = stack_peek(stack)) == NULL)
		puts("stack is empty");
	else
	{
		printf("top element is: %d\n", *el);
		STACK_PRINT_SIZE;
	}
		
	printf("stack head is: %p\n", stack->head);
	
	STACK_DESTROY;
	
	puts("initializing stack no destroy");
	stack_init(stack, NULL);
	STACK_PUSH_ELMNTS(5);
	
	printf("stack head is: %p\n", stack->head);
	
	STACK_DESTROY;

	printf("stack head is: %p\n", stack->head);
	
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