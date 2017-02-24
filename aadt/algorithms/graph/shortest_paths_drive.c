#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "graph.h"
#include "shortest_paths.h"

#define ITEMS 6

#define ADJ_LIST(n)	do {void * vp; AdjList * adjlp; int ret;\
						vp = ver_tbl[(n)];\
						ret = graph_adjlist(gr, vp, &adjlp);\
						if (0 == ret){\
							printf("Vertex %c is adjacent to: ", ((n) + 'a'));\
							print_adj_list((Arrlist *)(&adjlp->adjacent));}} while (0)

#define EDGES 		printf("Edges: %d\n", gr->ecount)
#define VERTICES	printf("Vertices: %d\n", gr->vcount)

int compar(const void * d1, const void * d2);
void destroy(void * data);
PathVertex * make_vertex(int num);
void print_adj_list(Arrlist * list);
void print_ret_list(ListElmt * element, void * args);

int main(void)
{
	Graph gr_;
	Graph * gr = &gr_;
	List retl_;
	List * retl = &retl_;
	PathVertex * ver_tbl[ITEMS];
	
	puts("Initializing directed weighted graph");
	graph_init(gr, compar, destroy);
	
	printf("Inserting %d vertices\n", ITEMS);
	int i;
	for (i = 0; i < ITEMS; ++i)
	{
		ver_tbl[i] = make_vertex(i);
		if (graph_ins_vertex(gr, ver_tbl[i]) < 0)
			return -1;
	}
	
	// edges from vertex a
	PathVertex ab8, ac4;
	ab8.data = ver_tbl[1]->data;
	ab8.weight = 8;
	graph_ins_edge(gr, ver_tbl[0], &ab8);
	
	ac4.data = ver_tbl[2]->data;
	ac4.weight = 4;
	graph_ins_edge(gr, ver_tbl[0], &ac4);
	
	// edges from vertex b
	PathVertex bc6, bd2;

	bc6.data = ver_tbl[2]->data;
	bc6.weight = 6;
	graph_ins_edge(gr, ver_tbl[1], &bc6);
	
	bd2.data = ver_tbl[3]->data;
	bd2.weight = 2;
	graph_ins_edge(gr, ver_tbl[1], &bd2);
	
	// edges for vertex c
	PathVertex ce4, cf1;
	
	ce4.data = ver_tbl[4]->data;
	ce4.weight = 4;
	graph_ins_edge(gr, ver_tbl[2], &ce4);
	
	cf1.data = ver_tbl[5]->data;
	cf1.weight = 1;
	graph_ins_edge(gr, ver_tbl[2], &cf1);
	
	// edges for vertex e
	PathVertex ef5;
	
	ef5.data = ver_tbl[5]->data;
	ef5.weight = 5;
	graph_ins_edge(gr, ver_tbl[4], &ef5);
	
	// edges for vertex f
	PathVertex fb2, fd7;
	
	fb2.data = ver_tbl[1]->data;
	fb2.weight = 2;
	graph_ins_edge(gr, ver_tbl[5], &fb2);
	
	fd7.data = ver_tbl[3]->data;
	fd7.weight = 7;
	graph_ins_edge(gr, ver_tbl[5], &fd7);
	
	for (i = 0; i < ITEMS; ++i)
		ADJ_LIST(i);

	VERTICES;
	EDGES;
	
	puts("\t     a");
	puts("\t    / \\");
	puts("\t  8/   \\4");
	puts("\t  /  6  \\");
	puts("\t b-------c");
	puts("\t |\\     /|");
	puts("\t |2\\   /1|");
	puts("\t2|  \\ /  |4");
	puts("\t |   f   |");
	puts("\t |7/   \\5|");
	puts("\t d       e");
	
	if (shortest_paths(gr, ver_tbl[0], retl, compar) < 0)
	{
		puts("Error in min_span_tree()");
		goto end;
	}
	
	puts("Result:");
	list_apply_all(retl, print_ret_list, NULL);
	list_destroy(retl);
	
end:
	puts("Destroying graph");
	graph_destroy(gr);
	
	return 0;
}

int compar(const void * d1, const void * d2)
{
	/* classic compar */
	int * i1, * i2;
	i1 = (int *)((PathVertex *)d1)->data;
	i2 = (int *)((PathVertex *)d2)->data;
	
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
	PathVertex * bp = (PathVertex *)data;
	
	free(bp->data);
	free(bp);
	return;
}

PathVertex * make_vertex(int num)
{
	/* make new vertex */
	PathVertex * pv;
	int * i;
	
	if ((i = (int *)malloc(sizeof(*i))) == NULL)
		return NULL;
	
	if ((pv = (PathVertex *)malloc(sizeof(*pv))) == NULL)
		return NULL;
	
	*i = num + 'a';
	pv->data = i;
	pv->color = -1;
	
	return pv;
}

void print_adj_list(Arrlist * list)
{
	/* print the adjacency list */
	int i;
	PathVertex * pv;
	ArrlistElmt * ale;
	for (i = 0; i < list->list_size; ++i)
	{	
		ale = arrlist_get_elmt(list, i);
		pv = (PathVertex *)ale;
		printf("\nv = %c, w = %.2f", *((int *)pv->data), pv->weight);
	}
		
	putchar('\n');
	return;
}

void print_ret_list(ListElmt * element, void * args)
{
	/* print the returned list */
	PathVertex * path_vertex = (PathVertex *)element->data;
	printf("vertex: %c, parent: %c, shortest: %.2f\n", 
			*((int *)path_vertex->data),
			(path_vertex->parent == NULL) ? '_' : *((int *)path_vertex->parent->data),
			path_vertex->shortest);
	return;
}