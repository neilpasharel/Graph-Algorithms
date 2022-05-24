
#include <stdio.h>
#include <stdlib.h>
#include "Longs_Matrix.h"


//--------------------------------------------------------------

long** longs_matrix_init_longs_matrix(int order, long init_value) {
	long** matrix = (long**) malloc(order * sizeof(long*));
	if (matrix == NULL) return NULL;

	for (int i = 0; i < order; i++) {
		matrix[i] = (long*) malloc(order * sizeof(long));
		if (matrix[i] == NULL) return NULL;

		for (int j = 0; j < order; j++) matrix[i][j] = init_value;
	}

	return matrix;
}

//--------------------------------------------------------------

void longs_matrix_free_longs_matrix(int order, long** matrix) {
	for (int i = 0; i < order; i++) free(matrix[i]);

	free(matrix);
}

//--------------------------------------------------------------

void longs_matrix_hard_copy_longs_matrix(int order, long** dest, long** src) {
	for (int i = 0; i < order; i++)
		for (int j = 0; j < order; j++)
			dest[i][j] = src[i][j];
}

//--------------------------------------------------------------

int longs_matrix_compare_longs_matrices(int order, long** A, long** B) {
	for (int i = 0; i < order; i++)
		for (int j = 0; j < order; j++)
			if (A[i][j] != B[i][j]) return 1;

	return 0;
}

//--------------------------------------------------------------

void longs_matrix_print_longs_matrix(int order, long** matrix, long lowerBound) {
	long tmp;

	for (int i = 0; i < order; i++) {
		for (int j = 0; j < order; j++) {
			tmp = matrix[i][j];

			if (tmp >= lowerBound) printf("inf ");
			else printf("%3ld ", tmp);
		}

		putchar('\n');
	}
}

//--------------------------------------------------------------

void longs_matrix_weights_list_from_weights_matrix(int order,
												   int is_directed, 
												   long** weighted_edges_list, 
												   long** edges_weights_matrix) {
	int pos = 0;
	long w_ij;

	for (int i = 0; i < order; i++) {
		for (int j = 0; j < order; j++) {
			if ((i == j) || (edges_weights_matrix[i][j] == LONG_MAX / 2)) continue;

			w_ij = edges_weights_matrix[i][j];

			if (is_directed == 0) {
				if (w_ij == LONG_MIN) {
					edges_weights_matrix[i][j] = edges_weights_matrix[j][i];
					continue;
				}
				else edges_weights_matrix[j][i] = LONG_MIN;
			}

			weighted_edges_list[pos][0] = i;
			weighted_edges_list[pos][1] = j;
			weighted_edges_list[pos][2] = w_ij;

			pos++;
		}
	}

	printf("\nthe weighted edges in list rep:\n");
	for (size_t i = 0; i < pos; i++)
		printf("%ld %ld %ld\n", weighted_edges_list[i][0], weighted_edges_list[i][1], weighted_edges_list[i][2]);
}

//--------------------------------------------------------------