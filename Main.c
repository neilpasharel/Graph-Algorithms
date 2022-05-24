
#include <stdio.h>
#include <stdlib.h>
#include "Main_Aux.h"
#include "Main_User_Interactive.h"
#include "Abstract_Graph.h"
#include "Edges_List.h"
#include "Longs_Matrix.h"
#include "Graph_Traversal.h"
#include "Graph_Transpose.h"
#include "Minimum_Spanning_Tree.h"
#include "Single_Source_Shortest_Path.h"
#include "All_Pairs_Shortest_Path.h"

//-------------------------------------------------------------------------------------------

void test_convert_adj_list_rep_to_adj_mat_rep(void* al_graph) {
    if (graph_get_rep_type(al_graph) != Adjacency_List) {
        printf("Input graph needs to be in adj list rep.\nExiting test..\n");
        return;
    }

    void* am_graph = graph_convert_graph_rep(al_graph);

    printf("\nResult (printing both graphs):\n");

    printf("graph implemented by adjacency list:\n");
    graph_print_graph(al_graph);

    printf("\ngraph implemented by adjacency matrix:\n");
    graph_print_graph(am_graph);

    graph_free_graph(am_graph, NULL);
}

//-------------------------------------------------------------------------------------------

void test_convert_adj_mat_rep_to_adj_list_rep(void* am_graph) {
    if (graph_get_rep_type(am_graph) != Adjacency_Matrix) {
        printf("Input graph needs to be in adj matrix rep.\nExiting test..\n");
        return;
    }

    void* al_graph = graph_convert_graph_rep(am_graph);

    printf("\nResult (printing both graphs):\n");

    printf("printing graph implemented by adj matrix:\n");
    graph_print_graph(am_graph);

    printf("printing graph implemented by adj list:\n");
    graph_print_graph(al_graph);

    graph_free_graph(al_graph, NULL);
}

//-------------------------------------------------------------------------------------------

void test_adj_list_rep_graph_hard_copy(void* graph) {
    if (graph_get_rep_type(graph) != Adjacency_List) {
        printf("Input graph needs to be in adj list rep.\nExiting test..\n");
        return;
    }

    int vertices_count = graph_get_vertices_count(graph);

    void* graph_copy = graph_hard_copy_graph(graph, 0, 0);
    if (graph_copy == NULL) return; //malloc fail

    printf("\nResult (printing both graphs):\n");

    printf("orig graph:\n");
    graph_print_graph(graph);

    printf("\nhard copy graph:\n");
    graph_print_graph(graph_copy);

    graph_free_graph(graph_copy, NULL);
}

//-------------------------------------------------------------------------------------------

void test_traspose_graph(void* graph) {
    int vertices_count = graph_get_vertices_count(graph);

    void* graph_t = transpose_graph(graph, 0, 0);
    if (graph_t == NULL) return; //malloc fail

    printf("\nResult (printing both graphs):\n");
    printf("orig graph:\n");
    graph_print_graph(graph);

    printf("\ngraph transpose :\n");
    graph_print_graph(graph_t);

    graph_free_graph(graph_t, NULL);
}

//-------------------------------------------------------------------------------------------

void test_BFS(void* graph) {
    int vertices_count = graph_get_vertices_count(graph);
    int* shortest_path_distances = NULL;
    int* parents = NULL;
    int* source = (int*) malloc(sizeof(int));
    if (source == NULL) return; //malloc fail

    int check = main_aux_init_BFS_test(graph, source, &shortest_path_distances, &parents);
    if (check != 0) return; //malloc fail

    BFS(graph, source, shortest_path_distances, parents);

    /*
    printf("shortest path distances: ");
    main_aux_print_array((void*) shortestPathDistances, vertices_count, sizeof(int), main_aux_print_int_pointer);
    printf("parents: ");
    main_aux_print_array((void*) parents, vertices_count, sizeof(int), main_aux_print_int_pointer);
    */

    int bfs_edges_count;

    int** bfs_edges_list = main_aux_parents_to_edges_list_conversion(parents, vertices_count, &bfs_edges_count);
    if (bfs_edges_list == NULL) return; //malloc fail

    void* bfs_tree = graph_init_graph_from_edges_list(vertices_count, bfs_edges_count, 1, bfs_edges_list, Adjacency_List);
    if (bfs_tree == NULL) return; //malloc fail

    printf("\nResult (printing bfs_tree as a graph):\n");
    graph_print_graph(bfs_tree);

    free(shortest_path_distances);
    free(parents);
    free(source);
    graph_free_graph(bfs_tree, NULL);
}

