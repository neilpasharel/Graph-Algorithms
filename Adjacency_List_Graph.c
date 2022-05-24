
#include <stdio.h>
#include <stdlib.h>
#include "Adjacency_List_Graph.h"
#include "Linked_List.h"


//-----------------------------------------------------------------------

typedef struct {
	int key;
	void* data;
	void* neighbors_list; //implemented as LL. preserves order.
} AL_vertex;

//----------------------------------------------

AL_vertex* adjacency_list_graph_init_vertex(void* data) {
	AL_vertex* new_vertex = (AL_vertex*) malloc(sizeof(AL_vertex));
	if (new_vertex == NULL) return NULL;

	new_vertex->key = -1;
	new_vertex->data = data;
	new_vertex->neighbors_list = linked_list_init_list(1);

	return new_vertex;
}

//----------------------------------------------

void adjacency_list_graph_free_vertex(AL_vertex* vertex, void (*free_vertex_data)(void*)) {
	if (free_vertex_data) free_vertex_data(vertex->data);

	linked_list_free_list((void*) vertex->neighbors_list, NULL);

	free(vertex);
}

//----------------------------------------------

typedef struct {
	int vertices_cap;
	int is_directed;
	int vertices_count;
	int edges_count;
	AL_vertex** vertices;
} Adj_list_graph;

//-----------------------------------------------------------------------

void* adjacency_list_graph_init_graph(int is_directed) {
	Adj_list_graph* graph = (Adj_list_graph*) malloc(sizeof(Adj_list_graph));
	if (graph == NULL) return NULL;

	graph->vertices_cap = INIT_VERTICES_CAP;
	graph->is_directed = is_directed;
	graph->vertices_count = 0;
	graph->edges_count = 0;
	
	graph->vertices = (AL_vertex**) calloc(INIT_VERTICES_CAP, sizeof(AL_vertex*));
	if (graph->vertices == NULL) {
		free(graph);

		return NULL;
	}

	return (void*) graph;
}

//-----------------------------------------------------------------------

void* adjacency_list_graph_init_graph_from_edges_list(int vertices_count, int edges_count, int is_directed, int** edges_list) {
	Adj_list_graph* graph = adjacency_list_graph_init_graph(is_directed);
	if (graph == NULL) return NULL;

	int vertices_cap = ((INIT_VERTICES_CAP >= vertices_count) ? INIT_VERTICES_CAP : vertices_count);
	graph->vertices_cap = vertices_cap;

	graph->vertices = (AL_vertex**) calloc(vertices_cap, sizeof(AL_vertex*));
	if (graph->vertices == NULL) return NULL;  //malloc fail

	int status;

	for (int i = 0; i < vertices_count; i++) {
		status = adjacency_list_graph_add_vertex(graph, NULL);
		if (status < 0) return NULL; //malloc fail
	}

	for (int i = 0; i < edges_count; i++) {
		status = adjacency_list_graph_add_edge(graph, edges_list[i][0], edges_list[i][1], NULL);
		if (status != ADJ_LIST_GRAPH_OK) return NULL; //malloc fail from linked list
	}

	return (void*) graph;
}

//-----------------------------------------------------------------------

void adjacency_list_graph_free_graph(void* al_graph, void (*free_vertex_data)(void*)) {
	Adj_list_graph* graph = (Adj_list_graph*) al_graph;

	for (int i = 0; i < graph->vertices_count; i++) {
		adjacency_list_graph_free_vertex((graph->vertices)[i], free_vertex_data);
	}

	free(graph->vertices);

	free(graph);
}

//-----------------------------------------------------------------------

