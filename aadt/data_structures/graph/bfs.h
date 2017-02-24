/* bfs.h -- breadth-first search header */

#ifndef BFS_H
#define BFS_H

#include "graph.h"
#include "list.h"

// breadth-first search structure
typedef struct BfsVertex_ {
	void * data;
	VertexColor color;
	int hops;
} BfsVertex;

int bfs(Graph * graph, BfsVertex * start, List * hops);
/*
returns: 0 on success, -1 otherwise; the hop count for every vertex in the list
specified by hops

description: performs a breadth-first search on the graph specified by graph

complexity: O(vertices + edges)
*/

#endif