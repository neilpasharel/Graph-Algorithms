
#include <stdio.h>
#include <stdlib.h>
#include "All_Pairs_Shortest_Path.h"
#include "Longs_Matrix.h"

void square_matrices_mult(int order, long** A, long** B, long** res, int i, int j) {
	long res_ij = 0;

	for (int k = 0; k < order; k++)
		res_ij += (A[i][k] * B[k][j]);

	res[i][j] = res_ij;
}

//--------------------------

void compute_2_matrices_operation(int order, long** A, long** B, long** res,
								  void (*operate)(int, long**, long**, long**, int, int)) {
	for (int i = 0; i < order; i++) {
		for (int j = 0; j < order; j++)
			operate(order, A, B, res, i, j);
	}
}

//---------------------------------------------------------------------

void extend_lightest_paths(int vertices_count, long** A, long** B, long** res, int i, int j) {
	long res_ij = LONG_MAX / 2;	//divided by 2 to prevent overflow.. FIX
	long tmp;

	for (int k = 0; k < vertices_count; k++) {
		tmp = A[i][k] + B[k][j];
		if (res_ij > tmp) {
			res_ij = tmp;
		}
	}

	res[i][j] = res_ij;
}

//--------------------------

//FIXXXX: infinite loop if if there is a zero weight cycle in the graph :(
void compute_APSP_parents(int vertices_count, long** edges_weights_matrix, 
						  long** path_weights_from_src_to_trgt, int** apsp_parents) {
	for (int i = 0; i < vertices_count; i++) {
		for (int j = 0; j < vertices_count; j++) {
			if (i == j) continue;

			for (int k = 0; k < vertices_count; k++) {
				if (k == j) continue;

				if (path_weights_from_src_to_trgt[i][j] == path_weights_from_src_to_trgt[i][k] + edges_weights_matrix[k][j]) {
					apsp_parents[i][j] = k;
					break;
				}
			}
		}
	}
}

//--------------------------

int APSP_mat_mult(int vertices_count, long** edges_weights_matrix, long** path_weights_from_src_to_trgt) {
	long** tmp = longs_matrix_init_longs_matrix(vertices_count, 0);
	if (tmp == NULL) return -1; //malloc fail

	longs_matrix_hard_copy_longs_matrix(vertices_count, path_weights_from_src_to_trgt, edges_weights_matrix);

	int k = 1;
	for (; k < vertices_count - 1; k *= 2) {
		compute_2_matrices_operation(vertices_count, 
									 path_weights_from_src_to_trgt, 
									 path_weights_from_src_to_trgt, 
									 tmp, 
									 extend_lightest_paths);

		longs_matrix_hard_copy_longs_matrix(vertices_count, path_weights_from_src_to_trgt, tmp);

		//printf("APSP weight matrix for iter %d:\n", k);
		//longs_matrix_print_longs_matrix(vertices_count, path_weights_from_src_to_trgt, 4 * (LONG_MAX /10));
	}

	k++;
	compute_2_matrices_operation(vertices_count, 
								 path_weights_from_src_to_trgt, 
								 edges_weights_matrix, 
								 tmp, 
								 extend_lightest_paths);

	int check = longs_matrix_compare_longs_matrices(vertices_count, path_weights_from_src_to_trgt, tmp);
	if (check == 1) return 1;

	free(tmp);

	return 0;
}

//----------------------------------------------------------------

//FIX BUG: first row is wrong (other rows are right..?)
int Floyd_Warshall(int vertices_count, long** edges_weights_matrix, long** path_weights_from_src_to_trgt, int** apsp_parents) {
	long** tmp = longs_matrix_init_longs_matrix(vertices_count, 0);
	if (tmp == NULL) return -1; //malloc fail

	longs_matrix_hard_copy_longs_matrix(vertices_count, path_weights_from_src_to_trgt, edges_weights_matrix);

	long curr_weight_i_to_j;
	long curr_weight_i_to_k_plus_k_to_j;

	for (int k = 0; k < vertices_count - 1; k++) {
		for (int i = 0; i < vertices_count; i++) {
			for (int j = 0; j < vertices_count; j++) {
				curr_weight_i_to_j = path_weights_from_src_to_trgt[i][j];
				curr_weight_i_to_k_plus_k_to_j = path_weights_from_src_to_trgt[i][k] + path_weights_from_src_to_trgt[k][j];

				tmp[i][j] = ((curr_weight_i_to_j <= curr_weight_i_to_k_plus_k_to_j) ? 
							  curr_weight_i_to_j : curr_weight_i_to_k_plus_k_to_j);
			}
		}
		longs_matrix_hard_copy_longs_matrix(vertices_count, path_weights_from_src_to_trgt, tmp);

		printf("APSP weight matrix for iter %d:\n", k);
		longs_matrix_print_longs_matrix(vertices_count, path_weights_from_src_to_trgt, 4 * (LONG_MAX / 10));
	}

	free(tmp);

	return 0;
}

//----------------------------------------------------------------