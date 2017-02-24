/* dfs.h -- depth first search interface */
#ifndef DFS_H
#define DFS_H

#include "graph.h"
#include "list.h"

typedef struct DfsVertex_ {
	void * data;
	VertexColor color;
} DfsVertex;

int dfs(Graph * graph, List * ordered);
/*
returns: 0 on success, -1 otherwise; topologically sorted vertecies of graph
in the list specified by ordered

description: performs a depth-first search for the graph specified by graph

complexity: O(vertices + edges)
*/

#endif