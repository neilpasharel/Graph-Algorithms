
#ifndef __GRAPHTRAVERSAL_H__
#define __GRAPHTRAVERSAL_H__

#include "Abstract_Graph.h"


void BFS(void* graph, int* source, int* distances, int* bfs_parents);

void DFS(void* graph, int* discovery_times, int* dfs_parents, int* finish_times);


// graph traversal apps

int* topological_sort(void* graph);

int** compute_strongly_connected_components_graph(void* graph, int** SCCs_counts, int* SCCs_count);

#endif __GRAPHTRAVERSAL_H__
