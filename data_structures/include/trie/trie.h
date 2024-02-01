/**
 * Trie data structure implementation.
 * Copyright (C) 2024 Lazar Razvan.
 */

#ifndef TRIE_H
#define TRIE_H


#include <stdbool.h>
#include <stdint.h>

#include "list/ksingly_linked_list.h"


/********************************** DEFINES **********************************/

// Consider 26 english letters
#define CHILDREN_NO			26

// Max trie depth (max word length)
#define MAX_WORD_LEN		128


/*********************************** TRIE ************************************/

// trie node
typedef struct trie_node {

	struct trie_node 	*children[CHILDREN_NO];
	bool				is_leaf;

} trie_node_t;


/****************************** WORD LIST ENTRY ******************************/

// trie node
typedef struct word_entry {

	char				*word;
	kslist_node_t		node;

} word_entry_t;


/********************************** REGEX ************************************/

// trie letter regex
typedef enum regex_opt {

	VAL_IN_LIST = 0,			// choose a value from list
	VAL_NOT_IN_LIST,			// choose a value that is not in list

} regex_opt_t;

typedef struct letter_regex {

	regex_opt_t			opt;	// current letter option
	uint32_t			mask;	// letter mask (1-set, 0-not set)

} letter_regex_t;

// trie word regex
typedef struct trie_regex {

	uint8_t				word_len;
	letter_regex_t		word_list[MAX_WORD_LEN];

} trie_regex_t;


/******************************** PUBLIC API *********************************/

//
trie_node_t *trie_node_create(void);
void trie_node_destroy(trie_node_t *node);

//
word_entry_t *trie_word_entry_create(char *word);
void trie_word_entry_destroy(word_entry_t *entry);

//
trie_node_t *trie_insert(trie_node_t *root, char *str);
trie_node_t *trie_delete(trie_node_t *root, char *str);

//
int trie_longest_prefix(trie_node_t *root, char *str);

//
int trie_get_leafs_by_prefix(trie_node_t *root, char *str, kslist_head_t *list);
int trie_get_leafs_by_regex(trie_node_t *root, trie_regex_t *regex,
							kslist_head_t *list);

//
void trie_print_leafs_by_prefix(trie_node_t *root, char *str);
void trie_print_leafs_by_regex(trie_node_t *root, trie_regex_t *regex);

//
bool trie_search(trie_node_t *root, char *str);

//
uint32_t trie_letter_list_to_mask(char *str);

//
void trie_print(trie_node_t *root);

#endif	// TRIE_H
