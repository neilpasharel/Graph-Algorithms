
#ifndef __APSP_H__
#define __APSP_H__

#include "Abstract_Graph.h"


int APSP_mat_mult(int vertices_count, long** edges_weights_matrix, long** path_weights_from_src_to_trgt);

//FIXXXX: infinite loop if if there is a zero weight cycle in the graph :(
void compute_APSP_parents(int vertices_count, long** edges_weights_matrix,
						  long** path_weights_from_src_to_trgt, int** apsp_parents);

int Floyd_Warshall(int vertices_count, long** edges_weights_matrix, long** path_weights_from_src_to_trgt, int** apsp_parents);


#endif __APSP_H__
