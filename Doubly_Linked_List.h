
#ifndef __DOUBLY_LINKED_LIST_H__
#define __DOUBLY_LINKED_LIST_H__

#define DLL_OK			 0 	// exit success
#define DLL_OUT_OF_MEM	-1 	// malloc fail
#define DLL_MISSING		-2	// hashmap does not contain this item
#define DLL_UNFIT_ARGS	-3	// need fitting arguaments


void* doubly_ll_init_doubly_ll(int (*compare_func)(const void*, const void*));

void doubly_ll_add_elem_by_pos(void* dll_list, int pos, void* key, void* elem);

void* doubly_ll_get_elem_by_pos(void* dll_list, int pos, void** elem);

void* doubly_ll_remove_elem_by_pos(void* dll_list, int pos, void** elem);

void doubly_ll_free_doubly_ll(void* dll_list, void (*free_key)(void*), void (*free_value)(void*));

void doubly_ll_print_doubly_ll(void* dll_list, void (*print_key)(void*), void (*print_value)(void*));

int doubly_ll_get_size(void* dll_list);

int doubly_ll_find_elem_by_key(void* dll_list, void* key, void** value);

void doubly_ll_insert_elem(void* dll_list, void* key, void* value);

int doubly_ll_delete_elem_by_key(void* dll_list, void* key, void** value);





#endif __DOUBLY_LINKED_LIST_H__