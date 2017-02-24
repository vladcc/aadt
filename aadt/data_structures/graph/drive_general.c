#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

#define ITEMS 5

#define EDGES 		printf("Edges: %d\n", gr->ecount)
#define VERTICES	printf("Vertices: %d\n", gr->vcount)
#define ADJ_LIST(n)	do {void * vp;\
						printf("Get adjecency list for vertex %d: ", (n));\
						vp = make_node((n));\
						ret = graph_adjlist(gr, vp, &adjlp);\
						printf("%s\n", (0 == ret) ? "success" : "failed");\
						free(vp);\
						if (0 == ret){\
							printf("Vertex %d is adjacent to: ", (n));\
							print_list((Arrlist *)(&adjlp->adjacent));}} while (0)	

#define REMOVE_V(n)	do {void * vp, * dum;\
						printf("Removing vertex %d: ", (n));\
						dum = vp = make_node((n));\
						ret = graph_rem_vertex(gr, &vp);\
						free(dum);\
						printf("%s\n", (0 == ret) ? "success" : "failed");\
						VERTICES;} while(0)

#define REMOVE_E(n, m)	do {void * vp;\
						printf("Removing edge %d from vertex %d: ", (m), (n));\
						vp = ver_tbl[(m)-1];\
						ret = graph_rem_edge(gr, ver_tbl[(n)-1], (const void **)&vp);\
						printf("%s\n", (0 == ret) ? "success" : "failed");\
						EDGES;} while(0)
							
#define IS_ADJ(n, m) do {int retv;\
						printf("Is vertex %d adjacent to %d?: ", (n), (m));\
						retv = graph_is_adjacent(gr, ver_tbl[(n-1)], ver_tbl[(m-1)]);\
						printf("%s\n", (retv >= 0) ? "yes" : "no");} while (0)
	
int compar(const void * d1, const void * d2);
void destroy(void * data);
void * make_node(int num);
void print_list(Arrlist * list);

int main(void)
{
	Graph gr_;
	Graph * gr = &gr_;
	AdjList * adjlp;
	void * ver_tbl[ITEMS];
	void * vp;
	int ret;
	
	puts("Initializing graph");
	graph_init(gr, compar, destroy);
	
	VERTICES;
	printf("Insterting %d vertices\n", ITEMS);
	int i;
	for (i = 0; i < ITEMS; ++i)
	{
		ver_tbl[i] = make_node(i+1);
		if (graph_ins_vertex(gr, ver_tbl[i]) < 0)
			return -1;
	}
	VERTICES;
	EDGES;
	
	printf("Inserting duplicate to vertex 1: ");
	vp = make_node(1);
	ret = graph_ins_vertex(gr, vp);
	printf("%s\n", (0 == ret) ? "success" : "failed");
	free(vp);
	
	REMOVE_V(1);
	
	puts("Re-inserting the vertex");
	graph_ins_vertex(gr, ver_tbl[0]);
	VERTICES;
	
	puts("Inserting edges:");
	graph_ins_edge(gr, ver_tbl[0], ver_tbl[1]);
	graph_ins_edge(gr, ver_tbl[1], ver_tbl[0]);
	EDGES;
	
	printf("Inserting duplicate edge: %s\n",
	(graph_ins_edge(gr, ver_tbl[1], ver_tbl[0]) == 1) ? "failed" : "success");
	EDGES;
	
	graph_ins_edge(gr, ver_tbl[0], ver_tbl[2]);
	graph_ins_edge(gr, ver_tbl[2], ver_tbl[0]);
	
	graph_ins_edge(gr, ver_tbl[2], ver_tbl[3]);
	graph_ins_edge(gr, ver_tbl[3], ver_tbl[2]);
	
	puts("Adjacency: 1:2-3; 2:1; 3:1-4; 4:3;");
	EDGES;
	
	puts("Removing when adjacent:");
	REMOVE_V(1);
	
	for (i = 0; i < ITEMS; ++i)
		ADJ_LIST(i+1);
	
	ADJ_LIST(1);
	IS_ADJ(1, 2);
	IS_ADJ(1, 3);
	IS_ADJ(1, 4);
	
	ADJ_LIST(2);
	REMOVE_V(2);
	puts("Removing existing edge:");
	REMOVE_E(2, 1);
	ADJ_LIST(2);
	ADJ_LIST(1);
	REMOVE_V(2);
	REMOVE_E(1, 2);
	ADJ_LIST(1);
	REMOVE_V(2);
	puts("Removing non-existing edge");
	REMOVE_E(1, 2);
	
	puts("Get adjacency list of non-existing element:");
	ADJ_LIST(10);
	
	puts("Look for adjacency to non-existent elements:");
	IS_ADJ(1, 2);
	IS_ADJ(1, 30);
	
	puts("Destroying graph");
	graph_destroy(gr);
	
	puts("All tests passed successfully");
	return 0;
}

void print_list(Arrlist * list)
{
	/* print the adjacency list */
	int i;
	ArrlistElmt * ale;
	for (i = 0; i < list->list_size; ++i)
	{
		ale = arrlist_get_elmt(list, i);
		printf("%d ", *((int *)ale));
	}
		
	putchar('\n');
	return;
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
	/* create a vertex */
	int * i;
	
	if ((i = (int *)malloc(sizeof(*i))) == NULL)
		return NULL;

	*i = num;
	
	return i;
}