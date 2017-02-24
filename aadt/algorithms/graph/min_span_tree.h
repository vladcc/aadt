/* min_span_tree.h -- public interface for minimum spanning tree */
#ifndef MIN_SPAN_TREE_H
#define MIN_SPAN_TREE_H

#include "graph.h"

typedef struct MstVertex_ {
	void * data;
	double weight;
	VertexColor color;
	double key;
	struct MstVertex_ * parent;
} MstVertex;

int min_span_tree(Graph * graph, const MstVertex * start, List * span,
					int (*compar)(const void * k1, const void * k2));
/*
returns: 0 if successful, -1 otherwise

description: Computes the minimum spanning tree for an undirected, weighted
graph specified by graph. The minimum spanning tree is computed starting from
start. The operation modifies graph. Each vertex in graph must be a MstVertex.
Assign a weight to each edge by setting the weight member of the MstVertex
passed as v2 to graph_ins_edge(). Use the data member of each MstVertex to
store data associated with the vertex. The compar() function is user defined and
should compare only the data members of the MstVertex structures. Information
about the minimum spanning tree is returned in span, which is a list of MstVertex
structures. In span, the vertex with parent == NULL is the root of the tree.
The vertices in span point to the actual vertices in graph.

complexity: O(edges * vertices ^ 2)
*/
#endif