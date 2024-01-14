/**
 * Trie data structure test.
 * Copyright (C) 2024 Lazar Razvan.
 */
#include <stddef.h>
#include <stdio.h>
#include <assert.h>

#include "trie/trie.h"


/*****************************************************************************/

int main()
{
	trie_node_t *root;

	// create trie root
	root = trie_node_create();
	if (!root) {
		printf("Unable to create trie root!\n");
		return -1;
	}

	// insert trie data
	assert(trie_insert(root, "hello"));
	assert(trie_insert(root, "helloa"));
	assert(trie_insert(root, "hei"));
	assert(trie_insert(root, "mama"));
	assert(trie_insert(root, "mata"));
	assert(trie_insert(root, "tata"));

	// trie lookup
	printf("String: hello [%s]\n", trie_search(root, "hello") ? "found" : "not_found");
	printf("String: helloa [%s]\n", trie_search(root, "helloa") ? "found" : "not_found");
	printf("String: hei [%s]\n", trie_search(root, "hei") ? "found" : "not_found");
	printf("String: mama [%s]\n", trie_search(root, "mama") ? "found" : "not_found");
	printf("String: mata [%s]\n", trie_search(root, "mata") ? "found" : "not_found");
	printf("String: tata [%s]\n", trie_search(root, "tata") ? "found" : "not_found");
	printf("String: razboi [%s]\n", trie_search(root, "razboi") ? "found" : "not_found");

	// trie longest prefix
	printf("Longest prefix: hello [%d]\n", trie_longest_prefix(root, "hello"));
	printf("Longest prefix: helloa [%d]\n", trie_longest_prefix(root, "helloa"));
	printf("Longest prefix: hei [%d]\n", trie_longest_prefix(root, "hei"));
	printf("Longest prefix: mama [%d]\n", trie_longest_prefix(root, "mama"));
	printf("Longest prefix: mata [%d]\n", trie_longest_prefix(root, "mata"));
	printf("Longest prefix: tata [%d]\n", trie_longest_prefix(root, "tata"));
	printf("Longest prefix: razboi [%d]\n", trie_longest_prefix(root, "razboi"));

	// print trie
	trie_print(root);
	printf("\n");

	// delete "hei"
	printf("Deleting \"hei\" from trie!\n");
	assert(trie_delete(root, "hei"));

	// trie lookup
	printf("String: hello [%s]\n", trie_search(root, "hello") ? "found" : "not_found");
	printf("String: helloa [%s]\n", trie_search(root, "helloa") ? "found" : "not_found");
	printf("String: hei [%s]\n", trie_search(root, "hei") ? "found" : "not_found");
	printf("String: mama [%s]\n", trie_search(root, "mama") ? "found" : "not_found");
	printf("String: mata [%s]\n", trie_search(root, "mata") ? "found" : "not_found");
	printf("String: tata [%s]\n", trie_search(root, "tata") ? "found" : "not_found");
	printf("String: razboi [%s]\n", trie_search(root, "razboi") ? "found" : "not_found");

	// trie longest prefix
	printf("Longest prefix: hello [%d]\n", trie_longest_prefix(root, "hello"));
	printf("Longest prefix: helloa [%d]\n", trie_longest_prefix(root, "helloa"));
	printf("Longest prefix: hei [%d]\n", trie_longest_prefix(root, "hei"));
	printf("Longest prefix: mama [%d]\n", trie_longest_prefix(root, "mama"));
	printf("Longest prefix: mata [%d]\n", trie_longest_prefix(root, "mata"));
	printf("Longest prefix: tata [%d]\n", trie_longest_prefix(root, "tata"));
	printf("Longest prefix: razboi [%d]\n", trie_longest_prefix(root, "razboi"));

	// print trie
	trie_print(root);
	printf("\n");

	// delete "hello"
	printf("Deleting \"hello\" from trie!\n");
	assert(trie_delete(root, "hello"));

	// trie lookup
	printf("String: hello [%s]\n", trie_search(root, "hello") ? "found" : "not_found");
	printf("String: helloa [%s]\n", trie_search(root, "helloa") ? "found" : "not_found");
	printf("String: hei [%s]\n", trie_search(root, "hei") ? "found" : "not_found");
	printf("String: mama [%s]\n", trie_search(root, "mama") ? "found" : "not_found");
	printf("String: mata [%s]\n", trie_search(root, "mata") ? "found" : "not_found");
	printf("String: tata [%s]\n", trie_search(root, "tata") ? "found" : "not_found");
	printf("String: razboi [%s]\n", trie_search(root, "razboi") ? "found" : "not_found");

	// trie longest prefix
	printf("Longest prefix: hello [%d]\n", trie_longest_prefix(root, "hello"));
	printf("Longest prefix: helloa [%d]\n", trie_longest_prefix(root, "helloa"));
	printf("Longest prefix: hei [%d]\n", trie_longest_prefix(root, "hei"));
	printf("Longest prefix: mama [%d]\n", trie_longest_prefix(root, "mama"));
	printf("Longest prefix: mata [%d]\n", trie_longest_prefix(root, "mata"));
	printf("Longest prefix: tata [%d]\n", trie_longest_prefix(root, "tata"));
	printf("Longest prefix: razboi [%d]\n", trie_longest_prefix(root, "razboi"));

	// print trie
	trie_print(root);
	printf("\n");

	return 0;
}
