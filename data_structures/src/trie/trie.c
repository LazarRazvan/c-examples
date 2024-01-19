/**
 * Trie data structure implementation.
 * Copyright (C) 2024 Lazar Razvan.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>

#include "trie/trie.h"


/********************************** HELPERS **********************************/

/**
 * Recursivity logic to print all leafs starting by a given node.
 *
 * @node: Trie starting node.
 */
static void
__trie_recursive_print_leafs(trie_node_t *node, int level, char *word)
{
	// print word for leafs
	if (node->is_leaf)
		printf("%.*s\n", level, word);

	for (int i = 0; i < CHILDREN_NO; i++) {
		if (node->children[i]) {
			word[level] = i + 'a';
			__trie_recursive_print_leafs(node->children[i], level + 1, word);
		}
	}
}

/**
 * Recursivity logic to create list of all leafs starting by a given node.
 *
 * @node: Trie starting node.
 */
static int
__trie_recursive_get_leafs(trie_node_t *node, int level, char *word,
							kslist_head_t *list)
{
	word_entry_t *entry;

	// create and add entries for leafs
	if (node->is_leaf) {
		word[level] = '\0';

		entry = trie_word_entry_create(word);
		if (!entry)
			return -1;

		kslist_push_tail(list, &entry->node);
	}

	for (int i = 0; i < CHILDREN_NO; i++) {
		if (node->children[i]) {
			word[level] = i + 'a';
			if (__trie_recursive_get_leafs(node->children[i], level + 1, word,
											list))
				return -1;
		}
	}

	return 0;
}

/**
 * Recursivity logic to print all leafs based on a regex.
 *
 * @node: Trie starting node.
 */
static int
__trie_recursive_get_regex(trie_node_t *node, trie_regex_t *regex,
							kslist_head_t *list, int level, char *word)
{
	word_entry_t *entry;
	letter_regex_t *letter_regex;


	// stop if word length is exceeded (print if a leaf is found)
	if (level == regex->word_len) {
		if (node->is_leaf) {
			word[level] = '\0';

			entry = trie_word_entry_create(word);
			if (!entry)
				return -1;

			kslist_push_tail(list, &entry->node);
		}

		return 0;
	}

	// get regex for current level
	letter_regex = &regex->word_list[level];

	// iterate in node children and match the regex
	for (int i = 0; i < CHILDREN_NO; i++) {
		if (!node->children[i])
			continue;

		// regex match
		switch (letter_regex->opt) {
		case VAL_IN_LIST:
			if (letter_regex->mask & (1 << i)) {
				word[level] = i + 'a';
				if (__trie_recursive_get_regex(node->children[i], regex, list,
											level + 1, word))
					return -1;
			}

			break;
		case VAL_NOT_IN_LIST:
			if (!(letter_regex->mask & (1 << i))) {
				word[level] = i + 'a';
				if (__trie_recursive_get_regex(node->children[i], regex, list,
											level + 1, word))
					return -1;
			}

			break;
		default:
			assert(0);	// invalid option
		}
	}

	return 0;
}

/**
 * Recursivity logic to get all leafs based on a regex.
 *
 * @node: Trie starting node.
 */
static void
__trie_recursive_print_regex(trie_node_t *node, trie_regex_t *regex, int level,
							char *word)
{
	letter_regex_t *letter_regex;


	// stop if word length is exceeded (print if a leaf is found)
	if (level == regex->word_len) {
		if (node->is_leaf)
			printf("%.*s\n", level, word);

		return;
	}

	// get regex for current level
	letter_regex = &regex->word_list[level];

	// iterate in node children and match the regex
	for (int i = 0; i < CHILDREN_NO; i++) {
		if (!node->children[i])
			continue;

		// regex match
		switch (letter_regex->opt) {
		case VAL_IN_LIST:
			if (letter_regex->mask & (1 << i)) {
				word[level] = i + 'a';
				__trie_recursive_print_regex(node->children[i], regex,
											level + 1, word);
			}

			break;
		case VAL_NOT_IN_LIST:
			if (!(letter_regex->mask & (1 << i))) {
				word[level] = i + 'a';
				__trie_recursive_print_regex(node->children[i], regex,
											level + 1, word);
			}

			break;
		default:
			assert(0);	// invalid option
		}
	}
}


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
 * Create a word entry for words list.
 *
 * @word: String to be saved in word entry (NULL terminated).
 *
 * Return new allocated entry on success and NULL otherwise.
 */
word_entry_t *trie_word_entry_create(char *word)
{
	word_entry_t *entry;

	//
	entry = (word_entry_t *)malloc(sizeof(word_entry_t));
	if (!entry)
		goto error;

	//
	entry->word = strdup(word);
	if (!entry->word)
		goto free_entry;

	return entry;

free_entry:
	free(entry);

error:
	return NULL;
}


