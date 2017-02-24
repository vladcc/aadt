/* shortest_paths.h -- shortest paths public interface */
#ifndef SHORTEST_PATHS_H
#define SHORTEST_PATHS_H

#include "graph.h"

typedef struct PathVertex_ {
	void * data;
	double weight;
	VertexColor color;
	double shortest;
	struct PathVertex_ * parent;
} PathVertex;

int shortest_paths(Graph * graph, const PathVertex * start, List * paths,
					int (*compar)(const void * k1, const void * k2));
/*
returns: 0 on success, -1 otherwise

description: Computes shortest paths between start and all other vertices in a
directed, weighted graph specified by graph. The operation modifies graph.
Each vertex in graph must be a PathVertex. Assign a weight to each edge by setting 
the weight member of the PathVertex passed as v2 to graph_ins_edge(). Use the data 
member of each PathVertex to store data associated with the vertex. The compar() 
function is user defined and should compare only the data members of the PathVertex 
structures. Information about the shortest paths is returned in paths, which is a list 
of PathVertex structures. In paths, the vertex with parent == NULL is the start vertex.
The vertices in paths point to the actual vertices in graph.

complexity: O(edges * vertices^2)
*/					
#endif