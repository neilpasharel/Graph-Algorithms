
#include <stdio.h>
#include <stdlib.h>
#include "Edges_List.h"

//----------------------------------------------------------------

int** edges_list_init_edges_list(int edges_count) {
	int** edges_list = (int**) malloc(edges_count * sizeof(int*));
	if (edges_list == NULL) return NULL; //malloc fail

	for (int i = 0; i < edges_count; i++) {
		edges_list[i] = (int*) calloc(2, sizeof(int));
		if (edges_list[i] == NULL) return NULL; //malloc fail
	}

	return edges_list;
}

//----------------------------------------------------------------

void edges_list_print_edges_list(int** edges_list, int edges_count) {
	for (int i = 0; i < edges_count; i++) printf("%d %d\n", edges_list[i][0], edges_list[i][1]);
}

//----------------------------------------------------------------

void edges_list_free_edges_list(int** edges_list, int edges_count) {
	for (int i = 0; i < edges_count; i++) free(edges_list[i]);

	free(edges_list);
}

//----------------------------------------------------------------