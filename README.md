# aadt
Algorithms and abstract data types in C

Hi.

This is the result of my study of algorithms and abstract data types.
All of them are working, tested, and documented. The tests for the
data types are documented as well. For the study I have used
"Mastering Algorithms with C" by Kyle Loudon. The resulting code here
is NOT the same as the code in the book, as I have created some data
types of my own and implemented some of the things differently from Loudon.

All of the data types along with the comparison sorting algorithms work
with void pointers, allowing for general use. The testing is done by executing
every function of the data type with wrong, regular, and edge case input
where applicable. If other testing methods are used, they are describer below.

Here's a list of what you will find here.

Data types

1. Singly linked list 	- plain old linked list with added sorting (albeit naive) in both directions,
searching, and traversing with a user defined function.

2. Circular list 		- a circular linked list with added sorting in both directions, searching 
and traversing with a user defined function.

3. Doubly linked list 	- a doubly linked list with added sorting, searching and traversing with a user 
defined function in both directions.

4. Array list			- this one is not in the book. It's a list type implemented as a dynamic array.
This allows for using qsort() from the stdlib. Includes sorting, binary search, linear search, and traversing
with a user defined function. When the data array fills up, it's reallocated and doubled in size.

5. Queue				- a queue data type. Unlike the one in the book, it's standalone and not implemented 
as a linked list.

6. Stack				- a stack data type. Unlike the one in the book, it's standalone and not implemented 
as a linked list.

7. Set					- a set implementation done with an array list instead of a linked list.

8. Hash table			- a chained hash table implemented as an array of linked lists. It's tested by obtaining 
uniqe words from a text and arranging them in alphabetical order.

9. Binary tree			- a binary tree implementation. It's tested by parsing a fully parenthesized expression
and traversing it preorder, inorder, and postorder, and displaying it in prefix, infix, and posfix notations respectivelly.

10. AVL tree			- an AVL binary search tree.

11. Heap				- a heap implemented as an array instead of a binary tree.

12. Graph				- a graph data type implemented with sets and lists.
Note: The arrlist.c in the graph directory is different than the standalone arrlist.c. Because of the use of qsort()
from the stdlib, the standalone version needs to pass argumets to compar() as pointers to the pointers pointing to
the data, rather than pointers pointing directly to the data.
Along with the general tests, the graph is tested with breadth-first search(counting network hops) and 
depth-first search(topological order/dependency sorting) algorithms. Visualisation is provided for convenience.


Algorithms

Sorting:
- Comparison sorts

1. Bubble sort(not in the book)
2. Insertion sort
3. Quicksort
4. Merge sort

- Counting sorts

1. Counting sort
2. Radix sort

The testing module acceps as an argument from the command line a singed int number > 0, then
generates an array of that many numbers in descending order, then runs every sorting algorithm
mentioned above along with qsort() from the stdlib, times them, and displays the results. 
For values above 20 000 bubble sort and insertion sort are not evoked, as they begin to take too long.

Searching:

1. Binary search

Graph algorithms:

1. Minimum spanning tree
2. Shortest paths
3. The Traveling Salesman problem - implemented without a graph, with a coordinate system and nearest 
neighbor heuristic, running in polynomial time but providing approximate results.
In all graph algorithms visualisation is provded for convenience.


All of the above is going to be of help to anyone interested in computer science and C. It sure was
of help to me. All examples here can be used in the real world. If you are interested in learning more 
about the theory behind them, get Kyle's book. It includes all kinds of other cool stuff not listed here, 
like encryption and data compression.
