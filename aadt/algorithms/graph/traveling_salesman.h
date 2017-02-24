/* traveling_salesman.h -- traveling salesman public interface */
#ifndef TRAVEL_SALES_H
#define TRAVEL_SALES_H

#include "list.h"

// define colors
typedef enum VertexColor_ {white, gray, black} VertexColor;

typedef struct TspVertex_ {
	void * data;
	double x, y;
	VertexColor color;
} TspVertex;

int traveling_salesman(List * vertices, const TspVertex * star, List * tour,
						int (*compar)(const void * k1, const void * k2));
/*
returns: 0 on success, -1 otherwise

description: Computes an approximate traveling salesman tour of the points specified in
vertices using the nearest neighbor heuristic. The opeartion modifies vertices. Each
element in vertices must be of type TspVertex. In TspVertex data contains the ID of the
vertex and x and y it's coordinates. compar() is user defined and should compare 
only the data fileds of the vertices. The tour is returned in tour, which is a list of 
TspVertex structures in the order they would be travelled. The elements in tour point to 
the actual vertices in the graph.

complexity: O(vertices^2)

note: The vertices are implied, since you can travel to any vertex from any other. 
This means we don't even need a graph, only a list of points and their coordinates.
*/
#endif