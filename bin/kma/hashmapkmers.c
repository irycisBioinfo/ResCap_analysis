/* Philip T.L.C. Clausen Jan 2017 plan@dtu.dk */

/*
 * Copyright (c) 2017, Philip Clausen, Technical University of Denmark
 * All rights reserved.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *		http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

#include <stdlib.h>
#include "hashmapkmers.h"
#include "pherror.h"

void hashMap_kmers_initialize(HashMap_kmers *dest, unsigned newSize) {
	/* set hashMap */
	dest->size = newSize;
	dest->n = 0;
	/* set hashTable */
	dest->table = calloc(newSize, sizeof(HashTable_kmers*));
	if(!dest->table) {
		ERROR();
	}
}

void hashMap_kmers_CountIndex(HashMap_kmers *dest, long unsigned key) {
	
	unsigned index;
	HashTable_kmers *node;
	
	/* get index */
	index = key % dest->size;
	
	/* find pos */
	if(dest->table[index] == 0) { // New value, no collision
		++dest->n;
		dest->table[index] = smalloc(sizeof(HashTable_kmers));
		node = dest->table[index];
		node->value = 1;
		node->key = key;
		node->next = 0;
	} else {
		for(node = dest->table[index]; node != 0; node = node->next) {
			if(key == node->key) { // Keys match change value
				++node->value;
				return;
			} else if(node->next == 0) { // This chain is filled, create next
				++dest->n;
				node->next = smalloc(sizeof(HashTable_kmers));
				node = node->next;
				node->next = 0;
				node->key = key;
				node->value = 1;
				return;
			}
		}
	}
}

void reallocHashMap_kmers(HashMap_kmers *dest) {
	
	long unsigned index;
	HashTable_kmers *table, *node, *node_next;
	
	/* save buckets */
	table = 0;
	index = ++dest->size;
	while(index--) {
		for(node = dest->table[index]; node; node = node_next) {
			node_next = node->next;
			node->next = table;
			table = node;
		}
	}
	
	/* reallocate table */
	free(dest->table);
	dest->table = calloc(dest->size <<= 1, sizeof(HashTable_kmers *));
	if(!dest->table) {
		ERROR();
	}
	--dest->size;
	
	/* refill table */
	for(node = table; node; node = node_next) {
		node_next = node->next;
		index = node->key & dest->size;
		node->next = dest->table[index];
		dest->table[index] = node;
	}
}

int hashMap_CountKmer(HashMap_kmers *dest, long unsigned key) {
	
	long unsigned index;
	HashTable_kmers *node;
	
	if(dest->n == dest->size) {
		reallocHashMap_kmers(dest);
	}
	
	index = key & dest->size;
	if(dest->table[index] == 0) {
		dest->table[index] = smalloc(sizeof(HashTable_kmers));
		node = dest->table[index];
		node->key = key;
		node->next = 0;
		++dest->n;
		return 1;
	} else {
		for(node = dest->table[index]; node != 0; node = node->next) {
			if(node->key == key) {
				return 0;
			} else if(node->next == 0) {
				node->next = smalloc(sizeof(HashTable_kmers));
				node = node->next;
				node->key = key;
				node->next = 0;
				++dest->n;
				return 1;
			}
		}
	}
	return -1;
}

void emptyHash(HashMap_kmers *dest) {
	
	unsigned i;
	HashTable_kmers *node, *next;
	
	for(i = 0; i < dest->size; ++i) {
		for(node = dest->table[i]; node != 0; node = next) {
			next = node->next;
			free(node);
		}
		dest->table[i] = 0;
	}
	dest->n = 0;
}
