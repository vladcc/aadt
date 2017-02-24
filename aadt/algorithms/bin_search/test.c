#include <stdio.h>
#include <stdlib.h>
#include "binary_search.h"

#define look_for(i) do {void * tr = make_target((i));\
						int ret;\
						printf("looking for %d\n", (i));\
						ret = binary_search(arr, tr, arr_size, sizeof(arr[0]), compar);\
						if (ret != -1)\
							printf("target found at pos: %d, target: %d\n", ret, arr[ret]);\
						else\
							puts("target not found");\
						free(tr);} while (0)

void print_arr(int * arr, int size);
int compar(const void * k1, const void * k2);
void * make_target(int n);

int main(int argc, char * argv[])
{
	int arr[] = {1, 5, 40, 200, 332, 999, 1532, 14209, 888889};
	int arr_size = sizeof(arr) / sizeof(arr[0]);
	
	print_arr(arr, arr_size);
	look_for(-5);
	look_for(0);
	look_for(1);
	look_for(200);
	look_for(1532);
	look_for(888889);
	look_for(2000333);
	
	return 0;
}

void * make_target(int n)
{
	int * ret;
	
	ret = malloc(sizeof(*ret));
	if (NULL == ret)
		return NULL;
	
	*ret = n;
	
	return ret;
}

void print_arr(int * arr, int size)
{
	int i;
	for (i = 0; i < size; ++i)
		printf("%d ", arr[i]);
	
	putchar('\n');
	return;
}

int compar(const void * k1, const void * k2)
{
	if (*((int *)k1) > *((int *)k2))
		return 1;
	else if (*((int *)k1) < *((int *)k2))
		return -1;
	else
		return 0;
}