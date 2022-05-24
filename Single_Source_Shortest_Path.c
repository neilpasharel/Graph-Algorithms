
#include <stdio.h>
#include <stdlib.h>
#include "Single_Source_Shortest_Path.h"
#include "Priority_Queue.h"
#include "Graph_Traversal.h"
#include "Longs_Matrix.h"


//----------------------------------------------------------------

void relax(int v1, int v2, long** edges_weights_matrix, long* path_weights_from_source, int* sp_parents) {
	long tmp = path_weights_from_source[v1] + edges_weights_matrix[v1][v2];

	if (path_weights_from_source[v2] > tmp) {
		path_weights_from_source[v2] = tmp;
		
		sp_parents[v2] = v1;
	}
}

//----------------------------------------------------------------

void init_SSSP(void* graph, int source, long* path_weights_from_source, int* sp_parents) {
	int vertices_count = graph_get_vertices_count(graph);

	path_weights_from_source[source] = 0;

	for (int i = 0; i < vertices_count; i++) {
		sp_parents[i] = INT_MAX;

		if (i == source) continue;

		path_weights_from_source[i] = LONG_MAX / 2;  //divided by 2 to prevent overflows on first relaxations
	}
}

////----------------------------------------------------------------

int Bellman_Ford(void* graph, int source, long** edges_weights_matrix, long* path_weights_from_source, int* sp_parents) {
	int vertices_count = graph_get_vertices_count(graph);

	init_SSSP(graph, source, path_weights_from_source, sp_parents);

	int curr_neighbor;
	void* v_iter;

	for (int k = 0; k < vertices_count - 1; k++) {
		for (int i = 0; i < vertices_count; i++) {
			v_iter = graph_init_neighbors_iterator(graph, i);
			if (v_iter == NULL) return -1; //malloc fail

			while (graph_neighbors_iterator_has_next(v_iter) == 1) {
				curr_neighbor = graph_neighbors_iterator_next(v_iter, NULL);

				relax(i, curr_neighbor, edges_weights_matrix, path_weights_from_source, sp_parents);
			}
			graph_free_neighbors_iterator(v_iter);
		}
	}

	for (int i = 0; i < vertices_count; i++) {
		v_iter = graph_init_neighbors_iterator(graph, i);
		if (v_iter == NULL) return -1; //malloc fail

		while (graph_neighbors_iterator_has_next(v_iter) == 1) {
			curr_neighbor = graph_neighbors_iterator_next(v_iter, NULL);

			if (path_weights_from_source[curr_neighbor] > path_weights_from_source[i] + edges_weights_matrix[i][curr_neighbor]) 
				return 1; //detected neg weight cycle
		}
		graph_free_neighbors_iterator(v_iter);
	}

	return 0;
}

//----------------------------------------------------------------

void DAG_SSSP(void* graph, int source, long** edges_weights_matrix, long* path_weights_from_source, int* sp_parents) {
	int vertices_count = graph_get_vertices_count(graph);

	init_SSSP(graph, source, path_weights_from_source, sp_parents);

	int* topo_sort = topological_sort(graph);

	int curr_vertex, curr_neighbor;
	void* v_iter;

	for (int i = 0; i < vertices_count; i++) {
		curr_vertex = topo_sort[i];

		v_iter = graph_init_neighbors_iterator(graph, curr_vertex);
		if (v_iter == NULL) return; //malloc fail

		while (graph_neighbors_iterator_has_next(v_iter) == 1) {
			curr_neighbor = graph_neighbors_iterator_next(v_iter, NULL);

			relax(curr_vertex, curr_neighbor, edges_weights_matrix, path_weights_from_source, sp_parents);
		}
		graph_free_neighbors_iterator(v_iter);
	}

	free(topo_sort);
}

//----------------------------------------------------------------

int init_Dijkstra(void* graph, int source, int** vertices_arr, void** p_queue) {
	int vertices_count = graph_get_vertices_count(graph);
	int status;

	*vertices_arr = (int*) malloc(vertices_count * sizeof(int));
	if (*vertices_arr == NULL) return -1; //malloc fail
	for (int i = 0; i < vertices_count; i++) {
		(*vertices_arr)[i] = i;
	}

	*p_queue = priority_queue_init_pq(MIN, vertices_count);
	if (*p_queue == NULL) return -1; //malloc fail

	int curr_key;

	for (int i = 0; i < vertices_count; i++) {
		curr_key = ((i == source) ? 0 : INT_MAX /2);

		status = priority_queue_insert(*p_queue, &(*vertices_arr)[i], curr_key, NULL); //FIX: keys are ints in pq, but weights are longs..
		if (status != PQ_OK) return -1;
	}

	return 0;
}

//----------------------------------------------------------------

void Dijkstra(void* graph, int source, long** edges_weights_matrix, long* path_weights_from_source, int* sp_parents) {
	void* p_queue;
	int* vertices_arr = NULL;

	int check = init_Dijkstra(graph, source, &vertices_arr, &p_queue);
	if (check != 0) return; //malloc fail

	init_SSSP(graph, source, path_weights_from_source, sp_parents);

	int curr_vertex, curr_neighbor;
	long curr_key, curr_neighbor_key, curr_edge_weight;
	void* v_iter;

	while (priority_queue_get_size(p_queue) > 0) {
		priority_queue_print_pq(p_queue);

		curr_vertex = *priority_queue_poll_first(p_queue);
		check = priority_queue_get_key_by_hm_key(p_queue, &vertices_arr[curr_vertex], &curr_key);
		curr_vertex = *priority_queue_extract_first(p_queue, NULL);

		path_weights_from_source[curr_vertex] = curr_key; //extracted from pq -> finalize path weight from source

		v_iter = graph_init_neighbors_iterator(graph, curr_vertex);
		if (v_iter == NULL) return; //malloc fail

		while (graph_neighbors_iterator_has_next(v_iter) == 1) {
			curr_neighbor = graph_neighbors_iterator_next(v_iter, NULL);

			check = priority_queue_get_key_by_hm_key(p_queue, &vertices_arr[curr_neighbor], &curr_neighbor_key);
			if (check == PQ_MISSING) continue;

			curr_edge_weight = curr_key + edges_weights_matrix[curr_vertex][curr_neighbor];
			if (curr_neighbor_key > curr_edge_weight) {
				priority_queue_decrease_key(p_queue, &vertices_arr[curr_neighbor], curr_edge_weight);

				sp_parents[curr_neighbor] = curr_vertex;

				priority_queue_print_pq(p_queue);
			}
		}
		graph_free_neighbors_iterator(v_iter);
	}

	priority_queue_free_pq(p_queue, NULL);
	free(vertices_arr);
}