//-------------------------------------------------------------------------------------------

void test_DFS(void* graph) {
    int vertices_count = graph_get_vertices_count(graph);
    int* discovery_times = NULL;
    int* finish_times = NULL;
    int* parents = NULL;

    int check = main_aux_init_DFS_test(&graph, &discovery_times, &finish_times, &parents);
    if (check != 0) return; //malloc fail

    DFS(graph, discovery_times, parents, finish_times);

    /*
    printf("discovery times: ");
    main_aux_print_array((void*) discoveryTimes, vertices_count, sizeof(size_t), main_aux_print_int_pointer);
    printf("parents: ");
    main_aux_print_array((void*) parents, vertices_count, sizeof(size_t), main_aux_print_int_pointer);
    printf("finishTimes: ");
    main_aux_print_array((void*) finishTimes, vertices_count, sizeof(size_t), main_aux_print_int_pointer);
    */

    int dfs_edges_count;

    int** dfs_edges_list = main_aux_parents_to_edges_list_conversion(parents, vertices_count, &dfs_edges_count);
    if (dfs_edges_list == NULL) return; //malloc fail

    void* dfs_tree = graph_init_graph_from_edges_list(vertices_count, dfs_edges_count, 1, dfs_edges_list, Adjacency_List);
    if (dfs_tree == NULL) return; //malloc fail

    printf("\nResult (printing dfs_tree as a graph):\n");
    graph_print_graph(dfs_tree);

    free(discovery_times);
    free(parents);
    free(finish_times);
    graph_free_graph(dfs_tree, NULL);
}

//-------------------------------------------------------------------------------------------

void test_topological_sort(void* graph) {
    int vertices_count = graph_get_vertices_count(graph);

    int* topo_sort = topological_sort(graph);
    if (topo_sort == NULL) return; //malloc fail

    printf("\nResult (printing topological sort order):\n");
    main_aux_print_array((void*) topo_sort, vertices_count, sizeof(int), main_aux_print_int_pointer);

    free(topo_sort);
}

//-------------------------------------------------------------------------------------------

void test_strongly_connected_components(void* graph) {
    int SCCs_count = 0;

    int* SCCs_counts = (int*) calloc(1, sizeof(int));
    if (SCCs_counts == NULL) return; //malloc fail

    int** res = compute_strongly_connected_components_graph(graph, &SCCs_counts, &SCCs_count);
    if (res == NULL) return; //malloc fail

    printf("\nResult (printing the SCCs):\n");
    for (int i = 0; i < SCCs_count; i++) {
        printf("- ");
        main_aux_print_array((void*)res[i], SCCs_counts[i], sizeof(int), main_aux_print_int_pointer);
    }

    for (int i = 0; i < SCCs_count; i++) free(res[i]);
    free(res);

    free(SCCs_counts);
}

//-------------------------------------------------------------------------------------------

void test_Kruskal(void* graph) {
    int vertices_count = graph_get_vertices_count(graph);
    int edges_count = graph_get_edges_count(graph);
    long** edges_weights_matrix = NULL;
    long** weighted_edges_list = NULL;
    int** mst_edges_list = NULL;

    user_interactive_add_weights_to_edges(graph, &edges_weights_matrix);
    if (edges_weights_matrix == NULL) return; //malloc fail

    main_aux_init_test_Kruskal(graph, edges_weights_matrix, &weighted_edges_list, &mst_edges_list);
    
    Kruskal(graph, weighted_edges_list, mst_edges_list);

    printf("\nResult (edges of an MST):\n");
    edges_list_print_edges_list(mst_edges_list, vertices_count - 1);

    for (int i = 0; i < edges_count; i++) free(weighted_edges_list[i]);
    free(weighted_edges_list);

    edges_list_free_edges_list(mst_edges_list, vertices_count -1);   
    longs_matrix_free_longs_matrix(vertices_count, edges_weights_matrix);
}

//-------------------------------------------------------------------------------------------

void test_Prim(void* graph) {
    int vertices_count = graph_get_vertices_count(graph);
    long** edges_weights_matrix = NULL;
    int* mst_parents = NULL; //res
    int source;

    user_interactive_add_weights_to_edges(graph, &edges_weights_matrix);
    if (edges_weights_matrix == NULL) return; //malloc fail

    int check = main_aux_init_test_Prim(graph, &source, &mst_parents);
    if (check != 0) return; //malloc fail

    Prim(graph, source, edges_weights_matrix, mst_parents);

    printf("\nResult (edges of an MST):\n");
    main_aux_print_test_Prim(vertices_count, edges_weights_matrix, mst_parents);

    longs_matrix_free_longs_matrix(vertices_count, edges_weights_matrix);
    free(mst_parents);
}

