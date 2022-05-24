
#ifndef __SSSP_H__
#define __SSSP_H__

#include "Abstract_Graph.h"


int Bellman_Ford(void* graph, int source, long** edges_weights_matrix, long* path_weights_from_source, int* sp_parents);

void DAG_SSSP(void* graph, int source, long** edges_weights_matrix, long* path_weights_from_source, int* sp_parents);

void Dijkstra(void* graph, int source, long** edges_weights_matrix, long* path_weights_from_source, int* sp_parents);


#endif __SSSP_H__
