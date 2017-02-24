#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include "bfs.h"

#define ITEMS 6

#define ADJ_LIST(n)	do {void * vp; AdjList * adjlp; int ret;\
						vp = ver_tbl[(n)-1];\
						ret = graph_adjlist(gr, vp, &adjlp);\
						if (0 == ret){\
							printf("Vertex %d is adjacent to: ", (n));\
							print_list((Arrlist *)(&adjlp->adjacent));}} while (0)
								
#define EDGES 		printf("Edges: %d\n", gr->ecount)
#define VERTICES	printf("Vertices: %d\n", gr->vcount)

int compar(const void * d1, const void * d2);
void destroy(void * data);
BfsVertex * make_vertex(int num);
void print_list(Arrlist * list);

int main(void)
{
	/* count network hops */
	Graph gr_;
	Graph * gr = &gr_;
	List retl_;
	List * retl = &retl_;
	BfsVertex * ver_tbl[ITEMS];
	
	puts("Initializing undirected graph");
	puts("Vertices are hosts, edges are connections between them");
	graph_init(gr, compar, destroy);
	
	printf("Insterting %d vertices\n", ITEMS);
	int i;
	for (i = 0; i < ITEMS; ++i)
	{
		ver_tbl[i] = make_vertex(i+1);
		if (graph_ins_vertex(gr, ver_tbl[i]) < 0)
			return -1;
	}
	
	puts("Inserting edges:");
	graph_ins_edge(gr, ver_tbl[0], ver_tbl[1]);
	graph_ins_edge(gr, ver_tbl[1], ver_tbl[0]);
	graph_ins_edge(gr, ver_tbl[0], ver_tbl[2]);
	graph_ins_edge(gr, ver_tbl[2], ver_tbl[0]);

	graph_ins_edge(gr, ver_tbl[1], ver_tbl[2]);
	graph_ins_edge(gr, ver_tbl[2], ver_tbl[1]);
	graph_ins_edge(gr, ver_tbl[1], ver_tbl[3]);
	graph_ins_edge(gr, ver_tbl[3], ver_tbl[1]);
	
	graph_ins_edge(gr, ver_tbl[2], ver_tbl[4]);
	graph_ins_edge(gr, ver_tbl[4], ver_tbl[2]);
	
	graph_ins_edge(gr, ver_tbl[3], ver_tbl[4]);
	graph_ins_edge(gr, ver_tbl[4], ver_tbl[3]);
	
	graph_ins_edge(gr, ver_tbl[4], ver_tbl[5]);
	graph_ins_edge(gr, ver_tbl[5], ver_tbl[4]);
	
	VERTICES;
	EDGES;
	
	for (i = 0; i < ITEMS; ++i)
		ADJ_LIST(i+1);
	
	printf("\t%4c\n", '1');
	printf("\t%3c %c\n", '/', '\\');
	printf("\t%2c - %c\n", '2', '3');
	printf("\t%2c   %c\n", '|', '|');
	printf("\t%2c - %c\n", '4', '5');
	printf("\t%2c   %c\n", ' ', '|');
	printf("\t%2c   %c\n", ' ', '6');
	
	puts("Breadth-first search...");
	puts("Shortest hop counts:");
	bfs(gr, ver_tbl[0], retl);

	ListElmt * tmp;
	for (tmp = retl->head; tmp != NULL; tmp = tmp->next)
	{
		BfsVertex * bvtmp = tmp->data;
		printf("vertex: %d, hops: %d\n", *((int *)bvtmp->data), bvtmp->hops);
	}
	list_destroy(retl);
	
	puts("Destroying graph");
	graph_destroy(gr);
	puts("Done");
	
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
	BfsVertex * bp = (BfsVertex *)data;
	
	free(bp->data);
	free(bp);
	return;
}

void print_list(Arrlist * list)
{
	/* print the adjacency list */
	int i;
	BfsVertex * bfs;
	ArrlistElmt * ale;
	for (i = 0; i < list->list_size; ++i)
	{	
		ale = arrlist_get_elmt(list, i);
		bfs = (BfsVertex *)ale;
		printf("%d ", *((int *)bfs->data));
	}
		
	putchar('\n');
	return;
}

BfsVertex * make_vertex(int num)
{
	/* make new vertex */
	BfsVertex * bfsv;
	int * i;
	
	if ((i = (int *)malloc(sizeof(*i))) == NULL)
		return NULL;
	
	if ((bfsv = (BfsVertex *)malloc(sizeof(*bfsv))) == NULL)
		return NULL;
	
	*i = num;
	bfsv->data = i;
	bfsv->color = -1;
	bfsv->hops = -1;
	
	return bfsv;
}