void* adjacency_list_graph_hard_copy_graph(void* src, size_t vertex_data_size, size_t edge_data_size) {
	Adj_list_graph* source = (Adj_list_graph*) src;

	int vertices_count = source->vertices_count;

	Adj_list_graph* res = adjacency_list_graph_init_graph(source->is_directed);
	if (res == NULL) return NULL; //malloc fail

	res->vertices = (AL_vertex**) malloc(source->vertices_cap * sizeof(AL_vertex*));
	if (res->vertices == NULL) return NULL;  //malloc fail

	void* data_copy = NULL;
	int status;

	for (int i = 0; i < vertices_count; i++) {
		data_copy = NULL;
		if (vertex_data_size > 0) {
			data_copy = malloc(vertex_data_size);
			if (data_copy == NULL) return NULL; //malloc fail

			memcpy(data_copy, source->vertices[i]->data, vertex_data_size);
		}

		status = adjacency_list_graph_add_vertex(res, NULL);
		if (status < 0) return NULL; //malloc fail

		linked_list_free_list(res->vertices[i]->neighbors_list, NULL);
		res->vertices[i]->neighbors_list = linked_list_hard_copy_list(source->vertices[i]->neighbors_list, edge_data_size);
	}

	res->vertices_cap = source->vertices_cap;
	res->vertices_count = source->vertices_count;
	res->edges_count = source->edges_count;

	return res;
}

//-----------------------------------------------------------------------
//TODO: FIX: bad function..
void adjacency_list_graph_get_edges_list(void* al_graph, int** edges_list) {
	Adj_list_graph* graph = (Adj_list_graph*) al_graph;

	Adj_list_graph* tmp = NULL;
	Adj_list_graph* graph_copy = NULL;

	int vertices_count = graph->vertices_count;
	int is_directed = graph->is_directed;

	if (is_directed == 0) {
		graph_copy = adjacency_list_graph_hard_copy_graph(graph, 0, 0);
		graph_copy->is_directed = 1;
		tmp = graph;
		graph = graph_copy;
	}

	int neighbor_key;
	int pos = 0;
	void* neighbors_iter;
	void* curr_edge_data;
	int status;

	for (int i = 0; i < vertices_count; i++) {
		neighbors_iter = adjacency_list_graph_init_neighbors_list_iterator(graph, i);
		if (neighbors_iter == NULL) return; //malloc fail

		while (adjacency_list_graph_neighbors_list_iterator_has_next(neighbors_iter) == 1) {
			neighbor_key = adjacency_list_graph_neighbors_list_iterator_next(neighbors_iter, &curr_edge_data);

			if (is_directed == 0) {
				graph->edges_count++;
				status = adjacency_list_graph_delete_edge(graph, neighbor_key, i, NULL);
				if (status != LL_OK) return; //malloc fail
			}

			edges_list[pos][0] = i;
			edges_list[pos][1] = neighbor_key;

			pos++;
		}

		adjacency_list_graph_free_neighbors_list_iterator(neighbors_iter);
	}

	if (is_directed == 0) {
		graph_copy = graph;
		graph = tmp;
		adjacency_list_graph_free_graph(graph_copy, NULL);
	}
}

//-----------------------------------------------------------------------

int adjacency_list_graph_is_directed(void* al_graph) {
	Adj_list_graph* graph = (Adj_list_graph*) al_graph;

	return graph->is_directed;
}

//-----------------------------------------------------------------------

int adjacency_list_graph_get_vertices_count(void* al_graph) {
	Adj_list_graph* graph = (Adj_list_graph*) al_graph;

	return graph->vertices_count;
}

//-----------------------------------------------------------------------

int adjacency_list_graph_get_edges_count(void* al_graph) {
	Adj_list_graph* graph = (Adj_list_graph*) al_graph;

	return graph->edges_count;
}

//-----------------------------------------------------------------------

int adjacency_list_graph_add_vertex(void* al_graph, void* data) {
	Adj_list_graph* graph = (Adj_list_graph*) al_graph;

	AL_vertex* new_vertex = adjacency_list_graph_init_vertex(data);
	if (new_vertex == NULL) return ADJ_LIST_GRAPH_OUT_OF_MEM;

	new_vertex->key = graph->vertices_count;
	graph->vertices_count++;

	int vertices_count = graph->vertices_count;

	if (vertices_count > graph->vertices_cap) {
		while (vertices_count > graph->vertices_cap) graph->vertices_cap += INIT_VERTICES_CAP;

		graph->vertices = (AL_vertex**) realloc(graph->vertices, graph->vertices_cap * sizeof(AL_vertex*));
		if (graph->vertices == NULL) return ADJ_LIST_GRAPH_OUT_OF_MEM;
	}

	graph->vertices[vertices_count -1] = new_vertex;

	return new_vertex->key;
}

