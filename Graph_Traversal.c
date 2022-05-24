
#include <stdio.h>
#include <stdlib.h>
#include "Graph_Traversal.h"
#include "Queue.h"
#include "Graph_Transpose.h"

typedef enum Color { White, Grey, Black } Color;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int init_BFS(void* graph, int* source, int* distances, Color** colors, void** queue) {
    *queue = queue_init_queue();
    if (*queue == NULL) return 0;

    int vertices_count = graph_get_vertices_count(graph);

    *colors = (Color*) malloc(vertices_count * sizeof(Color));
    if (*colors == NULL) return 0;
    for (int i = 0; i < vertices_count; i++) (*colors)[i] = White;

    (*colors)[*source] = Grey;
    distances[*source] = 0;

    queue_enqueue(*queue, (void*) source);

    return 1;
}

//------------------------------

void BFS(void* graph, int* source, int* distances, int* bfs_parents) {
    void* queue = NULL;
    Color* colors = NULL;

    int check = init_BFS(graph, source, distances, &colors, &queue);
    if (check == 0) return; //malloc fail

    void* nbr_iter;
    int curr_vertex_dist;
    int* curr_vertex;

    while (queue_is_empty(queue) != 1) {
        curr_vertex = (int *) queue_dequeue(queue);
        curr_vertex_dist = distances[*curr_vertex];

        nbr_iter = graph_init_neighbors_iterator(graph, *curr_vertex);
        if (nbr_iter == NULL) return; //malloc fail

        while (graph_neighbors_iterator_has_next(nbr_iter) == 1) {
            int* curr_neighbor = (int*)malloc(sizeof(int));
            if (curr_neighbor == NULL) return; //malloc fail

            *curr_neighbor = graph_neighbors_iterator_next(nbr_iter, NULL);

            if (colors[*curr_neighbor] == White) {
                colors[*curr_neighbor] = Grey;
                bfs_parents[*curr_neighbor] = *curr_vertex;
                distances[*curr_neighbor] = curr_vertex_dist + 1;

                queue_enqueue(queue, curr_neighbor);
            }
        }
        graph_free_neighbors_iterator(nbr_iter);

        colors[*curr_vertex] = Black;

        free(curr_vertex);
    }

    queue_free_queue(queue);
    free(colors);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void DFS_visit(void* graph, int vertex, int* discovery_times, int* dfs_parents, int* finish_times, Color* colors, int* time) {
    (*time)++;
    discovery_times[vertex] = *time;
    colors[vertex] = Grey;

    int curr_neighbor;

    void* v_iter = graph_init_neighbors_iterator(graph, vertex);
    while (graph_neighbors_iterator_has_next(v_iter) == 1) {
        curr_neighbor = graph_neighbors_iterator_next(v_iter, NULL);

        if (colors[curr_neighbor] == White) {
            dfs_parents[curr_neighbor] = vertex;
            DFS_visit(graph, curr_neighbor, discovery_times, dfs_parents, finish_times, colors, time);
        }
    }
    graph_free_neighbors_iterator(v_iter);

    colors[vertex] = Black;
    (*time)++;
    finish_times[vertex] = *time;
}

//---------------------------------

void DFS(void* graph, int* discovery_times, int* dfs_parents, int* finish_times) {
    int vertices_count = graph_get_vertices_count(graph);

    Color* colors = (Color*) calloc(vertices_count, sizeof(Color));
    if (colors == NULL) return;
    for (int i = 0; i < vertices_count; i++) colors[i] = White;

    int time = 0;

    for (int i = 0; i < vertices_count; i++) {
        if (colors[i] == White) DFS_visit(graph, i, discovery_times, dfs_parents, finish_times, colors, &time);
    }

    free(colors);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void topo_sort_DFS_visit(void* graph, int vertex, Color* colors, int* topo_sort, int* pos) {
    colors[vertex] = Grey;

    int curr_neighbor;

    void* vIter = graph_init_neighbors_iterator(graph, vertex);
    if (vIter == NULL) return; //malloc fail

    while (graph_neighbors_iterator_has_next(vIter) == 1) {
        curr_neighbor = graph_neighbors_iterator_next(vIter, NULL);

        if (colors[curr_neighbor] == White) topo_sort_DFS_visit(graph, curr_neighbor, colors, topo_sort, pos);
    }
    graph_free_neighbors_iterator(vIter);

    colors[vertex] = Black;
    topo_sort[*pos] = vertex;
    (*pos)--;
}

//---------------------------------

void topo_sort_DFS(void* graph, int* topo_sort) {
    int vertices_count = graph_get_vertices_count(graph);
    int pos = vertices_count -1;

    Color* colors = (Color*) calloc(vertices_count, sizeof(Color));
    if (colors == NULL) return; //malloc fail
    for (int i = 0; i < vertices_count; i++) colors[i] = White;

    for (int i = 0; i < vertices_count; i++) 
        if (colors[i] == White) topo_sort_DFS_visit(graph, i, colors, topo_sort, &pos);

    free(colors);
}

//---------------------------------

int* topological_sort(void* graph) {
    int vertices_count = graph_get_vertices_count(graph);

    int* topoSort = (int*) calloc(vertices_count, sizeof(int));
    if (topoSort == NULL) return NULL;

    topo_sort_DFS(graph, topoSort);

    return topoSort;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void scc_DFS_visit(void* graph, int vertex, Color* colors, int** SCCs, int* SCCs_counts, int SCCs_count) {
    colors[vertex] = Grey;
    SCCs_counts[SCCs_count - 1]++;

    SCCs[SCCs_count - 1] = (int*) realloc(SCCs[SCCs_count - 1], SCCs_counts[SCCs_count - 1] * sizeof(int));
    if (SCCs[SCCs_count - 1] == NULL) return;
    SCCs[SCCs_count - 1][SCCs_counts[SCCs_count - 1] - 1] = vertex;

    int curr_neighbor;

    void* v_iter = graph_init_neighbors_iterator(graph, vertex);
    if (v_iter == NULL) return; //malloc fail

    while (graph_neighbors_iterator_has_next(v_iter) == 1) {
        curr_neighbor = graph_neighbors_iterator_next(v_iter, NULL);

        if (colors[curr_neighbor] == White) scc_DFS_visit(graph, curr_neighbor, colors, SCCs, SCCs_counts, SCCs_count);
    }
    graph_free_neighbors_iterator(v_iter);

    colors[vertex] = Black;
}

//---------------------------------

void scc_DFS(void* graph, int* topo_sort_order, int*** SCCs, int** SCCs_counts, int* SCCs_count, Color* colors) {
    int vertices_count = graph_get_vertices_count(graph);

    for (int i = 0; i < vertices_count; i++) {
        if (colors[topo_sort_order[i]] == White) {
            (*SCCs_count)++;

            *SCCs = (int**) realloc(*SCCs, *SCCs_count * sizeof(int*));
            if (*SCCs == NULL) return; //malloc fail
            (*SCCs)[*SCCs_count - 1] = (int*)malloc(sizeof(int));

            *SCCs_counts = (int*) realloc(*SCCs_counts, *SCCs_count * sizeof(int));
            if (*SCCs_counts == NULL) return; //malloc fail
            (*SCCs_counts)[*SCCs_count - 1] = 0;

            scc_DFS_visit(graph, topo_sort_order[i], colors, *SCCs, *SCCs_counts, *SCCs_count);
        }
    }
}

//---------------------------------

int** compute_strongly_connected_components_graph(void* graph, int** SCCs_counts, int* SCCs_count) {
    int vertices_count = graph_get_vertices_count(graph);

    int* topo_sort_order = topological_sort(graph);

    Color* colors = (Color*) calloc(vertices_count, sizeof(Color));
    if (colors == NULL) return NULL; //malloc fail
    for (int i = 0; i < vertices_count; i++) colors[i] = White;

    void* graph_t = transpose_graph(graph, 0, 0);
    if (graph_t == NULL) return NULL; //malloc fail

    int** SCCs = (int**) malloc(sizeof(int*));
    if (SCCs == NULL) return NULL; //malloc fail

    scc_DFS(graph_t, topo_sort_order, &SCCs, SCCs_counts, SCCs_count, colors);

    free(topo_sort_order);
    free(colors);
    graph_free_graph(graph_t, NULL);

    return SCCs;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~