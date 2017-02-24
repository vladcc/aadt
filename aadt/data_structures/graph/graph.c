/* graph.c -- graph implementation */
/* v1.0 */
#include <string.h>
#include "graph.h"

void graph_init(Graph * graph, int (*compar)(const void * k1, const void *k2),
				void (*destroy)(void * data))
{
	/* initialize the graph */
	graph->vcount = 0;
	graph->ecount = 0;
	graph->compar = compar;
	graph->destroy = destroy;
	
	// initialize the list of the AdjList structure
	list_init(&graph->adjlists, NULL, NULL);
	
	return;
}

void graph_destroy(Graph * graph)
{
	/* destroy the graph */
	AdjList * adjlist;
	
	// destroy each adjlist
	while (graph->adjlists.size > 0)
	{
		if (list_rem_next(&graph->adjlists, NULL, (void **)&adjlist) == 0)
		{
			set_destroy(&adjlist->adjacent);
			
			if (graph->destroy != NULL)
				graph->destroy(adjlist->vertex);
			
			free(adjlist);
		}
	}
	
	// destroy the adjacency lists
	list_destroy(&graph->adjlists);
	
	// zero out memory
	memset(graph, 0, sizeof(*graph));
	
	return;
}

int graph_ins_vertex(Graph * graph, const void * data)
{
	/* insert a vertex in the graph */
	ListElmt * element;
	AdjList * adjlist;
	int retval;
	
	// don't insert duplicate vertecies
	for (element = (&graph->adjlists)->head; element != NULL; element = element->next)
	{
		if (graph->compar(data, ((AdjList *)element->data)->vertex) == 0)
			return 1;
	}
	
	// insert vertex
	if ((adjlist = (AdjList *)malloc(sizeof(*adjlist)) ) == NULL)
		return -1;
	
	adjlist->vertex = (void *)data;
	
	set_init(&adjlist->adjacent, graph->compar, NULL);
	
	if ((retval = list_ins_next(&graph->adjlists, (&graph->adjlists)->tail,
		 adjlist)) != 0)
		 return retval;
	
	// adjust the vertex count
	graph->vcount++;
	
	return 0;
}

int graph_ins_edge(Graph * graph, const void * v1, const void * v2)
{
	/* insert an edge in a vertex */
	ListElmt * element;
	int retval;
	
	// don't insert an edge without both vertices in the graph
	for (element = (&graph->adjlists)->head; element != NULL; element = element->next)
	{
		if (graph->compar(v2, ((AdjList *)element->data)->vertex) == 0)
			break;
	}
	
	if (NULL == element)
		return -1;
	
	for (element = (&graph->adjlists)->head; element != NULL; element = element->next)
	{
		if (graph->compar(v1, ((AdjList *)element->data)->vertex) == 0)
			break;
	}
	
	if (NULL == element)
		return -1;
	
	// insert the second vertex into the adjacency list of the first vertex
	if ((retval = set_insert(&((AdjList *)element->data)->adjacent, v2)) != 0)
		return retval;
	
	// update the edge count
	graph->ecount++;
	
	return 0;
}

int graph_rem_vertex(Graph * graph, void ** data)
{
	/* remove a vertex from the graph */
	ListElmt * element, * temp, * prev;
	AdjList * adjlist;
	int found;
	
	// traverse each adjacency list and the vertecies it contains
	prev = NULL;
	found = 0;
	
	for (element = (&graph->adjlists)->head; element != NULL; element = element->next)
	{
		// do not allow removal of the vertex if it is in an adjacency list
		if (set_is_member(&((AdjList *)element->data)->adjacent, *data) >= 0)
			return -1;
	
		// keep a pointer to the vertex to be removed
		if (graph->compar(*data, ((AdjList *)element->data)->vertex) == 0)
		{
			temp = element;
			found = 1;
		}
		
		// keep a pointer to the vertex before the one to be removed
		if (!found)
			prev = element;
	}
	// return if the vertex was not found
	if (!found)
		return -1;
	
	// don't remove the vertex if it's adjacent to something
	if ((&((AdjList *)temp->data)->adjacent)->list_size > 0)
		return -1;
	
	// remove the vertex
	if (list_rem_next(&graph->adjlists, prev, (void **)&adjlist) != 0)
		return -1;
	
	// free the memory
	*data = adjlist->vertex;
	free(adjlist);
	
	// update graph size
	graph->vcount--;
	
	return 0;
}

int graph_rem_edge(Graph * graph, const void * v1, const void ** v2)
{
	/* remove an edge from a vertex */
	ListElmt * element;
	
	// locate the adjacency list for the first vector
	for (element = (&graph->adjlists)->head; element != NULL; element = element->next)
	{
		if (graph->compar(v1, ((AdjList *)element->data)->vertex) == 0)
			break;
	}
	
	if (NULL == element)
		return -1;
	
	// remove the second vertex from the adjacency list of the first one
	if (set_remove(&((AdjList *)element->data)->adjacent, (void **)v2) != 0)
		return -1;
	
	// update edge count
	graph->ecount--;
	
	return 0;
}

int graph_adjlist(const Graph * graph, const void * v, AdjList ** adjlist)
{
	/* retrieve an adjacency list for a vertex */
	ListElmt * element;//, * prev;
	
	//prev = NULL;
	// locate the adjacency list
	for (element = (&graph->adjlists)->head; element != NULL; element = element->next)
	{
		if (graph->compar(v, ((AdjList *)element->data)->vertex) == 0)
			break;
		
		//prev = element;
	}
	
	if (NULL == element)
		return -1;
	
	// pass back the list
	*adjlist = element->data;
	
	return 0;
}

int graph_is_adjacent(const Graph * graph, const void * v1, const void * v2)
{
	/* find out if two vertices are adjacent */
	ListElmt * element;//, * prev;
	
	// prev = NULL;
	// locate the adjacency list of the first vertex
	for (element = (&graph->adjlists)->head; element != NULL; element = element->next)
	{
		if (graph->compar(v1, ((AdjList *)element->data)->vertex) == 0)
			break;
		
		// prev = element;
	}
	
	// not found
	if (NULL == element)
		return 0;
	
	// return whether the second vertex is in the adjacency list of the first one
	return set_is_member(&((AdjList *)element->data)->adjacent, v2);
}