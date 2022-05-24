
#include <stdio.h>
#include <stdlib.h>
#include "Union_Find.h"
#include "Linked_List.h"

#define INIT_ELEMS_CAP 40

//------------------------------------------

typedef struct {
	int key;
	void* data;
	void* my_set; //sets are implemented as linked lists
} UF_node;

typedef struct {
	int sets_count;
	int elems_count;
	int elems_cap;
	UF_node** elements;
} UF_multi_set;

//------------------------------------------

UF_node* uf_init_node(void* data) {
	UF_node* new_node = (UF_node*) malloc(sizeof(UF_node));
	if (new_node == NULL) return NULL;

	new_node->key = -1;
	new_node->data = data;
	new_node->my_set = NULL;

	return new_node;
}

//------------------------------------------

void* uf_init_multi_set() {
	UF_multi_set* m_set = (UF_multi_set*) malloc(sizeof(UF_multi_set));
	if (m_set == NULL) return NULL;

	m_set->sets_count = 0;
	m_set->elems_count = 0;
	m_set->elems_cap = INIT_ELEMS_CAP;

	m_set->elements = (UF_node**) calloc(INIT_ELEMS_CAP, sizeof(UF_node*));
	if (m_set->elements == NULL) {
		free(m_set);

		return NULL;
	}

	return (void*) m_set;
}

//------------------------------------------

int uf_make_set(void* multi_set, void* data) {
	UF_multi_set* m_set = (UF_multi_set*) multi_set;

	UF_node* new_node = uf_init_node(data);
	if (new_node == NULL) return UF_OUT_OF_MEM;

	new_node->key = m_set->elems_count;
	m_set->elems_count++;

	if (m_set->elems_count > m_set->elems_cap) {
		m_set->elems_cap += INIT_ELEMS_CAP;

		m_set->elements = (UF_node**) realloc(m_set->elements, m_set->elems_cap * sizeof(UF_node*));
		if (m_set->elements == NULL) {
			free(new_node);
			
			return UF_OUT_OF_MEM;
		}
	}

	m_set->elements[m_set->elems_count -1] = new_node;

	new_node->my_set = linked_list_init_list(0);

	if (new_node->my_set == NULL) return UF_OUT_OF_MEM;

	m_set->sets_count++;

	int status = linked_list_insert_or_update_item_by_key(new_node->my_set, new_node->key, NULL);
	if (status != LL_OK) return status;

	return m_set->elements[m_set->elems_count - 1]->key;
}

//------------------------------------------

int uf_find(void* multi_set, int key) {
	UF_multi_set* m_set = (UF_multi_set*)multi_set;

	return linked_list_get_head_key(m_set->elements[key]->my_set);
}

//------------------------------------------

int uf_link(void* multi_set, int key1, int key2) {
	UF_multi_set* m_set = (UF_multi_set*)multi_set;

	int rep1_key = uf_find(multi_set, key1);
	int rep2_key = uf_find(multi_set, key2);

	if (rep1_key == rep2_key) return UF_OK;

	UF_node* rep1 = m_set->elements[rep1_key];
	UF_node* rep2 = m_set->elements[rep2_key];

	int rep1_set_size = linked_list_get_size(rep1->my_set);
	int rep2_set_size = linked_list_get_size(rep2->my_set);

	rep1 = (rep1_set_size >= rep2_set_size ? m_set->elements[rep1_key] : m_set->elements[rep2_key]);
	rep2 = (rep1_set_size >= rep2_set_size ? m_set->elements[rep2_key] : m_set->elements[rep1_key]);

	void* rep1_set = rep1->my_set;
	void* rep2_set = rep2->my_set;

	void* ll2_iter = linked_list_init_iterator(rep2_set);
	if (ll2_iter == NULL) return UF_OUT_OF_MEM;

	int curr_member_key;
	void* tmp;

	while (linked_list_iterator_has_next(ll2_iter) == 1) {
		curr_member_key = linked_list_iterator_next(ll2_iter, &tmp);
		m_set->elements[curr_member_key]->my_set = rep1_set;
	}
	linked_list_free_iterator(ll2_iter);

	linked_list_concatenate_unordered_lls(rep1_set, rep2_set);

	m_set->sets_count--;

	return UF_OK;
}

//------------------------------------------

int uf_get_sets_count(void* multi_set) {
	UF_multi_set* m_set = (UF_multi_set*)multi_set;

	return m_set->sets_count;
}

//------------------------------------------

void uf_free_multi_set(void* multi_set, void (*free_data)(void*)) {
	UF_multi_set* m_set = (UF_multi_set*)multi_set;

	int* freed = (int*) calloc(m_set->elems_count, sizeof(int));
	if (freed == NULL) return;

	UF_node* curr_node;
	UF_node* curr_member;
	int curr_member_key;

	void* tmp;
	void* ll_iter;
	void* curr_set;

	for (int i = 0; i < m_set->elems_count; i++) {
		if ((freed[i] == 1) || (i != uf_find(multi_set, i))) continue; //if (freed[i] != 0): its set hasnt been freed yet: find is safe

		curr_node = m_set->elements[i];
		curr_set = curr_node->my_set;

		ll_iter = linked_list_init_iterator(curr_set);
		if (ll_iter == NULL) return;

		while (linked_list_iterator_has_next(ll_iter) == 1) {
			curr_member_key = linked_list_iterator_next(ll_iter, &tmp);
			curr_member = m_set->elements[curr_member_key];

			if (free_data != NULL) free_data(curr_member->data);

			free(curr_member);

			m_set->elements[curr_member_key] = NULL;
			freed[curr_member_key] = 1;
		}
		linked_list_free_iterator(ll_iter);

		linked_list_free_list(curr_set, NULL);
	}

	free(m_set->elements);
	free(m_set);
	free(freed);
}

//------------------------------------------

void uf_print_multi_set(void* multi_set) {
	UF_multi_set* m_set = (UF_multi_set*)multi_set;

	int set_count = 0;

	printf("~~~~~~~~~~~~\nsets:\n");
	
	for (int i = 0; (i < m_set->elems_count) && (set_count < m_set->sets_count); i++) {
		if (i != uf_find(multi_set, i)) continue;

		printf("--------\n%d'th set: ", set_count);

		linked_list_print_list(m_set->elements[i]->my_set, NULL);

		set_count++;
	}
}

//------------------------------------------