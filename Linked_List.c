
#include <stdio.h>
#include <stdlib.h>
#include "Linked_List.h"

//----------------------------------------------------------

typedef struct LL_node {
    int key;
    void* data;
    struct LL_node* next;
} LL_node;


typedef struct {
    int size;
    LL_node* head;
    LL_node* tail;
    int preserve_key_order;
} Linked_list;


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

LL_node* linked_list_init_node(int key, void* data) {
    LL_node* new_node = (LL_node*) malloc(sizeof(LL_node));
    if (new_node == NULL) return NULL;

    new_node->key = key;
    new_node->data = data;
    new_node->next = NULL;

    return new_node;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

LL_node* linked_list_find_node(Linked_list* list, int key) {
    if (list->size == 0) return NULL;

    LL_node* curr_node = list->head;

    while (curr_node != NULL) {
        if (curr_node->key == key) return curr_node;
        if ((list->preserve_key_order == 1) && (curr_node->key > key)) return NULL;

        curr_node = curr_node->next;
    }

    return curr_node;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

LL_node* linked_list_find_preceding_node(Linked_list* list, int key) {
    if ((list->size == 0) || ((list->preserve_key_order == 1) && (list->head->key >= key))) return NULL;

    LL_node* curr_node = list->head;

    while ((curr_node->next != NULL) && (curr_node->next->key != key)) {
        if ((list->preserve_key_order == 1) && (curr_node->next->key > key)) break;

        curr_node = curr_node->next;
    }

    return curr_node;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void linked_list_insert_node_first(Linked_list* list, LL_node* node_to_insert) {
    if (list->size == 0) list->tail = node_to_insert;

    node_to_insert->next = list->head;
    list->head = node_to_insert;
    list->size++;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void linked_list_insert_node_last(Linked_list* list, LL_node* node_to_insert) {
    if (list->size == 0) list->head = node_to_insert;
    else list->tail->next = node_to_insert;

    list->tail = node_to_insert;
    list->size++;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void linked_list_free_node(LL_node* node, void (*free_node_data)(void*)) {
    if (free_node_data != NULL) free_node_data(node->data);

    free(node);
}

//-----------------------------------------------------------------------------------------

void* linked_list_init_list(int preserve_key_order) {
    Linked_list* new_list = (Linked_list*) malloc(sizeof(Linked_list));
    if (new_list == NULL) return NULL;

    new_list->size = 0;
    new_list->head = NULL;
    new_list->tail = NULL;
    new_list->preserve_key_order = preserve_key_order;

    return (void*) new_list;
}

//----------------------------------------------------------
int linked_list_get_size(void* llist) {
    Linked_list* list = (Linked_list*) llist;

    return list->size;
}

//----------------------------------------------------------
int linked_list_find_item_by_key(void* llist, int key, void** item) {
    Linked_list* list = (Linked_list*) llist;

    LL_node* node = linked_list_find_node(list, key);
    if (node == NULL) return LL_MISSING;

    *item = node->data;

    return LL_OK;
}

//----------------------------------------------------------
int linked_list_insert_or_update_item_by_key(void* llist, int key, void* item) {
    Linked_list* list = (Linked_list*) llist;

    LL_node* node = linked_list_find_node(list, key); //if key already in LL, just update data
    if (node != NULL) {
        node->data = item;

        return LL_OK;
    }

    LL_node* new_node = linked_list_init_node(key, item); //insert new key to LL
    if (new_node == NULL) return LL_OUT_OF_MEM;

    //non order preserving insert
    if (list->preserve_key_order == 0) linked_list_insert_node_last(list, new_node);

    //order preserving insert
    else if ((list->size == 0) || (key < list->head->key)) linked_list_insert_node_first(list, new_node);
    else if (key > list->tail->key) linked_list_insert_node_last(list, new_node);
    else {
        LL_node* curr_node = linked_list_find_preceding_node(list, key);

        new_node->next = curr_node->next;
        curr_node->next = new_node;
        list->size++;
    }

    return LL_OK;
}

//----------------------------------------------------------
int linked_list_delete_item_by_key(void* llist, int key, void (*free_elem)(void*)) {
    Linked_list* list = (Linked_list*) llist;

    if (list->size == 0) return LL_MISSING;
    if (list->preserve_key_order == 1) {
        if ((key < list->head->key) || (list->tail->key < key)) return LL_MISSING;
    }

    LL_node* node_to_delete;

    LL_node* prec_node = linked_list_find_preceding_node(list, key);
    if (prec_node == NULL) {
        if (list->head->key == key) { //if its the head
            node_to_delete = list->head;
            list->head = list->head->next;

            if (list->size == 1) list->tail = NULL;
        }
        else return LL_MISSING;
    }
    else {
        node_to_delete = prec_node->next;
        prec_node->next = node_to_delete->next;

        if (list->tail->key == key) list->tail = prec_node;
    }

    linked_list_free_node(node_to_delete, free_elem);
    list->size--;

    return LL_OK;
}

//----------------------------------------------------------
void linked_list_free_list(void* llist, void (*free_elem)(void*)) {
    Linked_list* list = (Linked_list*) llist;

    LL_node* curr_node = list->head;
    LL_node* next_node = NULL;

    while (curr_node != NULL) {
        next_node = curr_node->next;

        linked_list_free_node(curr_node, free_elem);

        curr_node = next_node;
    }

    free(list);
}

//----------------------------------------------------------
void linked_list_print_list(void* llist, void (*print_elem)(void*)) {
    Linked_list* list = (Linked_list*) llist;

    LL_node* curr_node = list->head;

    while (curr_node != NULL) {
        printf(" -> ( %d ", curr_node->key);
        if (print_elem != NULL) {
            printf(": ");

            print_elem(curr_node->data);

            putchar(' ');
        }

        printf(")");

        curr_node = curr_node->next;
    }

    putchar('\n');
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

typedef struct {
    LL_node* curr_node;
} Linked_list_iterator;

void* linked_list_init_iterator(void* llist) {
    Linked_list* list = (Linked_list*) llist;

    Linked_list_iterator* iter = (Linked_list_iterator*) malloc(sizeof(Linked_list_iterator));
    if (iter == NULL) return NULL;

    //iter->curr_node = ((list->size == 0) ? NULL : list->head);
    iter->curr_node = list->head;

    return (void*) iter;
}

int linked_list_iterator_has_next(void* lliter) {
    Linked_list_iterator* iter = (Linked_list_iterator*) lliter;
    if (iter == NULL) return 0;

    return ((iter->curr_node != NULL) ? 1 : 0);
}

int linked_list_iterator_next(void* lliter, void** item) {
    Linked_list_iterator* iter = (Linked_list_iterator*) lliter;

    if (item != NULL) *item = iter->curr_node->data;
    int res = iter->curr_node->key;

    iter->curr_node = iter->curr_node->next;

    return res;
}

void linked_list_free_iterator(void* lliter) {
    Linked_list_iterator* iter = (Linked_list_iterator*) lliter;

    free(iter);
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int linked_list_get_head_key(void* llist) {
    Linked_list* list = (Linked_list*) llist;

    if (list->size == 0) return LL_MISSING;

    return list->head->key;
}

//----------------------------------------------------------

void linked_list_concatenate_unordered_lls(void* dest_ll, void* added_ll) {
    Linked_list* dest_list = (Linked_list*)dest_ll;
    Linked_list* added_list = (Linked_list*)added_ll;

    if ((dest_list == NULL) || (added_list == NULL)) return;

    if (added_list->size == 0) {
    }
    else if (dest_list->size == 0) {
        dest_list->head = added_list->head;
        dest_list->tail = added_list->tail;
    }
    else {
        dest_list->tail->next = added_list->head;
        dest_list->tail = added_list->tail;
    }
    
    dest_list->size += added_list->size;
    dest_list->preserve_key_order = 0;

    free(added_list);
}

//----------------------------------------------------------

void* linked_list_hard_copy_list(void* src, size_t elem_size) {
    Linked_list* source = (Linked_list*) src;

    Linked_list* res = linked_list_init_list(source->preserve_key_order);
    if (res == NULL) return NULL; //malloc fail

    if (source->size == 0) return res;

    void* data_copy = NULL;

    LL_node* prev_node_in_res = NULL;
    LL_node* new_node_for_res = NULL;
    LL_node* curr_node_in_src = source->head;

    while (curr_node_in_src != NULL) {
        data_copy = NULL;
        if (elem_size > 0) {
            data_copy = malloc(elem_size);
            if (data_copy == NULL) return NULL; //malloc fail

            memcpy(data_copy, curr_node_in_src->data, elem_size);
        }

        new_node_for_res = linked_list_init_node(curr_node_in_src->key, data_copy);
        if (new_node_for_res == NULL) return NULL; //malloc fail

        if (curr_node_in_src == source->head) res->head = new_node_for_res;
        else prev_node_in_res->next = new_node_for_res;
        prev_node_in_res = new_node_for_res;

        curr_node_in_src = curr_node_in_src->next;
    }

    res->size = source->size;
    res->tail = new_node_for_res;

    return (void*) res;
}