/**
 * Destroy a word entry for words list.
 *
 * @entry: Entry to be destroyd.
 */
void trie_word_entry_destroy(word_entry_t *entry)
{

	free(entry->word);
	free(entry);
}


/*****************************************************************************/

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


/*****************************************************************************/

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


/*****************************************************************************/

/**
 * Create all words (leafs) list in a trie by a given prefix.
 *
 * @root: Trie root
 * @str: Lookup prefix.
 * @list: List of words filled in by function.
 *
 * Return 0 on success (and fill in the list) or <0 on error.
 */
int trie_get_leafs_by_prefix(trie_node_t *root, char *str, kslist_head_t *list)
{
	int i = 0;
	uint8_t ch;
	trie_node_t *tmp;
	word_entry_t *entry;
	char word[MAX_WORD_LEN];
	kslist_node_t *it, *aux;

	if (!root)
		goto error;

	// if string is null, recursive print all leafs in trie
	if (!str)
		goto recursive_lookup;

	// move to last node in trie by the prefix
	tmp = root;
	for (i = 0; i < strlen(str); i++) {
		word[i] = str[i];
		ch = str[i] - 'a';

		//
		if (!tmp->children[ch])
			goto error;

		tmp = tmp->children[ch];
	}


recursive_lookup:
	// recursive get all words after prefix
	if (__trie_recursive_get_leafs(tmp, i, word, list))
		goto destroy_list;

	return 0;

destroy_list:
	kslist_for_each_safe(aux, it, list) {
		entry = container_of(aux, word_entry_t, node);
		trie_word_entry_destroy(entry);
	}

error:
	return -1;
}


/**
 * Create all words (leafs) list in a trie by a given regex.
 *
 * @root: Trie root
 * @regex: Lookup regex.
 * @list: List of words filled in by function.
 *
 * Return 0 on success (and fill in the list) or <0 on error.
 */
int trie_get_leafs_by_regex(trie_node_t *root, trie_regex_t *regex,
							kslist_head_t *list)
{
	word_entry_t *entry;
	kslist_node_t *it, *aux;
	char word[MAX_WORD_LEN];

	if (!root || !regex || !regex->word_len)
		return -1;

	// recursive get all words after prefix
	if (__trie_recursive_get_regex(root, regex, list, 0, word))
		goto destroy_list;

	return 0;

destroy_list:
	kslist_for_each_safe(aux, it, list) {
		entry = container_of(aux, word_entry_t, node);
		trie_word_entry_destroy(entry);
	}

	return -1;
}


/*****************************************************************************/

/**
 * Print all words (leafs) in a trie by a given prefix.
 *
 * @root: Trie root
 * @str: Lookup prefix.
 *
 * Print all words(leafs) in a trie that start with a given prefix.
 */
void trie_print_leafs_by_prefix(trie_node_t *root, char *str)
{
	int i = 0;
	uint8_t ch;
	trie_node_t *tmp;
	char word[MAX_WORD_LEN];

	if (!root)
		return;

	// if string is null, recursive print all leafs in trie
	if (!str)
		goto recursive_lookup;

	// move to last node in trie by the prefix
	tmp = root;
	for (i = 0; i < strlen(str); i++) {
		word[i] = str[i];
		ch = str[i] - 'a';

		//
		if (!tmp->children[ch])
			return;

		tmp = tmp->children[ch];
	}

recursive_lookup:
	// recursive get all words after prefix
	__trie_recursive_print_leafs(tmp, i, word);
}

/**
 * Print all words (leafs) in a trie by a given regex.
 *
 * @root: Trie root.
 * @regex: Lookup regex.
 *
 * Print all words(leafs) in a trie that match a given regex.
 */
void trie_print_leafs_by_regex(trie_node_t *root, trie_regex_t *regex)
{
	char word[MAX_WORD_LEN];

	if (!root || !regex || !regex->word_len)
		return;

	// recursive get all words after prefix
	__trie_recursive_print_regex(root, regex, 0, word);
}


/*****************************************************************************/

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


/*****************************************************************************/

/**
 * Convert a letter list to letter regex mask.
 *
 * @str: String letter list.
 *
 * Return a mask by enabling letter corresponding bit.
 */
uint32_t trie_letter_list_to_mask(char *str)
{
	uint32_t mask = 0;

	if (!str)
		return mask;

	for (int i = 0; i < strlen(str); i++)
		mask |= (1 << (str[i] - 'a'));

	return mask;
}


/*****************************************************************************/

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
