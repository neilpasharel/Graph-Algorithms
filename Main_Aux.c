
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "Main_Aux.h"
#include "Longs_Matrix.h"
#include "Edges_List.h"


//------------------------------

void main_aux_print_int_pointer(void* i) {
    printf("%d", *((int*) i));
}

//------------------------------


void main_aux_print_array(void* arr, int arr_count, size_t elem_size, void (*print_elem)(void*)) {
    for (int i = 0; i < arr_count; i++) {
        void* curr_elem = (void*)(((char*)arr) + (i * elem_size));

        print_elem(curr_elem);

        putchar(' ');
    }

    putchar('\n');
}

//------------------------------

int** main_aux_parents_to_edges_list_conversion(int* parents, int parents_count, int* edges_count) {
    int** edges_list = edges_list_init_edges_list(parents_count);
    if (edges_list == NULL) return NULL; //malloc fail

    int pos = 0;

    for (int i = 0; i < parents_count; i++) {
        if (parents[i] == INT_MAX) continue;

        edges_list[pos][0] = parents[i];
        edges_list[pos][1] = i;

        pos++;
    }

    *edges_count = pos;

    return edges_list;
}


//=============== BFS TEST AUX ================

int main_aux_init_BFS_test(void* graph, int* source, int** shortest_path_distances, int** parents) {
    int vertices_count = graph_get_vertices_count(graph);

    printf("Which vertex is the source? ");
    *source = -1;
    while ((*source < 0) || (*source >= vertices_count)) scanf_s("%d%*c", source);

    *shortest_path_distances = (int*) malloc(vertices_count * sizeof(int));
    if (*shortest_path_distances == NULL) return MAIN_OUT_OF_MEM;
    for (int i = 0; i < vertices_count; i++) (*shortest_path_distances)[i] = INT_MAX;

    *parents = (int*) malloc(vertices_count * sizeof(int));
    if (*parents == NULL) return MAIN_OUT_OF_MEM;
    for (int i = 0; i < vertices_count; i++) (*parents)[i] = INT_MAX;

    return MAIN_OK;
}


//=============== DFS TEST AUX ================

int main_aux_init_DFS_test(void* graph, int** discovery_times, int** finish_times, int** parents) {
    int vertices_count = graph_get_vertices_count(graph);

    *discovery_times = (int*) calloc(vertices_count, sizeof(int));  //initialize elems to 0
    if (*discovery_times == NULL) return MAIN_OUT_OF_MEM;

    *finish_times = (int*) calloc(vertices_count, sizeof(int)); //initialize elems to 0
    if (*finish_times == NULL) return MAIN_OUT_OF_MEM;

    *parents = (int*) malloc(vertices_count * sizeof(int));
    if (*parents == NULL) return MAIN_OUT_OF_MEM;
    for (int i = 0; i < vertices_count; i++) (*parents)[i] = INT_MAX; //initialize elems to biggest possible value

    return MAIN_OK;
}

//============ KRUSKAL TEST AUX ==============

int main_aux_init_test_Kruskal(void* graph, long** edges_weights_matrix, long*** weighted_edges_list, int*** mst_edges_list) {
    int vertices_count = graph_get_vertices_count(graph);
    int edges_count = graph_get_edges_count(graph);
    int is_directed = graph_is_directed(graph);

    *weighted_edges_list = (long**) malloc(edges_count * sizeof(long*));
    if (*weighted_edges_list == NULL) return MAIN_OUT_OF_MEM; //malloc fail

    for (int i = 0; i < edges_count; i++) {
        (*weighted_edges_list)[i] = (long*) calloc(3, sizeof(long));
        if ((*weighted_edges_list)[i] == NULL) return MAIN_OUT_OF_MEM; //malloc fail
    }

    //we convert to a 'list' because we want to sort
    longs_matrix_weights_list_from_weights_matrix(vertices_count, is_directed, *weighted_edges_list, edges_weights_matrix);

    *mst_edges_list = edges_list_init_edges_list(vertices_count - 1);
    if (*mst_edges_list == NULL) return MAIN_OUT_OF_MEM; //malloc fail
}


//============== PRIM TEST AUX ===============

int main_aux_init_test_Prim(void* graph, int* source, int** mst_parents) {
    int vertices_count = graph_get_vertices_count(graph);

    *mst_parents = (int*) malloc(vertices_count * sizeof(int));
    if (*mst_parents == NULL) return MAIN_OUT_OF_MEM;
    for (int i = 0; i < vertices_count; i++) (*mst_parents)[i] = INT_MAX; //initialize elems to biggest possible value (?)

    printf("\nWhich vertex is the source? ");
    *source = -1;
    while ((*source < 0) || (*source >= vertices_count)) scanf_s("%d%*c", source);

    return MAIN_OK;
}

//------------------------------

