
#include <stdio.h>
#include <stdlib.h>
#include "Adjacency_Matrix_Graph.h"

//---------------------------------------------------------------------

typedef struct {
	int vertices_count;
	int edges_count;
	int** adj_matrix;
	int is_directed;
} Adj_mat_graph;

//---------------------------------------------------------------------

void* adjacency_matrix_graph_init_graph_from_edges_list(int vertices_count, int edges_count, int is_directed, int** edges_list) {
	Adj_mat_graph* graph = (Adj_mat_graph*)malloc(sizeof(Adj_mat_graph));
	if (graph == NULL) return NULL;

	graph->vertices_count = vertices_count;
	graph->edges_count = edges_count;
	graph->is_directed = is_directed;

	graph->adj_matrix = (int**) malloc(vertices_count * sizeof(int*));
	if (graph->adj_matrix == NULL) {
		free(graph);

		return NULL;
	}

	for (int i = 0; i < vertices_count; i++) {
		(graph->adj_matrix)[i] = (int*) calloc(vertices_count, sizeof(int));
		if (graph->adj_matrix[i] == NULL) {
			for (int j = 0; j < i; j++) free(graph->adj_matrix[j]);

			free(graph->adj_matrix);
			free(graph);

			return NULL;
		}
	}

	for (int i = 0; i < edges_count; i++) {
		int v1 = edges_list[i][0];
		size_t v2 = edges_list[i][1];

		graph->adj_matrix[v1][v2] = 1;
		if (is_directed == 0) graph->adj_matrix[v2][v1] = 1;
	}

	return (void*) graph;
}

//---------------------------------------------------------------------

void adjacency_matrix_graph_free_graph(void* am_graph, void (*ignore)(void*)) {
	Adj_mat_graph* graph = (Adj_mat_graph*) am_graph;

	for (int i = 0; i < graph->vertices_count; i++) free(graph->adj_matrix[i]);

	free(graph->adj_matrix);
	free(graph);
}

//---------------------------------------------------------------------

void* adjacency_matrix_graph_hard_copy_graph(void* src, size_t ignore1, size_t ignore2) {
	Adj_mat_graph* source = (Adj_mat_graph*) src;

	Adj_mat_graph* res = (Adj_mat_graph*) malloc(sizeof(Adj_mat_graph));
	if (res == NULL) return NULL;

	int vertices_count = source->vertices_count;

	res->vertices_count = vertices_count;
	res->edges_count = source->edges_count;
	res->is_directed = source->is_directed;

	res->adj_matrix = (int**) malloc(vertices_count * sizeof(int*));
	if (res->adj_matrix == NULL) return NULL; //malloc fail

	for (int i = 0; i < vertices_count; i++) {
		(res->adj_matrix)[i] = (int*) malloc(vertices_count * sizeof(int));
		if (res->adj_matrix[i] == NULL) return NULL;

		for (int j = 0; j < vertices_count; j++) res->adj_matrix[i][j] = source->adj_matrix[i][j];
	}

	return (void*) res;
}

//---------------------------------------------------------------------

int adjacency_matrix_graph_get_vertices_count(void* am_graph) {
	Adj_mat_graph* graph = (Adj_mat_graph*) am_graph;

	return graph->vertices_count;
}

//---------------------------------------------------------------------

int adjacency_matrix_graph_get_edges_count(void* am_graph) {
	Adj_mat_graph* graph = (Adj_mat_graph*) am_graph;

	return graph->edges_count;
}
//---------------------------------------------------------------------

void adjacency_matrix_graph_get_edges_list(void* am_graph, int** edges_list) {
	Adj_mat_graph* graph = (Adj_mat_graph*) am_graph;

	int vertices_count = graph->vertices_count;
	int pos = 0;

	for (int i = 0; i < vertices_count; i++) {
		for (int j = 0; j < vertices_count; j++) {
			if (graph->adj_matrix[i][j] == 2) graph->adj_matrix[i][j] = 1;
			else if (graph->adj_matrix[i][j] == 1) {
				edges_list[pos][0] = i;
				edges_list[pos][1] = j;

				pos++;

				if (graph->is_directed == 0) graph->adj_matrix[j][i] = 2;
			}
		}
	}
}

//---------------------------------------------------------------------

int adjacency_matrix_graph_add_vertex(void* am_graph, void* ignore) {
	Adj_mat_graph* graph = (Adj_mat_graph*) am_graph;

	int new_vertex_key = graph->vertices_count;
	graph->vertices_count++;

	int vertices_count = graph->vertices_count;

	graph->adj_matrix = (int**) realloc(graph->adj_matrix, vertices_count * sizeof(int*));
	if (graph->adj_matrix == NULL) return ADJ_MAT_GRAPH_OUT_OF_MEM;

	graph->adj_matrix[new_vertex_key] = (int*) malloc(sizeof(int));
	if (graph->adj_matrix[new_vertex_key] == NULL) return ADJ_MAT_GRAPH_OUT_OF_MEM;

	for (int i = 0; i < vertices_count; i++) {
		graph->adj_matrix[i] = (int*) realloc(graph->adj_matrix[i], vertices_count * sizeof(int));
		if (graph->adj_matrix[i] == NULL) return ADJ_MAT_GRAPH_OUT_OF_MEM;

		graph->adj_matrix[i][new_vertex_key] = 0;
	}

	return new_vertex_key;
}