//-------------------------------------------------------------------------------------------

void test_Bellman_Ford(void* graph) {
    int vertices_count = graph_get_vertices_count(graph);
    long** edges_weights_matrix = NULL;
    long* path_weights_from_source = NULL; //res
    int* sp_parents = NULL; //res
    int source;

    user_interactive_add_weights_to_edges(graph, &edges_weights_matrix);
    if (edges_weights_matrix == NULL) return; //malloc fail

    int check = main_aux_init_test_SSSP(graph, &source, &path_weights_from_source, &sp_parents);
    if (check != 0) return; //malloc fail

    check = Bellman_Ford(graph, source, edges_weights_matrix, path_weights_from_source, sp_parents);
    if (check == -1) return; //malloc fail

    printf("\nResult:\n<graph contains a neg weight cycle>? %s\n", ((check == 1) ? "YES" : "NO"));

    if (check == 0) {
        check = main_aux_print_test_SSSP(graph, source, path_weights_from_source, sp_parents);
        if (check != 0) return; //malloc fail
    }

    longs_matrix_free_longs_matrix(vertices_count, edges_weights_matrix);
    free(path_weights_from_source);
    free(sp_parents);
}

//-------------------------------------------------------------------------------------------

void test_DAG_SSSP(void* graph) {
    int vertices_count = graph_get_vertices_count(graph);
    long** edges_weights_matrix = NULL;
    long* path_weights_from_source = NULL; //res
    int* sp_parents = NULL; //res
    int source;

    user_interactive_add_weights_to_edges(graph, &edges_weights_matrix);
    if (edges_weights_matrix == NULL) return; //malloc fail

    int check = main_aux_init_test_SSSP(graph, &source, &path_weights_from_source, &sp_parents);
    if (check != 0) return; //malloc fail

    DAG_SSSP(graph, source, edges_weights_matrix, path_weights_from_source, sp_parents);

    printf("\nResult:\n");
    check = main_aux_print_test_SSSP(graph, source, path_weights_from_source, sp_parents);
    if (check != 0) return; //malloc fail

    longs_matrix_free_longs_matrix(vertices_count, edges_weights_matrix);
    free(path_weights_from_source);
    free(sp_parents);
}

//-------------------------------------------------------------------------------------------

void test_Dijkstra(void* graph) {
    int vertices_count = graph_get_vertices_count(graph);
    long** edges_weights_matrix = NULL;
    long* path_weights_from_source = NULL; //res
    int* sp_parents = NULL; //res
    int source;

    user_interactive_add_weights_to_edges(graph, &edges_weights_matrix);
    if (edges_weights_matrix == NULL) return; //malloc fail

    int check = main_aux_init_test_SSSP(graph, &source, &path_weights_from_source, &sp_parents);
    if (check != 0) return; //malloc fail

    Dijkstra(graph, source, edges_weights_matrix, path_weights_from_source, sp_parents);

    printf("\nResult:\n");
    check = main_aux_print_test_SSSP(graph, source, path_weights_from_source, sp_parents);
    if (check != 0) return; //malloc fail

    longs_matrix_free_longs_matrix(vertices_count, edges_weights_matrix);
    free(path_weights_from_source);
    free(sp_parents);
}

//-------------------------------------------------------------------------------------------

void test_APSP_mat_mult(void* graph) {
    int vertices_count = graph_get_vertices_count(graph);
    long** edges_weights_matrix = NULL;
    long** path_weights_from_src_to_trgt = NULL; //res
    int** apsp_parents = NULL; //res

    user_interactive_add_weights_to_edges(graph, &edges_weights_matrix);
    if (edges_weights_matrix == NULL) return; //malloc fail

    int check = main_aux_init_test_APSP(graph, &path_weights_from_src_to_trgt, &apsp_parents);
    if (check != 0) return; //malloc fail

    check = APSP_mat_mult(vertices_count, edges_weights_matrix, path_weights_from_src_to_trgt);
    if (check != 0) return; //malloc fail

    printf("\nResult:\n<graph contains a neg weight cycle>? %s\n", ((check == 1) ? "YES" : "NO"));

    if (check == 0) {
        compute_APSP_parents(vertices_count, edges_weights_matrix, path_weights_from_src_to_trgt, apsp_parents);

        main_aux_print_test_APSP(graph, path_weights_from_src_to_trgt, apsp_parents);
    }

    for (int i = 0; i < vertices_count; i++) free(apsp_parents[i]);
    free(apsp_parents);

    longs_matrix_free_longs_matrix(vertices_count, edges_weights_matrix);

    longs_matrix_free_longs_matrix(vertices_count, path_weights_from_src_to_trgt);
}

