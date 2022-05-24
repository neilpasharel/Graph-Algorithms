
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Hash_Map.h"
#include "Doubly_Linked_List.h"


typedef struct {
	void** table;
	int table_cap;
	int size;  //elems_count
	size_t(*hash_func)(const void*);
	int (*compare_func)(const void*, const void*);
	//size_t elem_size;
} Hash_map;

//----------------------------------------------------------

void* hash_map_init_hash_map(int table_cap, size_t(*hash_func)(const void*), int (*compare_func)(const void*, const void*)) {
	assert((hash_func != NULL) && (compare_func != NULL));

	Hash_map* map = (Hash_map*) malloc(sizeof(Hash_map));
	if (map == NULL) return NULL;

	map->table = (void**) calloc(table_cap, sizeof(void*));
	if (map->table == NULL) {
		free(map);

		return NULL;
	}

	map->table_cap = table_cap;
	map->size = 0;
	map->hash_func = hash_func;
	map->compare_func = compare_func;

	return (void*) map;
}

//----------------------------------------------------------

void** hash_map_calc_table_entry_pos(Hash_map* map, void* key) {
	int index = map->hash_func(key) % map->table_cap;

	return (void**) (((char*) map->table) + (index * sizeof(void*)));
}

//----------------------------------------------------------

int hash_map_insert(void* hmap, void* key, void* item) {
	if (key == NULL) return MAP_UNFIT_ARGS;

	Hash_map* map = (Hash_map*) hmap;

	void** table_entry_pos = hash_map_calc_table_entry_pos(map, key);
	if (*table_entry_pos == NULL) {
		*table_entry_pos = doubly_ll_init_doubly_ll(map->compare_func);

		if (*table_entry_pos == NULL) return MAP_OUT_OF_MEM;
	}

	doubly_ll_insert_elem(*table_entry_pos, key, item);

	map->size++;

	return MAP_OK;
}

//----------------------------------------------------------

int hash_map_find(void* hmap, void* key, void** item) {
	if (key == NULL) return MAP_UNFIT_ARGS;

	Hash_map* map = (Hash_map*) hmap;

	void** table_entry_pos = hash_map_calc_table_entry_pos(map, key);
	if (*table_entry_pos == NULL) return MAP_MISSING;

	int status = doubly_ll_find_elem_by_key(*table_entry_pos, key, item);
	if (status == DLL_MISSING) return MAP_MISSING;

	return MAP_OK;
}

//----------------------------------------------------------

int hash_map_delete(void* hmap, void* key, void** value) {
	if (key == NULL) return MAP_UNFIT_ARGS;

	Hash_map* map = (Hash_map*) hmap;

	void** table_entry_pos = hash_map_calc_table_entry_pos(map, key);
	if (*table_entry_pos == NULL) return MAP_MISSING;

	int status = doubly_ll_delete_elem_by_key(*table_entry_pos, key, value);
	if (status == DLL_MISSING) return MAP_MISSING;

	if (doubly_ll_get_size(*table_entry_pos) == 0) {
		doubly_ll_free_doubly_ll(*table_entry_pos, NULL, NULL);

		*table_entry_pos = NULL;
	}

	map->size--;

	return MAP_OK;
}

//----------------------------------------------------------

int hash_map_get_size(void* hmap) {
	Hash_map* map = (Hash_map*) hmap;

	return map->size;
}

//----------------------------------------------------------

void hash_map_free_hash_map(void* hmap, void (*free_key)(void*), void (*free_value)(void*)) {
	Hash_map* map = (Hash_map*) hmap;

	for (int i = 0; i < map->table_cap; i++) {
		void** table_entry_pos = (void**) (((char*)map->table) + (i * sizeof(void*)));
		if (*table_entry_pos == NULL) continue;

		doubly_ll_free_doubly_ll(*table_entry_pos, free_key, free_value);
	}

	free(map->table);
	free(map);
}

//----------------------------------------------------------

void hash_map_print_hash_map(void* hmap, void (*print_key)(void*), void (*print_value)(void*)) {
	Hash_map* map = (Hash_map*)hmap;

	for (int i = 0; i < map->table_cap; i++) {
		printf("table entry %d : ", i);

		void** table_entry_pos = (void**) (((char*)map->table) + (i * sizeof(void*)));

		if (*table_entry_pos != NULL) doubly_ll_print_doubly_ll(*table_entry_pos, print_key, print_value);

		putchar('\n');
	}
}