
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Main_User_Interactive.h"
#include "Longs_Matrix.h"
#include "Edges_List.h"

//-------------------------------------------------------------------------------------------

int user_interactive_edges_list_build_menu() {
	printf("\n 1. Add an edge");
	printf("\n 2. Print graph");
	printf("\n 3. Exit creation loop");
	printf("\n What would you like to do: ");

	int i = -1;
	while ((i < 1) || (i > 3)) scanf_s("%d%*c", &i);

	return i;
}

//-------------------------------------------------------------------------------------------

void user_interactive_fill_edges_list(int** edges_list, int* edges_count, int vertices_count) {
	int i;
	int pos = 0;
	int key1, key2;

	while (1) {
		i = user_interactive_edges_list_build_menu();

		switch (i) {
		case 1:
			key1 = INT_MAX;
			key2 = INT_MAX;
			while ((key1 >= vertices_count) || (key2 >= vertices_count) || (key1 == key2)) {
				printf("\nWhich edge to create [specify it by 2 vertices indices]? ");
				scanf_s("%d %d%*c", &key1, &key2);
			}

			edges_list[pos][0] = key1;
			edges_list[pos][1] = key2;

			pos++;
			break;

		case 2:
			printf("\nPrinting edges list:\n");
			edges_list_print_edges_list(edges_list, pos);
			break;

		case 3:
			printf("\nExiting creation loop..\n");
			*edges_count = pos;
			return;
		}
	}
}

//-------------------------------------------------------------------------------------------

void* user_interactive_create_graph() {
	int vertices_count;
	int edges_count = 0;
	int is_directed;
	Graph_rep_type rep_type;

	printf("Let's build a graph:\n");

	printf("\nIs the graph to create directed [(1 / 0) for (directed / undirected)]? ");
	is_directed = -1;
	while ((is_directed != 0) && (is_directed != 1)) scanf_s("%d%*c", &is_directed);

	char type = 'a';
	printf("\nWhich representation type [(L / M) for (adjacency lists / adjacency matrix)]? ");
	while ((type != 'L') && (type != 'M')) scanf_s("%c%*c", &type);
	rep_type = ((type == 'L') ? Adjacency_List : Adjacency_Matrix);

	printf("\nHow many vertices? ");
	scanf_s("%d%*c", &vertices_count);

	int init_edges_count = (((int) pow(vertices_count, 2)) - vertices_count);
	if (is_directed == 0) init_edges_count /= 2; // maximum edges < 2 * nC2 .. (if directed, we count anti parallel edges)

	int** edges_list = edges_list_init_edges_list(init_edges_count);
	if (edges_list == NULL) return NULL; //malloc fail

	printf("\nLets add edges:\n");
	user_interactive_fill_edges_list(edges_list, &edges_count, vertices_count);

	for (int i = edges_count; i < init_edges_count; i++) free(edges_list[i]);
	edges_list = (int**) realloc(edges_list, edges_count * sizeof(int*));

	void* graph = graph_init_graph_from_edges_list(vertices_count, edges_count, is_directed, edges_list, rep_type);
	if (graph == NULL) return NULL; //malloc fail

	edges_list_free_edges_list(edges_list, edges_count);

	return graph;
}

//-------------------------------------------------------------------------------------------

void user_interactive_fill_edges_weights_matrix(void* graph, long** edges_weights_matrix) {
	int vertices_count = graph_get_vertices_count(graph);
	int is_directed = graph_is_directed(graph);
	void* v_iter;
	int curr_neighbor;
	long curr_edge_weight = LONG_MAX / 2;

	for (int i = 0; i < vertices_count; i++) {
		edges_weights_matrix[i][i] = 0;

		v_iter = graph_init_neighbors_iterator(graph, i);
		if (v_iter == NULL) return; //malloc fail

		while (graph_neighbors_iterator_has_next(v_iter) == 1) {
			curr_neighbor = graph_neighbors_iterator_next(v_iter, NULL);

			curr_edge_weight = edges_weights_matrix[i][curr_neighbor];
			if (curr_edge_weight != LONG_MAX / 2) continue;  //divided by 2 to prevent overflow.. FIX

			printf("Give weight to edge ( %d , %d ) : ", i, curr_neighbor);
			scanf_s("%ld%*c", &curr_edge_weight);

			edges_weights_matrix[i][curr_neighbor] = curr_edge_weight;
			if (is_directed == 0) edges_weights_matrix[curr_neighbor][i] = curr_edge_weight;
		}
		graph_free_neighbors_iterator(v_iter);
	}

	printf("\nthe weighted edges in matrix rep:\n");
	longs_matrix_print_longs_matrix(vertices_count, edges_weights_matrix, LONG_MAX / 2);
}

//-----------------------------------

void user_interactive_add_weights_to_edges(void* graph, long*** edges_weights_matrix) {
	int vertices_count = graph_get_vertices_count(graph);
	int edges_count = graph_get_edges_count(graph);

	*edges_weights_matrix = longs_matrix_init_longs_matrix(vertices_count, LONG_MAX / 2);
	if (*edges_weights_matrix == NULL) return; //malloc fail

	user_interactive_fill_edges_weights_matrix(graph, *edges_weights_matrix);
}