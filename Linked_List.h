
#ifndef __LINKEDLIST_H__
#define __LINKEDLIST_H__

#define LL_OK			 0 	// exit success
#define LL_OUT_OF_MEM	-1 	// malloc fail
#define LL_MISSING		-2	// list does not contain this item

/*
* keys are ints (int doubly_linked_list, keys are void* ..)
*/


void* linked_list_init_list(int preserve_key_order);

int linked_list_get_size(void* llist);

int linked_list_find_item_by_key(void* llist, int key, void** item);

int linked_list_insert_or_update_item_by_key(void* llist, int key, void* item);

int linked_list_delete_item_by_key(void* llist, int key, void (*free_elem)(void*));

void linked_list_free_list(void* llist, void (*free_elem)(void*));

void linked_list_print_list(void* llist, void (*print_elem)(void*));

//~~~~~~~~~~~~~~~~~~~~

void* linked_list_init_iterator(void* llist);

int linked_list_iterator_has_next(void* lliter);

int linked_list_iterator_next(void* lliter, void** item); //returns key

void linked_list_free_iterator(void* lliter);

//~~~~~~~~~~~~~~~~~~~~

int linked_list_get_head_key(void* llist);

//frees addedLL when done. doesn't preserve order. destList must have atleast 1 elem..
void linked_list_concatenate_unordered_lls(void* dest_ll, void* added_ll);

void* linked_list_hard_copy_list(void* src, size_t elem_size);


#endif __LINKEDLIST_H__
