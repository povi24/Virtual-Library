// Copyright 2021-2022 Povarna Andreea 314CA

#include "ll.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ht.h"

#define MAX_STRING_SIZE	256
#define HMAX 100

/*
 * Functii de comparare a cheilor:
 */
int
compare_function_ints(void *a, void *b)
{
	int int_a = *((int *)a);
	int int_b = *((int *)b);

	if (int_a == int_b) {
		return 0;
	} else if (int_a < int_b) {
		return -1;
	} else {
		return 1;
	}
}

int
compare_function_strings(void *a, void *b)
{
	char *str_a = (char *)a;
	char *str_b = (char *)b;

	return strcmp(str_a, str_b);
}


unsigned int
hash_function_int(void *a)
{
	unsigned int uint_a = *((unsigned int *)a);

	uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
	uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
	uint_a = (uint_a >> 16u) ^ uint_a;
	return uint_a;
}

unsigned int
hash_function_string(void *a)
{
	unsigned char *puchar_a = (unsigned char*) a;
	int hash = 5381;
	int c;

	while ((c = *puchar_a++))
		hash = ((hash << 5u) + hash) + c;

	return hash;
}


hashtable_t *
ht_create(unsigned int hmax, unsigned int (*hash_function)(void*),
		int (*compare_function)(void*, void*))
{
	hashtable_t *ht = malloc(sizeof(hashtable_t));
	ht->hash_function = hash_function;
	ht->compare_function = compare_function;
	ht->size = 0;
	ht->hmax = hmax;
	ht->buckets = malloc(hmax * sizeof(linked_list_t*));
	for(uint i = 0; i < hmax; i++)
		ht->buckets[i] = ll_create(sizeof(struct info));

	return ht;
}


int
ht_has_key(hashtable_t *ht, void *key)
{
	ll_node_t *current = ht->buckets[ht->hash_function(key) % ht->hmax]->head;

	while (current){
		if(ht->compare_function(((struct info *)(current->data))->key, key) == 0)
			return 1;
		current = current->next;
	}

	return 0;
}

void *
ht_get(hashtable_t *ht, void *key)
{
	ll_node_t *current = ht->buckets[ht->hash_function(key) % ht->hmax]->head;
	while (current != NULL) {
		if(ht->compare_function(((struct info *)current->data)->key, key) == 0)
			return ((struct info *)(current->data))->value;
		current = current->next;
	}

	return NULL;
}

void
ht_put(hashtable_t *ht, void *key, unsigned int key_size,
	void *value, unsigned int value_size)
{
	ll_node_t *hash = ht->buckets[ht->hash_function(key) % ht->hmax]->head;

	while(hash != NULL) {
		if(ht->compare_function(((struct info *)(hash->data))->key, key) == 0) {
			memcpy(((struct info *)(hash->data))->value, value, value_size);
			return;
		}
		hash = hash->next;
	}

	struct info new;
	new.key = malloc(key_size);
	new.value = malloc(value_size);
	memcpy(new.key, key, key_size);
	memcpy(new.value, value, value_size);

	ll_add_nth_node(ht->buckets[ht->hash_function(key) % ht->hmax],
    ht->buckets[ht->hash_function(key) % ht->hmax]->size, &new);

	ht->size++;
}


void
ht_remove_entry(hashtable_t *ht, void *key)
{
	ll_node_t *current = ht->buckets[ht->hash_function(key) % ht->hmax]->head;
	int i = 0;

	while(current != NULL){
		if(ht->compare_function(((struct info *)(current->data))->key,
                                                        key) == 0) {
			current = ll_remove_nth_node(ht->buckets[ht->hash_function(key)
                                                            % ht->hmax], i);
			free(((struct info *)current->data)->key);
			free(((struct info *)current->data)->value);
			free(current->data);
			free(current);
			ht->size--;
			return;
		}
		i++;
		current = current->next;
	}
}


void
ht_free(hashtable_t *ht)
{
    ll_node_t *current;

	for(uint i = 0; i < ht->hmax; i++) {
		current = ht->buckets[i]->head;
		while(current != NULL) {
			free(((struct info *)(current->data))->key);
			free(((struct info *)(current->data))->value);
			current = current->next;
		}
		ll_free(&ht->buckets[i]);
	}
	free(ht->buckets);
	free(ht);
}

unsigned int
ht_get_size(hashtable_t *ht)
{
	if (ht == NULL)
		return 0;

	return ht->size;
}

unsigned int
ht_get_hmax(hashtable_t *ht)

{
	if (ht == NULL)
		return 0;

	return ht->hmax;
}
