/* min_span_tree.c -- minimum spanning tree implementation */
#include <float.h>
#include <stdlib.h>
#include "graph.h"
#include "min_span_tree.h"

int min_span_tree(Graph * graph, const MstVertex * start, List * span,
					int (*compar)(const void * k1, const void * k2))
{
	AdjList * adjlist;
	MstVertex * mst_vertex, * adj_vertex;
	ListElmt * element;
	double minimum;
	int found, i, j;
	
	// initialize all vertices
	found = 0;
	for (element = (&graph->adjlists)->head; element != NULL; element = element->next)
	{
		mst_vertex = ((AdjList *)element->data)->vertex;
		
		if (graph->compar(mst_vertex, start) == 0)
		{
			// initialize the start vertex
			mst_vertex->color = white;
			mst_vertex->key = 0;
			mst_vertex->parent = NULL;
			found = 1;
		}
		else
		{
			// initialize the others
			mst_vertex->color = white;
			mst_vertex->key = DBL_MAX;
			mst_vertex->parent = NULL;
		}
	}
	
	// return if the start vertex was not found
	if (!found)
		return -1;
	
	// use the minimum spanning tree algorithm
	i = 0;
	while (i < graph->vcount)
	{
		minimum = DBL_MAX;
		for (element = (&graph->adjlists)->head; element != NULL; element = element->next)
		{
			mst_vertex = ((AdjList *)element->data)->vertex;
			if (white == mst_vertex->color && mst_vertex->key < minimum)
			{
				minimum = mst_vertex->key;
				adjlist = element->data;
			}
		}
		
		// color the selected vertex black
		((MstVertex *)adjlist->vertex)->color = black;
		
		// traverse each vertex adjacent to the selected
		for (j = 0; j < (&adjlist->adjacent)->list_size; ++j)
		{
			adj_vertex = arrlist_get_elmt(&adjlist->adjacent, j);
			
			// find the adjacent vertex in the adjacency list
			for (element = (&graph->adjlists)->head; element != NULL; element = element->next)
			{
				mst_vertex = ((AdjList *)element->data)->vertex;
				if (graph->compar(mst_vertex, adj_vertex) == 0)
				{
					// decide whether to change the key value and parent
					if (white == mst_vertex->color && adj_vertex->weight < mst_vertex->key)
					{
						mst_vertex->key = adj_vertex->weight;
						mst_vertex->parent = adjlist->vertex;
					}
					break;
				}
			}
		}
		
		// prepare for the next vertex
		++i;
	}
	
	// load the minimum spanning tree into a list
	list_init(span, NULL, NULL);
	
	for (element = (&graph->adjlists)->head; element != NULL; element = element->next)
	{
		// load each black vertex
		mst_vertex = ((AdjList *)element->data)->vertex;
		if (black == mst_vertex->color)
		{
			if (list_ins_next(span, span->tail, mst_vertex) != 0)
			{
				list_destroy(span);
				return -1;
			}
		}
	}
	
	return 0;
}

