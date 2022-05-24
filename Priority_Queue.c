
#include <stdio.h>
#include <stdlib.h>
#include "Priority_Queue.h"
#include "Hash_Map.h"

#define INIT_ELEMS_CAP 40


typedef struct {
    int* hm_key;
    int key;
    void* data;
    int pos_in_pq;
} PQ_node;

typedef struct {
    void* map;
    PQ_node** p_queue;
    int size;
    int pq_cap;
    OPT opt;
} Bin_heap;

//--------------------------------------

PQ_node* priority_queue_init_node(int* hm_key, int key, void* data) {
    PQ_node* new_node = (PQ_node*) malloc(sizeof(PQ_node));
    if (new_node == NULL) return NULL;

    new_node->hm_key = hm_key;
    new_node->key = key;
    new_node->data = data;
    new_node->pos_in_pq = 0;

    return new_node;
}

//--------------------------------------

void priority_queue_free_node(PQ_node* node, void (*free_data)(void*)) {
    if (free_data) free_data(node->data);

    free(node);
}

//--------------------------------------

int priority_queue_left_child_pos_in_pq(int my_pos) {
    return 2 * my_pos;
}

int priority_queue_right_child_pos_in_pq(int my_pos) {
    return 2 * my_pos + 1;
}

int priority_queue_parent_pos_in_pq(int my_pos) {
    return my_pos / 2;
}

//--------------------------------------

void priority_queue_swap_nodes_positions_in_pq(PQ_node** a, PQ_node** b) {
    int pos_tmp = (*a)->pos_in_pq;
    (*a)->pos_in_pq = (*b)->pos_in_pq;
    (*b)->pos_in_pq = pos_tmp;

    PQ_node* node_tmp = *a;
    *a = *b;
    *b = node_tmp;
}

//--------------------------------------

void priority_queue_heapify_up(Bin_heap* heap, int pos) {
    int parents_pos;
    PQ_node* curr_parent;
    PQ_node* this_node = heap->p_queue[pos];

    while (pos > 1) {
        parents_pos = priority_queue_parent_pos_in_pq(pos);
        curr_parent = heap->p_queue[parents_pos];

        if ((heap->opt == MIN) ? (this_node->key >= curr_parent->key) : (this_node->key <= curr_parent->key)) break;

        priority_queue_swap_nodes_positions_in_pq(&(heap->p_queue[pos]), &(heap->p_queue[parents_pos]));

        pos = parents_pos;
    }
}

//--------------------------------------

