#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include "dfs.h"

#define ITEMS 7

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
DfsVertex * make_vertex(int num);
void print_list(Arrlist * list);

int main(void)
{
	/* topological sorting */
	Graph gr_;
	Graph * gr = &gr_;
	List retl_;
	List * retl = &retl_;
	DfsVertex * ver_tbl[ITEMS];
	
	puts("Initializing acyclic directed (precedence) graph");
	puts("Vertices are tasks, edges are dependencies between them");
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
	graph_ins_edge(gr, ver_tbl[1], ver_tbl[2]);
	graph_ins_edge(gr, ver_tbl[2], ver_tbl[5]);
	graph_ins_edge(gr, ver_tbl[3], ver_tbl[2]);
	graph_ins_edge(gr, ver_tbl[3], ver_tbl[4]);
	graph_ins_edge(gr, ver_tbl[4], ver_tbl[5]);
	VERTICES;
	EDGES;
	
	for (i = 0; i < ITEMS; ++i)
		ADJ_LIST(i+1);
	
	printf("\t%c %4c %3c\n", '1', '4', '7');
	printf("\t%c %3c%c\n", '|', '/', '|');
	printf("\t%c  %c %c\n", '2', '/', '5');
	printf("\t%c %c  %c\n", '|', '/', '|');
	printf("\t%c----%c\n", '3', '6');

	
	puts("Depth-first search...");
	dfs(gr, retl);

	ListElmt * tmp;
	puts("Topological order:");
	for (tmp = retl->head; tmp != NULL; tmp = tmp->next)
	{
		DfsVertex * dvtmp = tmp->data;
		printf("%d ", *((int *)dvtmp->data));
	}
	putchar('\n');
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
	DfsVertex * bp = (DfsVertex *)data;
	
	free(bp->data);
	free(bp);
	return;
}

void print_list(Arrlist * list)
{
	/* print the adjacency list */
	int i;
	DfsVertex * bfs;
	ArrlistElmt * ale;
	for (i = 0; i < list->list_size; ++i)
	{	
		ale = arrlist_get_elmt(list, i);
		bfs = (DfsVertex *)ale;
		printf("%d ", *((int *)bfs->data));
	}
		
	putchar('\n');
	return;
}

DfsVertex * make_vertex(int num)
{
	/* make new vertex */
	DfsVertex * dfsv;
	int * i;
	
	if ((i = (int *)malloc(sizeof(*i))) == NULL)
		return NULL;
	
	if ((dfsv = (DfsVertex *)malloc(sizeof(*dfsv))) == NULL)
		return NULL;
	
	*i = num;
	dfsv->data = i;
	dfsv->color = -1;
	
	return dfsv;
}