/* traveling_salesman.c -- traveling salesman implementation */
#include <math.h>
#include <float.h>
#include <stdlib.h>
#include "list.h"
#include "traveling_salesman.h"

int traveling_salesman(List * vertices, const TspVertex * start, List * tour,
						int (*compar)(const void * k1, const void * k2))
{
	TspVertex * tsp_vertex, * tsp_start, * selection;
	ListElmt * element;
	double minimum, distance, x, y;
	int found, i;
	
	// initialize the list for the tour
	list_init(tour, NULL, NULL);
		
	// initialize the vertices
	found = 0;
	for (element = vertices->head; element != NULL; element = element->next)
	{
		tsp_vertex = element->data;
		
		if (compar(tsp_vertex, start) == 0)
		{
			// start the tour at the start vertex
			if (list_ins_next(tour, tour->tail, tsp_vertex) != 0)
			{
				list_destroy(tour);
				return -1;
			}
			
			// save the start vertex and its coordinates
			tsp_start = tsp_vertex;
			x = tsp_vertex->x;
			y = tsp_vertex->y;
			
			// color it black
			tsp_vertex->color = black;
			found = 1;
		}
		else
		{
			// color all other vertices white
			tsp_vertex->color = white;
		}
	}
	
	if (!found)
	{
		list_destroy(tour);
		return -1;
	}

	// use the nearest neighbor heuristic to compute the tour
	i = 0;
	while (i < (vertices->size - 1) )
	{
		// get the white vertex closest to the previous vertex in the tour
		minimum = DBL_MAX;
		for (element = vertices->head; element != NULL; element = element->next)
		{
			tsp_vertex = element->data;
			
			if (white == tsp_vertex->color)
			{
				// remember the Pythagorean theorm
				distance = sqrt(pow(tsp_vertex->x - x, 2.0) + pow(tsp_vertex->y - y, 2.0));
				
				if (distance < minimum)
				{
					minimum = distance;
					selection = tsp_vertex;
				}
			}
		}
		
		// save the coordinates of the nearest vertex
		x = selection->x;
		y = selection->y;
		
		// color it black
		selection->color = black;
		
		// insert in the tour
		if (list_ins_next(tour, tour->tail, selection) != 0)
		{
			list_destroy(tour);
			return -1;
		}
		
		// go to the next vertex
		++i;
	}
	
	// insert the start vertex again to complete the tour
	if (list_ins_next(tour, tour->tail, tsp_start) != 0)
	{
		list_destroy(tour);
		return -1;
	}
	
	return 0;
}