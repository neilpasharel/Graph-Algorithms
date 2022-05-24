
#ifndef __PRIORITYQUEUE_H__
#define __PRIORITYQUEUE_H__

#define PQ_OK			 0 	// exit success
#define PQ_OUT_OF_MEM	-1 	// malloc fail
#define PQ_MISSING		-2	// pq does not contain this item

/*
* p_queue uses hash_map, and therefore needs a pointer to an int type 'hm_key' for each elem in it
* TODO: maybe change so that hm_key is a struct type..
*/

//TODO: a lot of changes, check if works..

typedef enum OPT { MIN, MAX } OPT;

void* priority_queue_init_pq(OPT opt, int max_amount_of_elements);

int priority_queue_insert(void* bheap, int* hm_key, int key, void* data);

int* priority_queue_poll_first(void* bheap);

int priority_queue_delete_elem_by_hm_key(void* bheap, int* hm_key, void** data);

int* priority_queue_extract_first(void* bheap, void** data);

int priority_queue_decrease_key(void* bheap, int* hm_key, int new_key);

int priority_queue_get_key_by_hm_key(void* bheap, int* hm_key, int* key);

int priority_queue_get_pos_in_pq_by_hm_key(void* bheap, int* hm_key, int* pos);

int priority_queue_get_size(void* bheap);

void priority_queue_print_pq(void* bheap);

void priority_queue_free_pq(void* bheap, void (*free_data)(void*));



#endif __PRIORITYQUEUE_H__
