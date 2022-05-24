
#ifndef __MST_H__
#define __MST_H__

#include "Abstract_Graph.h"

//@ARG: dyanamic array weighted_edges_list[][3]
//@RET: dynamic array mst_edges_list[][2]
void Kruskal(void* graph, long** weighted_edges_list, int** mst_edges_list);


void Prim(void* graph, int source, long** edges_weights_matrix, int* mst_parents);


#endif __MST_H__
