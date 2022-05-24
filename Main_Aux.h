
#ifndef __MAINAUX_H__
#define __MAINAUX_H__

#include "Abstract_Graph.h"

#define MAIN_OK				 0 	// exit success
#define MAIN_OUT_OF_MEM		-1 	// malloc fail


void main_aux_print_int_pointer(void* i);

void main_aux_print_array(void* arr, int arr_count, size_t elem_size, void (*print_elem)(void*));

int** main_aux_parents_to_edges_list_conversion(int* parents, int parents_count, int* edges_count);


//=============== BFS TEST AUX ================

int main_aux_init_BFS_test(void* graph, int* source, int** shortest_path_distances, int** parents);


//=============== DFS TEST AUX ================

int main_aux_init_DFS_test(void* graph, int** discovery_times, int** finish_times, int** parents);


//============ KRUSKAL TEST AUX ===============

int main_aux_init_test_Kruskal(void* graph, long** edges_weights_matrix, long*** weighted_edges_list, int*** mst_edges_list);


//============== PRIM TEST AUX ===============

int main_aux_init_test_Prim(void* graph, int* source, int** mst_parents);

void main_aux_print_test_Prim(int vertices_count, long** edges_weights_matrix, int* mst_parents);


//============== SSSP TEST AUX ===============

int main_aux_init_test_SSSP(void* graph, int* source, long** path_weights_from_source, int** sp_parents);

int main_aux_print_test_SSSP(void* graph, int source, long* path_weights_from_source, int* sp_parents);


//============== APSP TEST AUX ===============

int main_aux_init_test_APSP(void* graph, long*** path_weights_from_src_to_trgt, int*** apsp_parents);

void main_aux_print_test_APSP(void* graph, long** path_weights_from_src_to_trgt, int** apsp_parents);


#endif __MAINAUX_H__
