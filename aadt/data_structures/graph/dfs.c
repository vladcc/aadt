/* dfs.c -- depth first search implementation */
#include <stdlib.h>
#include "dfs.h"
#include "graph.h"
#include "list.h"

static int dfs_main(Graph * graph, AdjList * adjlist, List * ordered)
{
	/* depth traversal of adjlist */
	int i;
	AdjList * clr_adjlist;
	DfsVertex * clr_vertex, * adj_vertex;
	Arrlist * arlp;
	
	// color the vertex gray and traverse its adjacency list
	((DfsVertex *)adjlist->vertex)->color = gray;
	arlp = &(adjlist->adjacent);
	
	for (i = 0; i < arlp->list_size; ++i)
	{
		adj_vertex = arrlist_get_elmt(arlp, i);
		
		if (graph_adjlist(graph, adj_vertex, &clr_adjlist) != 0)
			return -1;
		
		clr_vertex = clr_adjlist->vertex;
		
		// move one vertex deeper when the next adjacent vertex is white
		if (white == clr_vertex->color)
		{
			if (dfs_main(graph, clr_adjlist, ordered) != 0)
				return -1;
		}
	}
	
	// color the current vertex black and make it the first in the list
	((DfsVertex *)adjlist->vertex)->color = black;
	
	if (list_ins_next(ordered, NULL, (DfsVertex *)adjlist->vertex) != 0)
		return -1;
	
	return 0;
}

int dfs(Graph * graph, List * ordered)
{
	/* depth-first search */
	DfsVertex * vertex;
	ListElmt * element;
	
	// initialize all of the vertices in the graph
	for (element = (&graph->adjlists)->head; element != NULL; element = element->next)
	{
		vertex = ((AdjList *)element->data)->vertex;
		vertex->color = white;
	}
	
	// perform depth first search
	list_init(ordered, NULL, NULL);
	for (element = (&graph->adjlists)->head; element != NULL; element = element->next)
	{
		vertex = ((AdjList *)element->data)->vertex;
		if(white == vertex->color)
		{
			if (dfs_main(graph, (AdjList *)element->data, ordered) != 0)
			{
				list_destroy(ordered);
				return -1;
			}
		}
	}
	
	return 0;
}