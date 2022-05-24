
#include <stdio.h>
#include <stdlib.h>
#include "Abstract_Graph.h"
#include "Adjacency_List_Graph.h"
#include "Adjacency_Matrix_Graph.h"
#include "Edges_List.h"


typedef struct {
	void* implementation;
	Graph_rep_type rep_type;
} Graph;

//-----------------------------------------------------------------------------------

void* graph_init_graph(int is_directed, Graph_rep_type rep_type) {
	Graph* graph = (Graph*) malloc(sizeof(Graph));
	if (graph == NULL) return NULL;

	graph->rep_type = rep_type;

	int check = (graph->rep_type == Adjacency_List);
	void* (*f)(int) = ((check == 1) ? adjacency_list_graph_init_graph : NULL); //FIXXXXX

	if (f != NULL) graph->implementation = f(is_directed); //FIXXXXX
	if (graph->implementation == NULL) return NULL;

	return (void*) graph;
}

//-----------------------------------------------------------------------------------

void* graph_init_graph_from_edges_list(int vertices_count, int edges_count, int is_directed, int** edges_list, Graph_rep_type rep_type) {
	Graph* graph = (Graph*) malloc(sizeof(Graph));
	if (graph == NULL) return NULL;

	graph->rep_type = rep_type;

	int check = (graph->rep_type == Adjacency_List);
	void* (*f)(int, int, int, int**) = ((check == 1) ? adjacency_list_graph_init_graph_from_edges_list : 
													   adjacency_matrix_graph_init_graph_from_edges_list);

	graph->implementation = f(vertices_count, edges_count, is_directed, edges_list);
	if (graph->implementation == NULL) return NULL;

	return (void*) graph;
}

//-----------------------------------------------------------------------------------

void graph_free_graph(void* abs_graph, void (*free_vertex_data)(void*)) {
	Graph* graph = (Graph*) abs_graph;
	int check = (graph->rep_type == Adjacency_List);

	void (*f)(void*, void (*)(void*)) = (check == 1 ? adjacency_list_graph_free_graph : adjacency_matrix_graph_free_graph);

	f(graph->implementation, free_vertex_data);

	free(graph);
}

//-----------------------------------------------------------------------------------

void* graph_hard_copy_graph(void* src, size_t vertex_data_size, size_t edge_data_size) {
	Graph* source = (Graph*) src;

	Graph* res = graph_init_graph(graph_is_directed(source), source->rep_type);
	if (res == NULL) return NULL; //malloc fail

	int check = (source->rep_type == Adjacency_List);

	void* (*f)(void*, size_t, size_t) = (check == 1 ? adjacency_list_graph_hard_copy_graph : adjacency_matrix_graph_hard_copy_graph);

	res->implementation = f(source->implementation, vertex_data_size, edge_data_size);

	return (void*) res;
}

//-----------------------------------------------------------------------------------

Graph_rep_type graph_get_rep_type(void* abs_graph) {
	Graph* graph = (Graph*) abs_graph;

	return graph->rep_type;
}

//-----------------------------------------------------------------------------------

void* graph_convert_graph_rep(void* abs_graph) {
	Graph* graph = (Graph*) abs_graph;

	int vertices_count = graph_get_vertices_count(graph);
	int edges_count = graph_get_edges_count(graph);
	int is_directed = graph_is_directed(graph);

	Graph_rep_type dest_rep_type = ((graph_get_rep_type(graph) == Adjacency_List) ? Adjacency_Matrix : Adjacency_List);

	int** edges_list = edges_list_init_edges_list(edges_count);
	if (edges_list == NULL) return NULL;

	graph_get_edges_list(graph, edges_list);

	void* res = graph_init_graph_from_edges_list(vertices_count, edges_count, is_directed, edges_list, dest_rep_type);

	edges_list_free_edges_list(edges_list, edges_count);

	return res;
}

//-----------------------------------------------------------------------------------

int graph_is_directed(void* abs_graph) {
	Graph* graph = (Graph*) abs_graph;

	int check = (graph->rep_type == Adjacency_List);

	int (*f)(void*) = (check == 1 ? adjacency_list_graph_is_directed : adjacency_matrix_graph_is_directed);

	return f(graph->implementation);
}

//-----------------------------------------------------------------------------------

int graph_is_neighbor(void* abs_graph, int v1, int v2) {
	Graph* graph = (Graph*) abs_graph;

	int check = (graph->rep_type == Adjacency_List);

	int (*f)(void*, int, int) = (check == 1 ? adjacency_list_graph_is_neighbor : adjacency_matrix_graph_is_neighbor);

	return f(graph->implementation, v1, v2);
}

//-----------------------------------------------------------------------------------

void graph_print_graph(void* abs_graph) {
	Graph* graph = (Graph*) abs_graph;

	int check = (graph->rep_type == Adjacency_List);

	void (*f)(void*) = (check == 1 ? adjacency_list_graph_print_graph : adjacency_matrix_graph_print_graph);

	f(graph->implementation);
}

//-----------------------------------------------------------------------------------

int graph_get_vertices_count(void* abs_graph) {
	Graph* graph = (Graph*) abs_graph;

	int check = (graph->rep_type == Adjacency_List);

	int (*f)(void*) = (check == 1 ? adjacency_list_graph_get_vertices_count : adjacency_matrix_graph_get_vertices_count);

	return f(graph->implementation);
}

//-----------------------------------------------------------------------------------

