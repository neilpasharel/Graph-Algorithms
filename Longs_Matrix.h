
#ifndef __LONGS_MATRIX_H__
#define __LONGS_MATRIX_H__


long** longs_matrix_init_longs_matrix(int order, long init_value);

void longs_matrix_hard_copy_longs_matrix(int order, long** dest, long** src);

void longs_matrix_free_longs_matrix(int order, long** matrix);

int longs_matrix_compare_longs_matrices(int order, long** A, long** B); //returns (A == B ? 0 : 1);

void longs_matrix_print_longs_matrix(int order, long** matrix, long lowerBound);

void longs_matrix_weights_list_from_weights_matrix(int vertices_count,
												   int is_directed,
												   long** weighted_edges_list,
												   long** edges_weights_matrix);


#endif __LONGS_MATRIX_H__
