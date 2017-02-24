/* bfs.c -- breadth-first search implementation */

#include <stdlib.h>
#include "bfs.h"
#include "graph.h"
#include "list.h"
#include "queue.h"
#include "arrlist.h"

int bfs(Graph * graph, BfsVertex * start, List * hops)
{
	/* breadth-first search */
	Queue queue_;
	Queue * queue = &queue_;
	AdjList * adjlist, *clr_adjlist;
	BfsVertex * clr_vertex, * adj_vertex;
	ListElmt * element;
	
	// initialize all of the vertices in the graph
	for (element = (&graph->adjlists)->head; element != NULL; element = element->next)
	{
		clr_vertex = ((AdjList *)element->data)->vertex;

		if (graph->compar(clr_vertex, start) == 0)
		{
			// init start vertex
			clr_vertex->color = gray;
			clr_vertex->hops = 0;
		}
		else
		{
			// all other than the start
			clr_vertex->color = white;
			clr_vertex->hops = -1;
		}
	}
	
	// init the queue with the adj list of the start vertex
	queue_init(queue, NULL);
	
	if (graph_adjlist(graph, start, &clr_adjlist) != 0)
	{
		queue_destroy(queue);
		return -1;
	}
	
	if (queue_enq(queue, clr_adjlist) != 0)
	{
		queue_destroy(queue);
		return -1;
	}
	
	// perform breadth-first search
	while (queue->size > 0)
	{
		adjlist = queue_peek(queue);
		
		// travese each vertex in the current adjacency list
		int i;
		for (i = 0; i < (&adjlist->adjacent)->list_size; ++i)
		{
			adj_vertex = arrlist_get_elmt(&adjlist->adjacent, i);
			
			// determine the color of the next adjacent vertex
			if (graph_adjlist(graph, adj_vertex, &clr_adjlist) != 0)
			{
				queue_destroy(queue);
				return -1;
			}
			
			clr_vertex = clr_adjlist->vertex;
			// color each vertex gray and enqueue its adjlist
			if (white == clr_vertex->color)
			{
				clr_vertex->color = gray;
				clr_vertex->hops = ((BfsVertex *)adjlist->vertex)->hops + 1;
				
				if (queue_enq(queue, clr_adjlist) != 0)
				{
					queue_destroy(queue);
					return -1;
				}
			}
		}
		// deq the current adj list and color its vertex black
		if (queue_deq(queue, (void **)&adjlist) == 0)
			((BfsVertex *)adjlist->vertex)->color = black;
		else
		{
			queue_destroy(queue);
			return -1;
		}
	}
	queue_destroy(queue);
	
	// pass back the hop count for each vertex in a list
	list_init(hops, NULL, NULL);
	for (element = (&graph->adjlists)->head; element != NULL; element = element->next)
	{
		// skip verticies that were not visited
		clr_vertex = ((AdjList *)element->data)->vertex;
		if (clr_vertex->hops != -1)
		{
			if (list_ins_next(hops, hops->tail, clr_vertex) != 0)
			{
				list_destroy(hops);
				return -1;
			}
		}
	}
	
	return 0;
}