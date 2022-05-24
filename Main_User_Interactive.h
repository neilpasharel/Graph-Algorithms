
#ifndef __MAIN_USER_INTERACTIVE_H__
#define __MAIN_USER_INTERACTIVE_H__


#include "Abstract_Graph.h"

void* user_interactive_create_graph();

void user_interactive_add_weights_to_edges(void* graph, long*** edges_weights_matrix);


#endif __MAIN_USER_INTERACTIVE_H__
