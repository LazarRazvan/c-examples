/**
 * Trie data structure implementation.
 * Copyright (C) 2024 Lazar Razvan.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "trie/trie.h"


/******************************** PUBLIC API *********************************/

/**
 * Create a node in a trie.
 *
 * Return new allocated node on success and false otherwise.
 */
trie_node_t *trie_node_create(void)
{
	trie_node_t *node = NULL;

	// alloc
	node = (trie_node_t *)malloc(1 * sizeof(trie_node_t));
	if (!node)
		return NULL;

	// initialize
	for (int i = 0; i < CHILDREN_NO; i++)
		node->children[i] = NULL;

	node->is_leaf = false;

	return node;
}

/**
 * Destroy a node in a trie.
 *
 * @node: Node to be destroyed.
 *
 * Note that this function destroy the entire path under the node.
 */
void trie_node_destroy(trie_node_t *node)
{

	for (int i = 0; i < CHILDREN_NO; i++) {
		if (node->children[i])
			trie_node_destroy(node->children[i]);
	}

	free(node);
}

/**
 * Insert an entry in a trie.
 *
 * @root: Trie root.
 * @str: Path to be inserted in the trie.
 *
 * Return trie root on success and NULL otherwise.
 */
trie_node_t *trie_insert(trie_node_t *root, char *str)
{
	uint8_t ch;
	trie_node_t *tmp;

	if (!root || !str)
		return NULL;

	tmp = root;
	// iterate letters in string
	for (int i = 0; i < strlen(str); i++) {
		ch = str[i] - 'a';

		// if path not found in trie, create it
		if (!tmp->children[ch]) {
			//
			tmp->children[ch] = trie_node_create();
			if (!tmp->children[ch])
				return NULL;
		}

		tmp = tmp->children[ch];
	}

	// mark leaf node
	tmp->is_leaf = true;

	return root;
}

/**
 * Remove an entry from a trie.
 *
 * @root: Trie root.
 * @str: Path to be removed from the trie.
 *
 * Return trie root on success and NULL on error.
 */
trie_node_t *trie_delete(trie_node_t *root, char *str)
{
	int prefix;
	uint8_t ch;
	trie_node_t *tmp;

	if (!root || !str)
		return NULL;

	// find longest prefix in trie
	prefix = trie_longest_prefix(root, str);
	if (prefix < 0)
		return NULL;	// path not found in trie

	if (prefix == 0) {
		// single path in trie
		ch = str[0] - 'a';
		assert(root->children[ch]);

		trie_node_destroy(root->children[ch]);

		return root;
	}

	// reaching here to compute branching
	// iterate in trie and remove str[prefix] entry from str[prefix-1] parent
	assert(prefix-1 < strlen(str));

	tmp = root;
	// iterate letters in string
	for (int i = 0; i < prefix - 1; i++) {
		ch = str[i] - 'a';
		//
		assert(tmp->children[ch]);
		tmp = tmp->children[ch];
	}

	// delete node
	ch = str[prefix-1] - 'a';
	assert(tmp->children[ch]);

	trie_node_destroy(tmp->children[ch]);
	tmp->children[ch] = NULL;

	return root;
}

/**
 * Search longest prefix for a given string in a trie.
 *
 * @root: Trie root.
 * @str: Path to be searched in the trie.
 *
 * Longest prefix represent the longest prefix in the string until the first
 * trie branching (new words) is found.
 *
 * Return value:
 * * -1: if string not found in trie
 * * 0 : if it is the only path in trie
 * * >0: if a branching occurs.
 */
int trie_longest_prefix(trie_node_t *root, char *str)
{
	uint8_t ch, prefix = 0;
	trie_node_t *tmp;

	if (!root || !str)
		goto err;

	tmp = root;
	for (int i = 0; i < strlen(str); i++) {
		ch = str[i] - 'a';

		// check if letter in path is in trie
		if (!tmp->children[ch])
			goto path_not_found;

		// if found, check if any branching occurs
		for (int j = 0; j < CHILDREN_NO; j++) {
			if (j != ch && tmp->children[j]) {
				prefix = i + 1;
				break;
			}
		}

		// increase prefix and move further
		tmp = tmp->children[ch];
	}

	return prefix;

path_not_found:
err:
	return -1;
}


/**
 * Search if a given string is found in a trie.
 *
 * @root: Trie root.
 * @str: Path to be searched in the trie.
 *
 * Return true if string is found in root and false otherwise.
 */
bool trie_search(trie_node_t *root, char *str)
{
	uint8_t ch;
	trie_node_t *tmp;

	if (!root || !str)
		return false;

	tmp = root;
	// iterate letters in string
	for (int i = 0; i < strlen(str); i++) {
		ch = str[i] - 'a';

		tmp = tmp->children[ch];
		if (!tmp)
			return false;
	}

	return tmp->is_leaf;
}

/**
 * Print a trie data structure.
 *
 * @root: Trie root.
 */
void trie_print(trie_node_t *root)
{
	trie_node_t *tmp;

	if (!root)
		return;

	tmp = root;
	for (int i = 0; i < CHILDREN_NO; i++) {
		if (tmp->children[i]) {
			printf("%c->", i + 'a');
			trie_print(tmp->children[i]);
		}
	}
}