//-----------------------------------------------------------------------

int adjacency_list_graph_add_edge(void* al_graph, int v1, int v2, void* edge_data) {
	Adj_list_graph* graph = (Adj_list_graph*) al_graph;

	int vertices_count = graph->vertices_count;
	if ((v1 >= vertices_count) || (v2 >= vertices_count)) return ADJ_LIST_GRAPH_MISSING;

	AL_vertex* vertex1 = graph->vertices[v1];
	AL_vertex* vertex2 = graph->vertices[v2];
	int status;

	status = linked_list_insert_or_update_item_by_key(
													  (void*) vertex1->neighbors_list, 
													   v2, 
													  ((edge_data != NULL) ? edge_data : (void*) graph->vertices[v2])
													 );
	if (status != LL_OK) return status;

	if (graph->is_directed == 0) {
		status = linked_list_insert_or_update_item_by_key(
														  (void*) vertex2->neighbors_list,
														   v1, 
														  ((edge_data != NULL) ? edge_data : (void*) graph->vertices[v1])
														 );
		if (status != LL_OK) return status;
	}

	graph->edges_count++;

	return ADJ_LIST_GRAPH_OK;
}

//-----------------------------------------------------------------------

int adjacency_list_graph_delete_edge(void* al_graph, int v1, int v2, void (*free_edge_data)(void*)) {
	Adj_list_graph* graph = (Adj_list_graph*) al_graph;

	int status = linked_list_delete_item_by_key((void*) graph->vertices[v1]->neighbors_list, v2, free_edge_data);
	if (status != LL_OK) return status;

	if (graph->is_directed == 0) {
		status = linked_list_delete_item_by_key((void*)graph->vertices[v2]->neighbors_list, v1, free_edge_data);
		if (status != LL_OK) return status;
	}

	graph->edges_count--;

	return ADJ_LIST_GRAPH_OK;
}

//-----------------------------------------------------------------------

int adjacency_list_graph_get_vertex_data_by_key(void* al_graph, int vertex_key, void** data) {
	Adj_list_graph* graph = (Adj_list_graph*) al_graph;

	if (vertex_key >= graph->vertices_count) return ADJ_LIST_GRAPH_MISSING;

	*data = graph->vertices[vertex_key]->data;

	return ADJ_LIST_GRAPH_OK;
}

//-----------------------------------------------------------------------

int adjacency_list_graph_is_neighbor(void* al_graph, int v1, int v2) {
	Adj_list_graph* graph = (Adj_list_graph*)al_graph;

	void* tmp;

	return linked_list_find_item_by_key((void*) graph->vertices[v1]->neighbors_list, graph->vertices[v2]->key, &tmp);
}

//-----------------------------------------------------------------------

int adjacency_list_graph_get_edge_data(void* al_graph, int v1, int v2, void** edge_data) {
	Adj_list_graph* graph = (Adj_list_graph*) al_graph;

	return linked_list_find_item_by_key((void*) graph->vertices[v1]->neighbors_list, graph->vertices[v2]->key, edge_data);
}

//-----------------------------------------------------------------------

void adjacency_list_graph_print_graph(void* al_graph) {
	Adj_list_graph* graph = (Adj_list_graph*) al_graph;

	int vertices_count = graph->vertices_count;

	for (int i = 0; i < vertices_count; i++) {
		printf("%d'th vertex: ", i);

		linked_list_print_list(graph->vertices[i]->neighbors_list, NULL);
	}

	putchar('\n');
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void* adjacency_list_graph_init_neighbors_list_iterator(void* al_graph, int vertex) {
	Adj_list_graph* graph = (Adj_list_graph*) al_graph;

	return linked_list_init_iterator((void*)graph->vertices[vertex]->neighbors_list);
}

int adjacency_list_graph_neighbors_list_iterator_has_next(void* alIter) {
	return linked_list_iterator_has_next(alIter);
}

int adjacency_list_graph_neighbors_list_iterator_next(void* alIter, void** item) {
	return linked_list_iterator_next(alIter, item);
}

void adjacency_list_graph_free_neighbors_list_iterator(void* alIter) {
	linked_list_free_iterator(alIter);
}

//------------------------------------------
























