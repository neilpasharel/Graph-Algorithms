
#include <stdio.h>
#include <stdlib.h>
#include "Queue.h"
#include "Doubly_Linked_List.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void* queue_init_queue() { 
    return doubly_ll_init_doubly_ll(NULL);
}


void queue_enqueue(void* queue, void* data) {
    doubly_ll_add_elem_by_pos(queue, 0, data, NULL);
}


void* queue_poll(void* queue) {
    return doubly_ll_get_elem_by_pos(queue, doubly_ll_get_size(queue) -1, NULL);
}


void* queue_dequeue(void* queue) {
    return doubly_ll_remove_elem_by_pos(queue, doubly_ll_get_size(queue) - 1, NULL);
}


void queue_free_queue(void* queue) {
    doubly_ll_free_doubly_ll(queue, NULL, NULL);
}


void queue_print_queue(void* queue, void (*print_elem)(void*)) {
    doubly_ll_print_doubly_ll(queue, print_elem, NULL);
}


int queue_is_empty(void* queue) {
    return ((doubly_ll_get_size(queue) == 0) ? 1 : 0);
}