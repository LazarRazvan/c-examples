/**
 * Trie data structure implementation.
 * Copyright (C) 2024 Lazar Razvan.
 */

#include <stdbool.h>
#include <stdint.h>


/********************************** DEFINES **********************************/

// Consider 26 english letters
#define CHILDREN_NO			26


/****************************** DATA STRUCTURE *******************************/

typedef struct trie_node {

	struct trie_node *children[CHILDREN_NO];
	bool is_leaf;

} trie_node_t;


/******************************** PUBLIC API *********************************/

//
trie_node_t *trie_node_create(void);
void trie_node_destroy(trie_node_t *node);

//
trie_node_t *trie_insert(trie_node_t *root, char *str);

//
trie_node_t *trie_delete(trie_node_t *root, char *str);

//
int trie_longest_prefix(trie_node_t *root, char *str);

//
bool trie_search(trie_node_t *root, char *str);

//
void trie_print(trie_node_t *root);
