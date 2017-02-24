#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "min_span_tree.h"
#include "graph.h"

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
MstVertex * make_vertex(int num);
void print_adj_list(Arrlist * list);
void print_ret_list(ListElmt * element, void * args);

int main(void)
{
	Graph gr_;
	Graph * gr = &gr_;
	List retl_;
	List * retl = &retl_;
	MstVertex * ver_tbl[ITEMS];
	
	puts("Initializing undirected weighted graph");
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
	MstVertex ab7, ac4;
	ab7.data = ver_tbl[1]->data;
	ab7.weight = 7;
	graph_ins_edge(gr, ver_tbl[0], &ab7);
	
	ac4.data = ver_tbl[2]->data;
	ac4.weight = 4;
	graph_ins_edge(gr, ver_tbl[0], &ac4);
	
	// edges from vertex b
	MstVertex ba7, bc6, bd2, bf4;
	ba7.data = ver_tbl[0]->data;
	ba7.weight = 7;
	graph_ins_edge(gr, ver_tbl[1], &ba7);
	
	bc6.data = ver_tbl[2]->data;
	bc6.weight = 6;
	graph_ins_edge(gr, ver_tbl[1], &bc6);
	
	bd2.data = ver_tbl[3]->data;
	bd2.weight = 2;
	graph_ins_edge(gr, ver_tbl[1], &bd2);
	
	bf4.data = ver_tbl[5]->data;
	bf4.weight = 4;
	graph_ins_edge(gr, ver_tbl[1], &bf4);
	
	// edges for vertex c
	MstVertex ca4, cb6, ce9, cf8;
	
	ca4.data = ver_tbl[0]->data;
	ca4.weight = 4;
	graph_ins_edge(gr, ver_tbl[2], &ca4);
	
	cb6.data = ver_tbl[1]->data;
	cb6.weight = 6;
	graph_ins_edge(gr, ver_tbl[2], &cb6);
	
	ce9.data = ver_tbl[4]->data;
	ce9.weight = 9;
	graph_ins_edge(gr, ver_tbl[2], &ce9);
	
	cf8.data = ver_tbl[5]->data;
	cf8.weight = 8;
	graph_ins_edge(gr, ver_tbl[2], &cf8);
	
	// edges for vertex d
	MstVertex db2, df7;
	
	db2.data = ver_tbl[1]->data;
	db2.weight = 2;
	graph_ins_edge(gr, ver_tbl[3], &db2);
	
	df7.data = ver_tbl[5]->data;
	df7.weight = 7;
	graph_ins_edge(gr, ver_tbl[3], &df7);
	
	// edges for vertex e
	MstVertex ec9, ef1;
	
	ec9.data = ver_tbl[2]->data;
	ec9.weight = 9;
	graph_ins_edge(gr, ver_tbl[4], &ec9);
	
	ef1.data = ver_tbl[5]->data;
	ef1.weight = 1;
	graph_ins_edge(gr, ver_tbl[4], &ef1);
	
	// edges for vertex f
	MstVertex fb4, fc8, fd7, fe1;
	
	fb4.data = ver_tbl[1]->data;
	fb4.weight = 4;
	graph_ins_edge(gr, ver_tbl[5], &fb4);
	
	fc8.data = ver_tbl[2]->data;
	fc8.weight = 8;
	graph_ins_edge(gr, ver_tbl[5], &fc8);
	
	fd7.data = ver_tbl[3]->data;
	fd7.weight = 7;
	graph_ins_edge(gr, ver_tbl[5], &fd7);
	
	fe1.data = ver_tbl[4]->data;
	fe1.weight = 1;
	graph_ins_edge(gr, ver_tbl[5], &fe1);
	
	for (i = 0; i < ITEMS; ++i)
		ADJ_LIST(i);

	VERTICES;
	EDGES;
	
	puts("\t     a");
	puts("\t    / \\");
	puts("\t  7/   \\4");
	puts("\t  /  6  \\");
	puts("\t b-------c");
	puts("\t |\\     /|");
	puts("\t |4\\   /8|");
	puts("\t2|  \\ /  |9");
	puts("\t |   f   |");
	puts("\t |7/   \\1|");
	puts("\t d       e");
	
	if (min_span_tree(gr, ver_tbl[0], retl, compar) < 0)
	{
		puts("Error in min_span_tree()");
		goto end;
	}
	
	puts("Result:");
	list_apply_all(retl, print_ret_list, NULL);
	list_destroy(retl);

	puts("\t     a");
	puts("\t      \\");
	puts("\t       \\4");
	puts("\t     6  \\");
	puts("\t b-------c");
	puts("\t |\\");
	puts("\t |4\\");
	puts("\t2|  \\");
	puts("\t |   f");
	puts("\t |     \\1");
	puts("\t d       e");
	
end:
	puts("Destroying graph");
	graph_destroy(gr);
	
	return 0;
}

int compar(const void * d1, const void * d2)
{
	/* classic compar */
	int * i1, * i2;
	i1 = (int *)((MstVertex *)d1)->data;
	i2 = (int *)((MstVertex *)d2)->data;
	
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
	MstVertex * bp = (MstVertex *)data;
	
	free(bp->data);
	free(bp);
	return;
}

MstVertex * make_vertex(int num)
{
	/* make new vertex */
	MstVertex * mstv;
	int * i;
	
	if ((i = (int *)malloc(sizeof(*i))) == NULL)
		return NULL;
	
	if ((mstv = (MstVertex *)malloc(sizeof(*mstv))) == NULL)
		return NULL;
	
	*i = num + 'a';
	mstv->data = i;
	mstv->color = -1;
	
	return mstv;
}

void print_adj_list(Arrlist * list)
{
	/* print the adjacency list */
	int i;
	MstVertex * mst;
	ArrlistElmt * ale;
	for (i = 0; i < list->list_size; ++i)
	{	
		ale = arrlist_get_elmt(list, i);
		mst = (MstVertex *)ale;
		printf("\nv = %c, w = %.2f", *((int *)mst->data), mst->weight);
	}
		
	putchar('\n');
	return;
}

void print_ret_list(ListElmt * element, void * args)
{
	/* print the returned list */
	MstVertex * mst_vertex = (MstVertex *)element->data;
	printf("vertex: %c, parent: %c, cost: %.2f\n", 
			*((int *)mst_vertex->data),
			(mst_vertex->parent == NULL) ? '_' : *((int *)mst_vertex->parent->data),
			mst_vertex->key);
	return;
}