void main_aux_print_test_Prim(int vertices_count, long** edges_weights_matrix, int* mst_parents) {
    long tot_weight = 0;

    printf("\nthe edges of the MST:\n");
    for (size_t i = 0; i < vertices_count; i++) {
        if (mst_parents[i] >= vertices_count) continue;

        tot_weight += edges_weights_matrix[mst_parents[i]][i];

        printf("%d %d\n", mst_parents[i], i);
    }

    printf("\nweighing: %ld\n", tot_weight);
}

//============== SSSP TEST AUX ===============

int main_aux_init_test_SSSP(void* graph, int* source, long** path_weights_from_source, int** sp_parents) {
    int vertices_count = graph_get_vertices_count(graph);

    *sp_parents = (int*) malloc(vertices_count * sizeof(int)); 
    if (*sp_parents == NULL) return MAIN_OUT_OF_MEM;

    *path_weights_from_source = (long*) malloc(vertices_count * sizeof(long));
    if (*path_weights_from_source == NULL) return MAIN_OUT_OF_MEM;

    printf("\nWhich vertex is the source? ");
    *source = -1;
    while ((*source < 0) || (*source >= vertices_count)) scanf_s("%d%*c", source);

    return MAIN_OK;
}

//------------------------------

int main_aux_print_test_SSSP(void* graph, int source, long* path_weights_from_source, int* sp_parents) {
    int vertices_count = graph_get_vertices_count(graph);

    printf("\nthe edges of the ssspTree:\n");
    for (int i = 0; i < vertices_count; i++) 
        if (sp_parents[i] < vertices_count) printf("%d %d\n", sp_parents[i], i);

    printf("\nthe weight of the lightest path from %d to each vertex:\n", source);
    for (int i = 0; i < vertices_count; i++) {
        printf("to %d it's: ", i);

        if (path_weights_from_source[i] == LONG_MAX / 2) printf("%d is not reachable from %d\n", i, source);
        else printf("%ld\n", path_weights_from_source[i]);
    }

    int sp_edges_count;

    int** sp_edges_list = main_aux_parents_to_edges_list_conversion(sp_parents, vertices_count, &sp_edges_count);
    if (sp_edges_list == NULL) return -1; //malloc fail

    void* sp_tree = graph_init_graph_from_edges_list(vertices_count, sp_edges_count, 1, sp_edges_list, Adjacency_List);
    if (sp_tree == NULL) return -1; //malloc fail

    printf("\nPrinting sp_tree as a graph:\n");
    graph_print_graph(sp_tree);

    graph_free_graph(sp_tree, NULL);

    return 0;
}

//============== APSP TEST AUX ===============

int main_aux_init_test_APSP(void* graph, long*** path_weights_from_src_to_trgt, int*** apsp_parents) {
    int vertices_count = graph_get_vertices_count(graph);

    *apsp_parents = (int**)malloc(vertices_count * sizeof(int*));
    if (*apsp_parents == NULL) return MAIN_OUT_OF_MEM;

    for (int i = 0; i < vertices_count; i++) {
        (*apsp_parents)[i] = (int*) malloc(vertices_count * sizeof(int));
        if ((*apsp_parents)[i] == NULL) return MAIN_OUT_OF_MEM;

        for (int j = 0; j < vertices_count; j++) 
            (*apsp_parents)[i][j] = INT_MAX;  //initialize elems to biggest possible value
    }

    *path_weights_from_src_to_trgt = longs_matrix_init_longs_matrix(vertices_count, LONG_MAX / 2);
    if (*path_weights_from_src_to_trgt == NULL) return MAIN_OUT_OF_MEM;

    return MAIN_OK;
}

//------------------------------

void main_aux_print_lightest_path(int vertices_count, int** apsp_parents, int i, int j) {
    if (i == j) printf("%d ", i);
    else if (apsp_parents[i][j] == INT_MAX) printf("%d is not reachable from %d", j, i);
    else {
        main_aux_print_lightest_path(vertices_count, apsp_parents, i, apsp_parents[i][j]);

        printf("-> %d ", j);
    }
}

//------------------------------

void main_aux_print_test_APSP(void* graph, long** path_weights_from_src_to_trgt, int** apsp_parents) {
    int vertices_count = graph_get_vertices_count(graph);

    printf("\nLightest weights matrix:\n");
    longs_matrix_print_longs_matrix(vertices_count, path_weights_from_src_to_trgt, LONG_MAX / 2);

    printf("\nParents matrix:\n");
    for (int i = 0; i < vertices_count; i++) {
        for (int j = 0; j < vertices_count; j++) {
            if (i == j) printf("@ ");
            else printf("%d ", apsp_parents[i][j]);
        }

        putchar('\n');
    }

    /* 
    //FIX: infinite loop when graph contains zero weight cycle
    
    for (int i = 0; i < vertices_count; i++) {
        printf("~~~~~\nsource is %d:\n", i);

        for (int j = 0; j < vertices_count; j++) {
            printf("\nlightest path from %d to %d: ", i, j);

            main_aux_print_lightest_path(vertices_count, apsp_parents, i, j);
        }

        putchar('\n');
    }

    */
}
