
#ifndef __UNIONFIND_H__
#define __UNIONFIND_H__

#define UF_OK			 0 	// exit success
#define UF_OUT_OF_MEM	-1 	// malloc fail
#define UF_MISSING		-2	// graph does not contain this item


void* uf_init_multi_set();

int uf_make_set(void* multi_set, void* data); //adds data as new element in a set of its own, returns key or error code

int uf_find(void* multi_set, int key); //return representative's key

int uf_link(void* multi_set, int key1, int key2);

int uf_get_sets_count(void* multi_set);

void uf_print_multi_set(void* multi_set); //for each elem, function prints it's set. TODO: stop dup printing..

void uf_free_multi_set(void* multi_set, void (*free_data)(void*));


#endif __UNIONFIND_H__