int graph_get_edges_count(void* abs_graph) {
	Graph* graph = (Graph*) abs_graph;

	int check = (graph->rep_type == Adjacency_List);

	int (*f)(void*) = (check == 1 ? adjacency_list_graph_get_edges_count : adjacency_matrix_graph_get_edges_count);

	return f(graph->implementation);
}

//-----------------------------------------------------------------------------------

int graph_get_vertex_data_by_key(void* abs_graph, int vertex_key, void** data) {
	Graph* graph = (Graph*) abs_graph;

	int check = (graph->rep_type == Adjacency_List);

	int (*f)(void*, int, void**) = (check == 1 ? adjacency_list_graph_get_vertex_data_by_key : NULL); //FIXXXXX

	int res = GRAPH_MISSING; //FIXXXXX
	if (f != NULL) res = f(graph->implementation, vertex_key, data); //FIXXXXX

	return res;
}

//-----------------------------------------------------------------------------------

int graph_get_edge_data(void* abs_graph, int v1, int v2, void** edge_data) {
	Graph* graph = (Graph*) abs_graph;

	int check = (graph->rep_type == Adjacency_List);

	int (*f)(void*, int, int, void**) = (check == 1 ? adjacency_list_graph_get_edge_data : NULL); //FIXXXXX

	int res = GRAPH_MISSING; //FIXXXXX
	if (f != NULL) res = f(graph->implementation, v1, v2, edge_data); //FIXXXXX

	return res;
}


//-----------------------------------------------------------------------------------

void graph_get_edges_list(void* abs_graph, int** edges_list) {
	Graph* graph = (Graph*) abs_graph;

	int check = (graph->rep_type == Adjacency_List);

	void (*f)(void*, int**) = (check == 1 ? adjacency_list_graph_get_edges_list : adjacency_matrix_graph_get_edges_list);

	f(graph->implementation, edges_list);
}

//-----------------------------------------------------------------------------------

int graph_add_vertex(void* abs_graph, void* data) {
	Graph* graph = (Graph*) abs_graph;

	int check = (graph->rep_type == Adjacency_List);

	int (*f)(void*, void*) = (check == 1 ? adjacency_list_graph_add_vertex : adjacency_matrix_graph_add_vertex);

	return f(graph->implementation, data);
}

//-----------------------------------------------------------------------------------

int graph_add_edge(void* abs_graph, int v1, int v2, void* edge_data) {
	Graph* graph = (Graph*) abs_graph;
	
	int check = (graph->rep_type == Adjacency_List);

	int (*f)(void*, int, int, void*) = (check == 1 ? adjacency_list_graph_add_edge : adjacency_matrix_graph_add_edge);

	return f(graph->implementation, v1, v2, edge_data);
}

//-----------------------------------------------------------------------------------

int graph_delete_edge(void* abs_graph, int v1, int v2, void (*free_edge_data)(void*)) {
	Graph* graph = (Graph*) abs_graph;

	int check = (graph->rep_type == Adjacency_List);

	int (*f)(void*, int, int, void (*)(void*)) = (check == 1 ? adjacency_list_graph_delete_edge : adjacency_matrix_graph_delete_edge);

	return f(graph->implementation, v1, v2, free_edge_data);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

typedef struct {
	void* implementation;
	Graph_rep_type rep_type;
} Graph_neighbors_iterator;

//----------------------------------------

void* graph_init_neighbors_iterator(void* abs_graph, int vertex_key) {
	Graph* graph = (Graph*) abs_graph;

	int check = (graph->rep_type == Adjacency_List);

	Graph_neighbors_iterator* a_iter = (Graph_neighbors_iterator*) malloc(sizeof(Graph_neighbors_iterator));
	if (a_iter == NULL) return NULL; //malloc fail

	a_iter->rep_type = graph->rep_type;

	void* (*f)(void*, int) = ((check == 1) ? adjacency_list_graph_init_neighbors_list_iterator : 
												adjacency_matrix_graph_init_neighbors_iterator);

	a_iter->implementation = f(graph->implementation, vertex_key);
	if (a_iter->implementation == NULL) return NULL; //malloc fail

	return (void*) a_iter;
}

//----------------------------------------

int graph_neighbors_iterator_has_next(void* abs_iter) {
	Graph_neighbors_iterator* a_iter = (Graph_neighbors_iterator*) abs_iter;

	int check = (a_iter->rep_type == Adjacency_List);

	int (*f)(void*) = ((check == 1) ? adjacency_list_graph_neighbors_list_iterator_has_next : 
									  adjacency_matrix_graph_neighbors_iterator_has_next);

	return f(a_iter->implementation);
}

//----------------------------------------

int graph_neighbors_iterator_next(void* abs_iter, void** data) {
	Graph_neighbors_iterator* a_iter = (Graph_neighbors_iterator*) abs_iter;

	int check = (a_iter->rep_type == Adjacency_List);

	int (*f)(void*, void**) = ((check == 1) ? adjacency_list_graph_neighbors_list_iterator_next : 
											  adjacency_matrix_graph_neighbors_iterator_next);

	return f(a_iter->implementation, data);
}

//----------------------------------------

void graph_free_neighbors_iterator(void* abs_iter) {
	Graph_neighbors_iterator* a_iter = (Graph_neighbors_iterator*) abs_iter;

	int check = (a_iter->rep_type == Adjacency_List);

	void (*f)(void*) = ((check == 1) ? adjacency_list_graph_free_neighbors_list_iterator : 
									   adjacency_matrix_graph_free_neighbors_iterator);

	f(a_iter->implementation);
}

//----------------------------------------