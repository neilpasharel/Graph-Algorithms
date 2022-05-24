
#include <stdio.h>
#include <stdlib.h>
#include "Doubly_Linked_List.h"

typedef struct DLL_node {
    void* key;
    void* data;
    struct DLL_node* prev;
    struct DLL_node* next;
} DLL_node;


typedef struct {
    int size;
    DLL_node* head;
    DLL_node* tail;
    int (*compare_func)(const void*, const void*);
} Doubly_linked_list;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void* doubly_ll_init_doubly_ll(int (*compare_func)(const void*, const void*)) {
    Doubly_linked_list* new_list = (Doubly_linked_list*) malloc(sizeof(Doubly_linked_list));
    if (new_list == NULL) return NULL;

    new_list->size = 0;
    new_list->head = NULL;
    new_list->tail = NULL;
    new_list->compare_func = compare_func;

    return (void*) new_list;
}

//---------------------------------------------------

DLL_node* doubly_ll_init_node(void* key, void* data) {
    DLL_node* node = (DLL_node*) malloc(sizeof(DLL_node));
    if (node == NULL) return NULL;

    node->key = key;
    node->data = data;
    node->prev = NULL;
    node->next = NULL;

    return node;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

DLL_node* doubly_ll_find_node_by_pos(Doubly_linked_list* list, int pos) {
    if (pos > list->size || pos < 0) return NULL;

    DLL_node* curr_node = NULL;
    int curr_pos, in_reverse;

    int middle = ((list->size - 1) / 2);
    in_reverse = (pos > middle ? 1 : 0);
    curr_pos = (pos > middle ? list->size - 1 : 0);
    curr_node = (pos > middle ? list->tail : list->head);

    while ((curr_node != NULL) && (curr_pos != pos)) {
        curr_node = (in_reverse ? curr_node->prev : curr_node->next);
        curr_pos += (in_reverse ? -1 : 1);
    }
    return curr_node;
}

//---------------------------------------------------

void doubly_ll_add_elem_by_pos(void* dll_list, int pos, void* key, void* elem) {
    Doubly_linked_list* list = (Doubly_linked_list*) dll_list;

    DLL_node* new_node = NULL;
    DLL_node* curr_node = NULL;

    new_node = doubly_ll_init_node(key, elem);

    if (list->size == 0) {  /*first insert*/
        list->head = new_node;
        list->tail = new_node;
    }
    else {
        curr_node = doubly_ll_find_node_by_pos(list, pos);   /* find and insert */

        if (curr_node != NULL) {
            new_node->prev = curr_node->prev;
            new_node->next = curr_node;

            if (curr_node->prev == NULL) list->head = new_node;
            else curr_node->prev->next = new_node;

            curr_node->prev = new_node;
        }
        else { //adding to end
            list->tail->next = new_node;
            new_node->prev = list->tail;
            list->tail = new_node;
        }
    }

    list->size++;
}

//---------------------------------------------------

void* doubly_ll_get_elem_by_pos(void* dll_list, int pos, void** elem) {
    Doubly_linked_list* list = (Doubly_linked_list*) dll_list;

    DLL_node* node = doubly_ll_find_node_by_pos(list, pos);

    if (elem != NULL) *elem = node->data;

    return node->key;
}

//---------------------------------------------------

void* doubly_ll_remove_elem_by_pos(void* dll_list, int pos, void** elem) {
    Doubly_linked_list* list = (Doubly_linked_list*) dll_list;

    DLL_node* curr_node = doubly_ll_find_node_by_pos(list, pos);
    if (curr_node == NULL) return -1;

    if (curr_node->prev == NULL) list->head = curr_node->next;
    else curr_node->prev->next = curr_node->next;

    if (curr_node->next == NULL) list->tail = curr_node->prev;
    else curr_node->next->prev = curr_node->prev;

    list->size--;

    if (elem != NULL) *elem = curr_node->data;

    void* res = curr_node->key;

    free(curr_node);

    return res;
}

//---------------------------------------------------

void doubly_ll_free_doubly_ll(void* dll_list, void (*free_key)(void*), void (*free_value)(void*)) {
    Doubly_linked_list* list = (Doubly_linked_list*) dll_list;

    DLL_node* curr_node = list->head;
    DLL_node* next_node = NULL;

    while (curr_node != NULL) {
        next_node = curr_node->next;

        if (free_key) free_key(curr_node->key);
        if (free_value) free_value(curr_node->data);

        free(curr_node);

        curr_node = next_node;
    }

    free(list);
}

//---------------------------------------------------

void doubly_ll_print_doubly_ll(void* dll_list, void (*print_key)(void*), void (*print_value)(void*)) {
    Doubly_linked_list* list = (Doubly_linked_list*) dll_list;

    int pos = 0;
    DLL_node* curr_node = list->head;

    while (curr_node != NULL) {
        printf("( ");

        if (print_key) print_key(curr_node->key);
        else printf("%d", curr_node->key);

        putchar(' ');

        if ((curr_node != NULL) && (curr_node->data != NULL)) {
            printf(", ");
            if (print_value) print_value(curr_node->data);
            else printf("%d", curr_node->data);

            putchar(' ');

            pos++;
        }

        printf(") -> ");

        curr_node = curr_node->next;
    }

    putchar('\n');
}

//---------------------------------------------------

int doubly_ll_get_size(void* dll_list) {
    Doubly_linked_list* list = (Doubly_linked_list*)dll_list;

    return list->size;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

DLL_node* doubly_ll_find_node_by_key(Doubly_linked_list* list, void* key) {
    if (list->size == 0) return NULL;

    DLL_node* curr_node = list->head;

    while (curr_node != NULL) {
        if (list->compare_func(curr_node->key, key) == 0) return curr_node;

        curr_node = curr_node->next;
    }
    return curr_node;
}

//----------------------------------------------------------

int doubly_ll_find_elem_by_key(void* dll_list, void* key, void** value) {
    Doubly_linked_list* list = (Doubly_linked_list*) dll_list;

    DLL_node* node = doubly_ll_find_node_by_key(list, key);
    if (node == NULL) return DLL_MISSING;

    if (value != NULL) *value = node->data;

    return DLL_OK;
}

//----------------------------------------------------------

void doubly_ll_insert_elem(void* dll_list, void* key, void* value) {
    doubly_ll_add_elem_by_pos(dll_list, doubly_ll_get_size(dll_list) -1, key, value);
}

//----------------------------------------------------------

int doubly_ll_delete_elem_by_key(void* dll_list, void* key, void** value) {
    Doubly_linked_list* list = (Doubly_linked_list*) dll_list;

    DLL_node* node = doubly_ll_find_node_by_key(list, key);
    if (node == NULL) return DLL_MISSING;

    if (node->prev == NULL) list->head = node->next;
    else node->prev->next = node->next;

    if (node->next == NULL) list->tail = node->prev;
    else node->next->prev = node->prev;

    if (value != NULL) *value = node->data;

    free(node);

    list->size--;

    return DLL_OK;
}




