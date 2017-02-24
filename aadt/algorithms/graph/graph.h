/* graph.h -- graph public interface */
/* v1.0 */
#ifndef GRAPH_H
#define GRAPH_H

#include <stdlib.h>
#include "list.h"
#include "arrlist.h"
/* WARNING: do not use qsort() with this version of arrlist.c; 
 * the standalone arrlist.c uses the same compar() as qsort(), this one doesn't */
#include "set.h"

// adjacency list structure
typedef struct AdjList_ {
	void * vertex;
	Set adjacent;
} AdjList;

// graph structure
typedef struct Graph_ {
	int vcount;
	int ecount;
	int (*compar)(const void * k1, const void *k2);
	void (*destroy)(void * data);
	List adjlists;
} Graph;

// define colors
typedef enum VertexColor_ {white, gray, black} VertexColor;

void graph_init(Graph * graph, int (*compar)(const void * k1, const void *k2),
				void (*destroy)(void * data));
/*
returns: nothing

description: Initializes the graph pointed to by graph. destroy() can be NULL if
data is not to be deallocated after destroying the graph.

complexity: O(1)
*/

void graph_destroy(Graph * graph);
/*
returns: nothing

description: Destroyis the graph.

complexity: O(vertices + edges)
*/

int graph_ins_vertex(Graph * graph, const void * data);
/*
returns: 0 if successful, 1 if the vertex already exists, -1 otherwise

description: Inserts data as a vertex.

complexity: O(vertices)
*/

int graph_ins_edge(Graph * graph, const void * v1, const void * v2);
/*
returns: 0 if successful, 1 if edge already exists, -1 otherwise

description: Inserts an edge from vertex v1 to vertex v2. Both vertices must
have been inserted previously using graph_ins_vertex(). The new edge is
represented with a pointer to v2 in the adjecency list of v1. To enter an
edge(u, v) in an undirected graph, call twice: once from u to v and once from
v to u.

complexity: O(vertices)
*/

int graph_rem_vertex(Graph * graph, void ** data);
/*
returns: 0 on success, -1 otherwise

description: Removes the vertex matching *data from graph. All edges incident
to and from the vertex must have been removed previously with graph_rem_edge().
Upon return, data points to the data stored in the removed vertex.

complexity: O(vertices + edges)
*/

int graph_rem_edge(Graph * graph, const void * v1, const void ** v2);
/*
returns: 0 on success, -1 otherwise

description: Removes the edge from v1 to v2 in the graph. Upon return, v2 points
to the data stored in the adjacency list of the vertex specified by v1.

complexity: O(vertices)
*/

int graph_adjlist(const Graph * graph, const void * v, AdjList ** adjlist);
/*
returns: 0 if adjacency list is retrieved, -1 otherwise

description: Retrieves vertices adjacent to v in graph.  The adjacent vertices
are returned in the form of an AdjList structure. The structure contains the 
vertex matching v and a set of vertices adjacent to it. A pointer to the actual
adjacency list in the graph is returned.

complexity: O(vertices)
*/

int graph_is_adjacent(const Graph * graph, const void * v1, const void * v2);
/*
returns: >= 0 if v2 is adjacent to v1, less than 0 otherwise.

description: Determines whether v2 is adjacent to v1.

complexity: O(vertices)
*/

#endif