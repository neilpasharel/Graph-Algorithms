
#include <stdio.h>
#include <stdlib.h>
#include "Graph_Transpose.h"
#include "Edges_List.h"

//-------------------------------------

void* transpose_graph(void* graph, size_t vertex_data_size, size_t edge_data_size) {
	int vertices_count = graph_get_vertices_count(graph);
	int edges_count = graph_get_edges_count(graph);
	int is_directed = graph_is_directed(graph);
	Graph_rep_type rep_type = graph_get_rep_type(graph);

	if (is_directed == 0) return graph_hard_copy_graph(graph, vertex_data_size, edge_data_size);

	int** edges_list = edges_list_init_edges_list(edges_count);

	graph_get_edges_list(graph, edges_list);

	int** reversed_edges_list = edges_list_init_edges_list(edges_count);

	for (int i = 0; i < edges_count; i++) {
		reversed_edges_list[i][0] = edges_list[i][1];
		reversed_edges_list[i][1] = edges_list[i][0];
	}

	void* res = graph_init_graph_from_edges_list(vertices_count, edges_count, is_directed, reversed_edges_list, rep_type);

	edges_list_free_edges_list(edges_list, edges_count);
	edges_list_free_edges_list(reversed_edges_list, edges_count);

	return res;
}