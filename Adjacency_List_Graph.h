
#ifndef __GRAPH_ADJLIST_H_
#define __GRAPH_ADJLIST_H_

#define INIT_VERTICES_CAP 40

#define ADJ_LIST_GRAPH_OK			 0 	// exit success
#define ADJ_LIST_GRAPH_OUT_OF_MEM	-1 	// malloc fail
#define ADJ_LIST_GRAPH_MISSING		-2	// graph does not contain this item


//=============== INIT / FREE ================

void* adjacency_list_graph_init_graph(int is_directed); //empty initiation

void* adjacency_list_graph_init_graph_from_edges_list(int vertices_count, int edges_count, int is_directed, int** edges_list);

void* adjacency_list_graph_hard_copy_graph(void* src, size_t vertex_data_size, size_t edge_data_size);

void adjacency_list_graph_free_graph(void* al_graph, void (*free_vertex_data)(void*));


//=============== QUERIES ================

int adjacency_list_graph_is_directed(void* al_graph);

int adjacency_list_graph_is_neighbor(void* al_graph, int v1, int v2);

void adjacency_list_graph_print_graph(void* al_graph);


//=============== GETTERS ================

int adjacency_list_graph_get_vertices_count(void* al_graph);

int adjacency_list_graph_get_edges_count(void* al_graph);

int adjacency_list_graph_get_vertex_data_by_key(void* al_graph, int vertex_key, void** data);

int adjacency_list_graph_get_edge_data(void* al_graph, int v1, int v2, void** edge_data);

void adjacency_list_graph_get_edges_list(void* al_graph, int** edges_list); //FIX


//=============== SETTERS ================

int adjacency_list_graph_add_vertex(void* al_graph, void* data); //returns key which is 'new name'

int adjacency_list_graph_add_edge(void* al_graph, int v1, int v2, void* edge_data);

int adjacency_list_graph_delete_edge(void* al_graph, int v1, int v2, void (*free_edge_data)(void*));


//=============== ITERATOR ================

void* adjacency_list_graph_init_neighbors_list_iterator(void* al_graph, int vertex);

int adjacency_list_graph_neighbors_list_iterator_has_next(void* alIter);

int adjacency_list_graph_neighbors_list_iterator_next(void* alIter, void** item); //returns key [and also item if given pointer..]

void adjacency_list_graph_free_neighbors_list_iterator(void* alIter);



#endif __GRAPH_ADJLIST_H_
