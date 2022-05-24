
#ifndef __ABSTRACT_GRAPH_H__
#define __ABSTRACT_GRAPH_H__

#define GRAPH_OK			 0 	// exit success
#define GRAPH_OUT_OF_MEM	-1 	// malloc fail
#define GRAPH_MISSING		-2	// graph does not contain this item

typedef enum Graph_representation_type { Adjacency_List , Adjacency_Matrix } Graph_rep_type;


//=============== INIT / FREE ================

void* graph_init_graph(int is_directed, Graph_rep_type rep_type); //FIX: works only for adjList

void* graph_init_graph_from_edges_list(int vertices_count, int edges_count, int is_directed, int** edges_list, Graph_rep_type rep_type);

void* graph_hard_copy_graph(void* src, size_t vertex_data_size, size_t edge_data_size);

void graph_free_graph(void* abs_graph, void (*free_vertex_data)(void*));

//=============== ABSTRACT RELATED ================

Graph_rep_type graph_get_rep_type(void* abs_graph);

void* graph_convert_graph_rep(void* abs_graph); //converts inplace [maybe change that?]


//=============== QUERIES ================

int graph_is_directed(void* abs_graph);

int graph_is_neighbor(void* abs_graph, int v1, int v2);

void graph_print_graph(void* abs_graph);


//=============== GETTERS ================

int graph_get_vertices_count(void* abs_graph);

int graph_get_edges_count(void* abs_graph);

int graph_get_vertex_data_by_key(void* abs_graph, int vertex_key, void** data); //FIX: works only for adjList

int graph_get_edge_data(void* abs_graph, int v1, int v2, void** edge_data); //FIX: works only for adjList

void graph_get_edges_list(void* abs_graph, int** edges_list);


//=============== SETTERS ================

int graph_add_vertex(void* abs_graph, void* data); //returns key which we treat as 'new name'

int graph_add_edge(void* abs_graph, int v1, int v2, void* edge_data);

int graph_delete_edge(void* abs_graph, int v1, int v2, void (*free_edge_data)(void*));


//=============== ITERATOR ================

void* graph_init_neighbors_iterator(void* abs_graph, int vertex_key);

int graph_neighbors_iterator_has_next(void* abs_iter);

int graph_neighbors_iterator_next(void* abs_iter, void** data); //returns key [and also item if given pointer..]

void graph_free_neighbors_iterator(void* abs_iter);


#endif __ABSTRACT_GRAPH_H__