//---------------------------------------------------------------------

int adjacency_matrix_graph_add_edge(void* am_graph, int v1, int v2, void* ignore) {
	Adj_mat_graph* graph = (Adj_mat_graph*) am_graph;

	if (graph->adj_matrix[v1][v2] == 1) return ADJ_MAT_GRAPH_OK;

	graph->edges_count++;
	graph->adj_matrix[v1][v2] = 1;
	if (graph->is_directed == 0) graph->adj_matrix[v2][v1] = 1;

	return ADJ_MAT_GRAPH_OK;
}

//---------------------------------------------------------------------

int adjacency_matrix_graph_delete_edge(void* am_graph, int v1, int v2, void (*ignore)(void*)) {
	Adj_mat_graph* graph = (Adj_mat_graph*)am_graph;

	if (graph->adj_matrix[v1][v2] == 0) return ADJ_MAT_GRAPH_OK;

	graph->edges_count--;
	graph->adj_matrix[v1][v2] = 0;
	if (graph->is_directed == 0) graph->adj_matrix[v2][v1] = 0;

	return ADJ_MAT_GRAPH_OK;
}

//---------------------------------------------------------------------

int adjacency_matrix_graph_is_directed(void* am_graph) {
	Adj_mat_graph* graph = (Adj_mat_graph*) am_graph;

	return graph->is_directed;
}

//---------------------------------------------------------------------

int adjacency_matrix_graph_is_neighbor(void* am_graph, int v1, int v2) {
	Adj_mat_graph* graph = (Adj_mat_graph*) am_graph;

	return ((graph->adj_matrix[v1][v2] == 0) ? 0 : 1);
}

//---------------------------------------------------------------------

void adjacency_matrix_graph_print_graph(void* am_graph) {
	Adj_mat_graph* graph = (Adj_mat_graph*) am_graph;

	int vertices_count = graph->vertices_count;

	for (int i = 0; i < vertices_count; i++) {
		for (int j = 0; j < vertices_count; j++) {
			printf("%3d ", graph->adj_matrix[i][j]);
		}

		putchar('\n');
	}

	putchar('\n');
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

size_t adjacency_matrix_graph_get_next_neighbor(Adj_mat_graph* graph, int vertex_key, int curr_neighbor_key) {
	int vertices_count = graph->vertices_count;
	int next_neighbor_key = curr_neighbor_key;

	do next_neighbor_key++;
	while ((next_neighbor_key < vertices_count) && (graph->adj_matrix[vertex_key][next_neighbor_key] == 0));

	return next_neighbor_key;
}

//-----------------------------

typedef struct {
	Adj_mat_graph* graph;
	int this_vertex_key;
	int curr_neighbor_key;
} Adjacency_matrix_graph_neighbors_iterator;

//-----------------------------

void* adjacency_matrix_graph_init_neighbors_iterator(void* am_graph, int vertex_key) {
	Adj_mat_graph* graph = (Adj_mat_graph*) am_graph;

	Adjacency_matrix_graph_neighbors_iterator* iter = (Adjacency_matrix_graph_neighbors_iterator*) 
													   malloc(sizeof(Adjacency_matrix_graph_neighbors_iterator));
	if (iter == NULL) return NULL;

	iter->graph = graph;
	iter->this_vertex_key = vertex_key;
	iter->curr_neighbor_key = -1;

	return (void*) iter;
}

//-----------------------------


int adjacency_matrix_graph_neighbors_iterator_has_next(void* am_iter) {
	Adjacency_matrix_graph_neighbors_iterator* iter = (Adjacency_matrix_graph_neighbors_iterator*) am_iter;

	int vertices_count = iter->graph->vertices_count;

	int next_neighbor_key = adjacency_matrix_graph_get_next_neighbor(iter->graph, iter->this_vertex_key, iter->curr_neighbor_key);

	return ((next_neighbor_key < vertices_count) ? 1 : 0);
}

//-----------------------------

size_t adjacency_matrix_graph_neighbors_iterator_next(void* am_iter, void** ignore) {
	Adjacency_matrix_graph_neighbors_iterator* iter = (Adjacency_matrix_graph_neighbors_iterator*) am_iter;

	int vertices_count = iter->graph->vertices_count;

	int next_neighbor_key = adjacency_matrix_graph_get_next_neighbor(iter->graph, iter->this_vertex_key, iter->curr_neighbor_key);

	iter->curr_neighbor_key = next_neighbor_key;

	return next_neighbor_key;
}

//-----------------------------

void adjacency_matrix_graph_free_neighbors_iterator(void* am_iter) {
	Adjacency_matrix_graph_neighbors_iterator* iter = (Adjacency_matrix_graph_neighbors_iterator*) am_iter;

	free(iter);
}

