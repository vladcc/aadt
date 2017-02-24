#include <stdio.h>
#include "traveling_salesman.h"
#include "list.h"

#define X 7
#define Y 7
#define POINTS 7
#define NUL '\0'

void init_points(TspVertex * pnts);
void print_field(const char (* field)[Y], int rows, int cols);
int compar(const void * d1, const void * d2);

int main(void)
{
	/* test the traveling salesman solution */
	static char field[Y][X];
	static TspVertex pnts[POINTS];
	int i, j;
	List vertices_, tour_;
	List * vertices = &vertices_;
	List * tour = &tour_;
	ListElmt * ple;
	
	for (i = 0; i < Y; ++i)
		for (j = 0; j < X; ++j)
			field[i][j] = '-';
	
	init_points(pnts);
	for (i = 0; i < POINTS; ++i)
		field[(Y - 1) - ((int)pnts[i].y)][(int)pnts[i].x] = *((char *)pnts[i].data);
	
	for (i = 0; i < Y; ++i)
		printf("point = %c; x,y = %d, %d\n", *((char *)pnts[i].data), 
												(int)pnts[i].x, (int)pnts[i].y);
	
	putchar('\n');
	print_field(field, Y, X);
	
	list_init(vertices, NULL, NULL);
	for (i = 0; i < Y; ++i)
	{
		if (list_ins_next(vertices, vertices->tail, &pnts[i]) != 0)
		{
			list_destroy(vertices);
			return -1;
		}
	}
	
	
	if (traveling_salesman(vertices, &pnts[0], tour, compar) != 0)
	{
		puts("Err: calculation failed");
		return -1;
	}
	
	printf("Approximate shortest route: ");
	for (ple = tour->head; ple != NULL; ple = ple->next)
		printf("%c ", *((char *)((TspVertex *)ple->data)->data));
	
	putchar('\n');
	
	list_destroy(tour);
	list_destroy(vertices);
	
	return 0;
}

void print_field(const char (* field)[Y], int rows, int cols)
{
	/* print coordinate filed */
	int i, j;
	static char ch = '^';
	
	for (i = 0; i < rows; ++i)
	{
		printf("\t%c", ch);
		ch = '|';
		putchar('0' + rows - i - 1);
		for (j = 0; j < cols; ++j)
			putchar(field[i][j]);
		
		putchar('\n');
	}
	
	printf("\ty ");
	for (i = 0; i < rows; ++i)
		putchar('0' + i);
	
	putchar('\n');
	
	puts("\t x------>");
	return;
}

void init_points(TspVertex * pnts)
{
	/* create the points */
	static char ids[] = {"abcdefg"};
	int i;
	
	for (i = 0; i < Y; ++i)
		pnts[i].data = &ids[i];
	
	pnts[0].x = 2;
	pnts[0].y = 1;
	
	pnts[1].x = 5;
	pnts[1].y = 2;
	
	pnts[2].x = 1;
	pnts[2].y = 3;
	
	pnts[3].x = 4;
	pnts[3].y = 3;
	
	pnts[4].x = 6;
	pnts[4].y = 3;
	
	pnts[5].x = 2;
	pnts[5].y = 4;
	
	pnts[6].x = 5;
	pnts[6].y = 5;
	
	return;
}

int compar(const void * d1, const void * d2)
{
	/* classic compar */
	int * i1, * i2;
	i1 = (int *)((TspVertex *)d1)->data;
	i2 = (int *)((TspVertex *)d2)->data;
	
	if (*i1 < *i2)
		return -1;
	else if (*i1 > *i2)
		return 1;
	else
		return 0;
}