void priority_queue_heapify_down(Bin_heap* heap, int pos) {
    int left_child_pos = priority_queue_left_child_pos_in_pq(pos);
    int right_child_pos = priority_queue_right_child_pos_in_pq(pos);
    int best_pos = pos;

    if ((left_child_pos <= heap->size) &&
        ((heap->opt == MIN) ?
            (heap->p_queue[left_child_pos]->key < heap->p_queue[best_pos]->key) :
            (heap->p_queue[left_child_pos]->key > heap->p_queue[best_pos]->key))) {
        best_pos = left_child_pos;
    }
    if ((right_child_pos <= heap->size) &&
        ((heap->opt == MIN) ?
            (heap->p_queue[right_child_pos]->key < heap->p_queue[best_pos]->key) :
            (heap->p_queue[right_child_pos]->key > heap->p_queue[best_pos]->key))) {
        best_pos = right_child_pos;
    }
    if (best_pos != pos) {
        priority_queue_swap_nodes_positions_in_pq(&(heap->p_queue[best_pos]), &(heap->p_queue[pos]));

        priority_queue_heapify_down(heap, best_pos);
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

size_t hash_func_for_int_keys(const void* key) {
    size_t hash = *((size_t*) key);

    hash += (hash << 12);
    hash ^= (hash >> 22);
    hash += (hash << 4);
    hash ^= (hash >> 9);
    hash += (hash << 10);
    hash ^= (hash >> 2);
    hash += (hash << 7);
    hash ^= (hash >> 12);
    hash = (hash >> 3) * 2654435761;

    return hash;
}

int compare_ints(const void* a, const void* b) {
    int aa = *((int*)a);
    int bb = *((int*)b);

    return aa - bb;
}

void queue_print_int(void* key) {
    printf("%d", *((int*)key));
}

//--------------------------------------

void* priority_queue_init_pq(OPT opt, int max_amount_of_elements) {
    Bin_heap* heap = (Bin_heap*) malloc(sizeof(Bin_heap));
    if (heap == NULL) return NULL;

    heap->p_queue = (PQ_node**) calloc(INIT_ELEMS_CAP, sizeof(PQ_node*));
    if (heap->p_queue == NULL) {
        free(heap);

        return NULL;
    }

    heap->map = hash_map_init_hash_map(max_amount_of_elements, hash_func_for_int_keys, compare_ints);
    if (heap->map == NULL) {
        free(heap->p_queue);
        free(heap);

        return NULL;
    }

    heap->size = 0;
    heap->pq_cap = INIT_ELEMS_CAP;
    heap->opt = opt;

    return (void*) heap;
}

//--------------------------------------------------------------------------------

int priority_queue_insert(void* bheap, int* hm_key, int key, void* data) {
    Bin_heap* heap = (Bin_heap*) bheap;

    void** tmp = NULL;
    if (hash_map_find(heap->map, hm_key, tmp) == MAP_OK) return priority_queue_decrease_key(bheap, hm_key, key);

    heap->size++;

    if (heap->size > (3 * heap->pq_cap) / 4) {
        heap->pq_cap += INIT_ELEMS_CAP;

        heap->p_queue = (PQ_node**) realloc(heap->p_queue, heap->pq_cap * sizeof(PQ_node*));

        if (heap->p_queue == NULL) return PQ_OUT_OF_MEM;
    }

    PQ_node* new_node = priority_queue_init_node(hm_key, key, data);
    if (new_node == NULL) return PQ_OUT_OF_MEM;

    hash_map_insert(heap->map, hm_key, new_node);
    new_node->pos_in_pq = heap->size;
    heap->p_queue[new_node->pos_in_pq] = new_node;

    priority_queue_heapify_up(heap, new_node->pos_in_pq);

    return PQ_OK;
}

//--------------------------------------------------------------------------------

int* priority_queue_poll_first(void* bheap) {
    Bin_heap* heap = (Bin_heap*) bheap;

    if (heap->size == 0) return NULL;

    return heap->p_queue[1]->hm_key;
}

//--------------------------------------------------------------------------------

int priority_queue_delete_elem_by_hm_key(void* bheap, int* hm_key, void** data) {
    Bin_heap* heap = (Bin_heap*) bheap;

    PQ_node* node_to_delete;
    int check = hash_map_delete(heap->map, hm_key, &node_to_delete);

    if ((heap->size == 0) || (node_to_delete == NULL) || (check == MAP_MISSING)) return PQ_MISSING;

    int pos = node_to_delete->pos_in_pq;
    if (data != NULL) *data = node_to_delete->data;

    if (pos < heap->size) priority_queue_swap_nodes_positions_in_pq(&(heap->p_queue[pos]), &(heap->p_queue[heap->size]));

    priority_queue_free_node(node_to_delete, NULL);

    heap->p_queue[heap->size] = NULL;

    heap->size--;

    if ((heap->pq_cap > INIT_ELEMS_CAP) && (heap->size < heap->pq_cap / 4)) {
        heap->pq_cap -= INIT_ELEMS_CAP;

        heap->p_queue = (PQ_node**) realloc(heap->p_queue, heap->pq_cap * sizeof(PQ_node));
        if (heap->p_queue == NULL) return PQ_OUT_OF_MEM;
    }

    priority_queue_heapify_down(heap, pos);

    return PQ_OK;
}

//--------------------------------------------------------------------------------

int* priority_queue_extract_first(void* bheap, void** data) {
    Bin_heap* heap = (Bin_heap*) bheap;

    int* res = priority_queue_poll_first(bheap);

    int status = priority_queue_delete_elem_by_hm_key(heap, heap->p_queue[1]->hm_key, data);
    if (status != PQ_OK) return NULL;

    return res;
}

//--------------------------------------------------------------------------------

int priority_queue_decrease_key(void* bheap, int* hm_key, int new_key) {
    Bin_heap* heap = (Bin_heap*) bheap;

    PQ_node* this_node;
    int check = hash_map_find(heap->map, hm_key, &this_node);

    if ((heap->size == 0) || (this_node == NULL) || (check == MAP_MISSING)) return PQ_MISSING;

    if (this_node->key == new_key) return PQ_OK;

    this_node->key = new_key;

    if (this_node->pos_in_pq < heap->size) {
        int pos = this_node->pos_in_pq;

        priority_queue_swap_nodes_positions_in_pq(&(heap->p_queue[pos]), &(heap->p_queue[heap->size]));

        priority_queue_heapify_down(heap, pos);
    }

    priority_queue_heapify_up(heap, this_node->pos_in_pq);

    return PQ_OK;
}

//--------------------------------------------------------------------------------

int priority_queue_get_key_by_hm_key(void* bheap, int* hm_key, int* key) {
    Bin_heap* heap = (Bin_heap*) bheap;

    PQ_node* this_node;
    int check = hash_map_find(heap->map, hm_key, &this_node);

    if ((heap->size == 0) || (this_node == NULL) || (check == MAP_MISSING)) return PQ_MISSING;

    *key = this_node->key;

    return PQ_OK;
}

//--------------------------------------------------------------------------------

int priority_queue_get_pos_in_pq_by_hm_key(void* bheap, int* hm_key, int* pos) {
    Bin_heap* heap = (Bin_heap*) bheap;

    PQ_node* this_node;
    int check = hash_map_find(heap->map, hm_key, &this_node);

    if ((heap->size == 0) || (this_node == NULL) || (check == MAP_MISSING)) return PQ_MISSING;

    *pos = this_node->pos_in_pq;

    return PQ_OK;
}

//--------------------------------------------------------------------------------

int priority_queue_get_size(void* bheap) {
    Bin_heap* heap = (Bin_heap*) bheap;

    return heap->size;
}
//--------------------------------------------------------------------------------

void priority_queue_print_pq(void* bheap) {
    Bin_heap* heap = (Bin_heap*) bheap;

    for (int i = 1; i <= heap->size; i++) {
        queue_print_int(heap->p_queue[i]->hm_key);

        putchar('\n');
    }

    putchar('\n');
}

//--------------------------------------------------------------------------------

void priority_queue_free_pq(void* bheap, void (*free_data)(void*)) {
    Bin_heap* heap = (Bin_heap*) bheap;

    for (int i = 1; i <= heap->size; i++) priority_queue_free_node(heap->p_queue[i], free_data);

    free(heap->p_queue);
    free(heap->map);
    free(heap);
}