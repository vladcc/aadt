/* shortest_paths.c -- shortest paths implementation */
#include <float.h>
#include <stdlib.h>
#include "graph.h"
#include "shortest_paths.h"

static void relax(PathVertex * from, PathVertex * to, double edge_weight)
{
	// relax an edge between from and to
	if (to->shortest > from->shortest + edge_weight)
	{
		to->shortest = from->shortest + edge_weight;
		to->parent = from;
	}
	
	return;
}

int shortest_paths(Graph * graph, const PathVertex * start, List * paths,
					int (*compar)(const void * k1, const void * k2))
{
	AdjList * adjlist;
	PathVertex * pth_vertex, * adj_vertex;
	ListElmt * element;
	double minimum;
	int found, i, j;
	
	found = 0;
	for (element = (&graph->adjlists)->head; element != NULL; element = element->next)
	{
		pth_vertex = ((AdjList *)element->data)->vertex;
		
		if (graph->compar(pth_vertex, start) == 0)
		{
			// initialize the start vertex
			pth_vertex->color = white;
			pth_vertex->shortest = 0;
			pth_vertex->parent = NULL;
			found = 1;
		}
		else
		{
			// initialize the others
			pth_vertex->color = white;
			pth_vertex->shortest = DBL_MAX;
			pth_vertex->parent = NULL;
		}
	}
	
	// return if the start vertex was not found
	if (!found)
		return -1;
	
	// use the shortest paths
	i = 0;
	while (i < graph->vcount)
	{
		minimum = DBL_MAX;
		for (element = (&graph->adjlists)->head; element != NULL; element = element->next)
		{
			pth_vertex = ((AdjList *)element->data)->vertex;
			if (white == pth_vertex->color && pth_vertex->shortest < minimum)
			{
				minimum = pth_vertex->shortest;
				adjlist = element->data;
			}
		}
		
		// color the selected vertex black
		((PathVertex *)adjlist->vertex)->color = black;
		
		// traverse each vertex adjacent to the selected
		for (j = 0; j < (&adjlist->adjacent)->list_size; ++j)
		{
			adj_vertex = arrlist_get_elmt(&adjlist->adjacent, j);
			
			// find the adjacent vertex in the adjacency list
			for (element = (&graph->adjlists)->head; element != NULL; element = element->next)
			{
				pth_vertex = ((AdjList *)element->data)->vertex;
				if (graph->compar(pth_vertex, adj_vertex) == 0)
				{
					// relax the adjacent vertex
					relax(adjlist->vertex, pth_vertex, adj_vertex->weight);
				}
			}
		}
		
		// prepare for the next vertex
		++i;
	}
	
	// load the minimum spanning tree into a list
	list_init(paths, NULL, NULL);
	
	for (element = (&graph->adjlists)->head; element != NULL; element = element->next)
	{
		// load each black vertex
		pth_vertex = ((AdjList *)element->data)->vertex;
		if (black == pth_vertex->color)
		{
			if (list_ins_next(paths, paths->tail, pth_vertex) != 0)
			{
				list_destroy(paths);
				return -1;
			}
		}
	}
	
	return 0;
}