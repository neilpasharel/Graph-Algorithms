
#ifndef __GRAPH_ADJMAT_H_
#define __GRAPH_ADJMAT_H_

#define ADJ_MAT_GRAPH_OK			 0 	// exit success
#define ADJ_MAT_GRAPH_OUT_OF_MEM	-1 	// malloc fail
#define ADJ_MAT_GRAPH_MISSING		-2	// graph does not contain this item


//TODO: add AM_Vertex struct (make adj matrix of instances of this struct..)

//=============== INIT / FREE ================

void* adjacency_matrix_graph_init_graph_from_edges_list(int vertices_count, int edges_count, int is_directed, int** edges_list);

void* adjacency_matrix_graph_hard_copy_graph(void* src, size_t ignore1, size_t ignore2);

void adjacency_matrix_graph_free_graph(void* am_graph, void (*ignore)(void*));

//=============== QUERIES ================

int adjacency_matrix_graph_is_directed(void* am_graph);

int adjacency_matrix_graph_is_neighbor(void* am_graph, int v1, int v2);

void adjacency_matrix_graph_print_graph(void* am_graph);


//=============== GETTERS ================

int adjacency_matrix_graph_get_vertices_count(void* am_graph);

int adjacency_matrix_graph_get_edges_count(void* am_graph);

void adjacency_matrix_graph_get_edges_list(void* am_graph, int** edges_list);


//=============== SETTERS ================

int adjacency_matrix_graph_add_vertex(void* am_graph, void* ignore);

int adjacency_matrix_graph_add_edge(void* am_graph, int v1, int v2, void* ignore);

int adjacency_matrix_graph_delete_edge(void* am_graph, int v1, int v2, void (*ignore)(void*));


//=============== ITERATOR ================

void* adjacency_matrix_graph_init_neighbors_iterator(void* am_graph, int vertex_key);

int adjacency_matrix_graph_neighbors_iterator_has_next(void* am_iter);

int adjacency_matrix_graph_neighbors_iterator_next(void* am_iter, void** ignore);

void adjacency_matrix_graph_free_neighbors_iterator(void* am_iter);


#endif __GRAPH_ADJMAT_H_
