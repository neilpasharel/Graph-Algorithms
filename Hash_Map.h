
#ifndef __HASHMAP_H__
#define __HASHMAP_H__

#define MAP_OK			 0 	// exit success
#define MAP_OUT_OF_MEM	-1 	// malloc fail
#define MAP_MISSING		-2	// hashmap does not contain this item
#define MAP_UNFIT_ARGS	-3	// need fitting arguaments

/*
* hashMap needs pointer to function to compare keys.
* hashMap needs pointer to function to hash keys.
*
*/

void* hash_map_init_hash_map(int table_cap, size_t (*hash_func)(const void*), int (*compare_func)(const void*, const void*));

int hash_map_insert(void* hmap, void* key, void* item);

int hash_map_find(void* hmap, void* key, void** item);

int hash_map_delete(void* hmap, void* key, void** value);

int hash_map_get_size(void* hmap);

void hash_map_free_hash_map(void* hmap, void (*free_key)(void*), void (*free_value)(void*));

void hash_map_print_hash_map(void* hmap, void (*print_key)(void*), void (*print_value)(void*));

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/*
[suggestions in case keys are strings..]

size_t hash_func_for_string_keys(const void* key) {
    const char* kkey = (const char*) key;
    size_t hash = 0;

    for (; *kkey != '\0'; kkey++) {
        hash += *kkey;
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash;
}

int compare_strings(const void* a, const void* b) {
    const char* aa = (const char*) a;
    const char* bb = (const char*) b;

    return strcmp(aa, bb);
}

void print_string(void* key) {
    printf("%s", (char*) key);
}

*/

//---------------------------------------------------------------------

/*
[suggestions in case keys are ints..]

size_t hash_func_for_int_keys(const void* key) {
    size_t hash = *((int*) key);

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
    int aa = *((int*) a);
    int bb = *((int*) b);

    return aa - bb;
}

void print_int(void* key) {
    printf("%d", *((int*) key));
}

*/

//---------------------------------------------------------------------

/*
[suggestions in case keys are data..]

size_t hash_func_for_struct_leys(const void* data) {
    //need to feel in struct name
    size_t size_of_struct_instance = sizeof( ___ );

    const uint8_t* byte = (const uint8_t*) data;
    size_t hash = 0;

    for (size_t i = 0; i < size_of_struct_instance; ++i) {
        hash += *byte++;
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash;
}

*/


#endif __HASHMAP_H__