//-------------------------------------------------------------------------------------------

void test_Floyd_Warshall(void* graph) {
    int vertices_count = graph_get_vertices_count(graph);
    long** edges_weights_matrix = NULL;
    long** path_weights_from_src_to_trgt = NULL; //res
    int** apsp_parents = NULL; //res

    user_interactive_add_weights_to_edges(graph, &edges_weights_matrix);
    if (edges_weights_matrix == NULL) return; //malloc fail

    int check = main_aux_init_test_APSP(graph, &path_weights_from_src_to_trgt, &apsp_parents);
    if (check != 0) return; //malloc fail

    check = Floyd_Warshall(vertices_count, edges_weights_matrix, path_weights_from_src_to_trgt, apsp_parents);
    if (check == -1) return; //malloc fail

    /*
    printf("\nResult:\n<graph contains a neg weight cycle>? %s\n", (check == 1 ? "YES" : "NO"));

    if (check == 0) {
        main_aux_print_test_APSP(graph, path_weights_from_src_to_trgt, apsp_parents);
    }
    */

    for (int i = 0; i < vertices_count; i++) free(apsp_parents[i]);
    free(apsp_parents);

    longs_matrix_free_longs_matrix(vertices_count, edges_weights_matrix);

    longs_matrix_free_longs_matrix(vertices_count, path_weights_from_src_to_trgt);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


int main() {
    int tests_count = 15;
    int check, i;

    void* graph = user_interactive_create_graph();

    // an array of pointers to functions
    void (*tests[])(void* graph) = { 
                          test_convert_adj_list_rep_to_adj_mat_rep ,
                          test_convert_adj_mat_rep_to_adj_list_rep ,
                          test_adj_list_rep_graph_hard_copy ,
                          test_traspose_graph ,
                          test_BFS ,
                          test_DFS ,
                          test_topological_sort ,
                          test_strongly_connected_components ,
                          test_Kruskal ,
                          test_Prim ,
                          test_Bellman_Ford ,
                          test_DAG_SSSP ,
                          test_Dijkstra ,
                          test_APSP_mat_mult ,
                          test_Floyd_Warshall
    };

    char* names[] = { 
                      "convert graph rep: list -> matrix" ,
                      "convert graph rep: matrix -> list" ,
                      "create hard copy" ,
                      "create graph transpose" ,
                      "BFS graph traversal" ,
                      "DFS graph traversal" ,
                      "topological sort" ,
                      "strongly connected components" ,
                      "MST Kruskal" ,
                      "MST Prim" ,
                      "SSSP Bellman-Ford" ,
                      "SSSP in DAG" ,
                      "SSSP Dijkstra (assumes no negative weighted edges)",
                      "APSP matrix multiplication (extend path dynammic programming)" ,
                      "APSP Floyd-Warshall (need to add check negative weight cycle...)"
    };

    while (1) {
        printf("\nTESTS:\n");
        for (i = 1; i <= tests_count; i++) {
            printf("%2d. %s\n", i, names[i - 1]);
        }

        printf("\nWhich test to run? ");
        i = -1;
        while ((i <= 0) || ( i > tests_count)) scanf_s("%d%*c", &i);

        //printf("\nReminder: for yes/no questions, answer 1 for yes, and 0 for no.\n\n");

        printf("\nDo you want to continue with current graph, or build a new one?\n");
        printf("(there are tests which require specific graph properties..)\n");
        check = -1;
        while ((check != 0) && (check != 1)) scanf_s("%d%*c", &check);
        if (check == 0) {
            free(graph);

            graph = user_interactive_create_graph();
        }

        printf("\n=============== TEST %02d ===============\n\n", i);
        printf("test name ~~ %s\n\n", names[i -1]);

        tests[i -1](graph);

        printf("\n============= TEST %02d DONE ============\n\n", i);

        printf("\nDo you want to quit? ");
        check = -1;
        while ((check != 0) && (check != 1)) scanf_s("%d%*c", &check);
        if (check == 0) continue;
        else break;
    }

    free(graph);

    return 0;
}