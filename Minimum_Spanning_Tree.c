
#include <stdio.h>
#include <stdlib.h>
#include "Minimum_Spanning_Tree.h"
#include "Union_Find.h"
#include "Priority_Queue.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int kruskal_compare_weighted_edges(const void* e1, const void* e2) {
	long* ee1 = *((long**) e1);
	long* ee2 = *((long**) e2);

	return (int) (ee1[2] - ee2[2]);
}

//----------------------------

int init_Kruskal(void* graph, long** weighted_edges_list, void** m_set) {
	int vertices_count = graph_get_vertices_count(graph);
	int edges_count = graph_get_edges_count(graph);
	int status;

	qsort((void*) weighted_edges_list, edges_count, sizeof(3 * sizeof(long)), kruskal_compare_weighted_edges);

	printf("\nthe edges of the graph with weights after sort:\n");
	for (int i = 0; i < edges_count; i++) 
		printf("%d %d %d\n", weighted_edges_list[i][0], weighted_edges_list[i][1], weighted_edges_list[i][2]);

	*m_set = uf_init_multi_set();
	if (*m_set == NULL) return -1; //malloc fail

	for (int i = 0; i < vertices_count; i++) { //adding vertices to multi set
		status = uf_make_set(*m_set, NULL);
		if (status < 0) return -1; //malloc fail
	}

	return 0;
}

//--------------------------------

void Kruskal(void* graph, long** weighted_edges_list, int** mst_edges_list) {
	int edges_count = graph_get_edges_count(graph);
	long tot_weight = 0;
	int pos = 0, v1, v2;
	void* m_set = NULL;

	init_Kruskal(graph, weighted_edges_list, &m_set);

	for (int i = 0; i < edges_count; i++) {
		v1 = weighted_edges_list[i][0];
		v2 = weighted_edges_list[i][1];

		if (uf_find(m_set, v1) != uf_find(m_set, v2)) {
			mst_edges_list[pos][0] = v1;
			mst_edges_list[pos][1] = v2;

			uf_link(m_set, v1, v2);
			pos++;

			tot_weight += weighted_edges_list[i][2];
		}
	}

	printf("\nthe weight of the MST: %ld\n", tot_weight);

	uf_free_multi_set(m_set, NULL);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int init_Prim(void* graph, int** vertices_arr, int source, void** p_queue) {
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
		curr_key = ((i == source) ? INT_MIN : INT_MAX);

		status = priority_queue_insert(*p_queue, &(*vertices_arr)[i], curr_key, NULL);
		if (status != PQ_OK) return -1;
	}

	return 0;
}

//---------------------------

void Prim(void* graph, int source, long** edges_weights_matrix, int* mst_parents) {
	void* p_queue;
	int* vertices_arr = NULL;

	int check = init_Prim(graph, &vertices_arr, source, &p_queue);
	if (check != 0) return; //malloc fail

	int curr_vertex, curr_neighbor;
	long curr_edge_weight, curr_neighbor_key;
	void* v_iter;

	//priority_queue_print_pq(p_queue);
	while (priority_queue_get_size(p_queue) > 0) {
		curr_vertex = *priority_queue_extract_first(p_queue, NULL);
		//priority_queue_print_pq(p_queue);

		v_iter = graph_init_neighbors_iterator(graph, curr_vertex);
		if (v_iter == NULL) return; //malloc fail

		while (graph_neighbors_iterator_has_next(v_iter) == 1) {
			curr_neighbor = graph_neighbors_iterator_next(v_iter, NULL);
			curr_edge_weight = edges_weights_matrix[curr_vertex][curr_neighbor];

			check = priority_queue_get_key_by_hm_key(p_queue, &vertices_arr[curr_neighbor], &curr_neighbor_key);
			if ((check != PQ_MISSING) && (curr_edge_weight < curr_neighbor_key)) {
				mst_parents[curr_neighbor] = curr_vertex;

				priority_queue_decrease_key(p_queue, &vertices_arr[curr_neighbor], curr_edge_weight);
				//priority_queue_print_pq(p_queue);
			}
		}
		graph_free_neighbors_iterator(v_iter);
	}

	priority_queue_free_pq(p_queue, NULL);
	free(vertices_arr);
}

