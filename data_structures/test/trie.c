/**
 * Trie data structure test.
 * Copyright (C) 2024 Lazar Razvan.
 */
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include <string.h>

#include "trie/trie.h"


/*****************************************************************************/

#define INPUT_FILE			"words.txt"


/*****************************************************************************/

static trie_node_t *__trie_create(void)
{
	FILE *input;
	char buf[MAX_WORD_LEN];
	trie_node_t *root = NULL;

	// open file
	input = fopen(INPUT_FILE, "r");
	if (!input) {
		printf("Unable to open %s file!\n", INPUT_FILE);
		goto error;
	}

	// create trie root
	root = trie_node_create();
	if (!root) {
		printf("Unable to create trie root!\n");
		goto file_close;
	}

	// create trie nodes
	while (fgets(buf, MAX_WORD_LEN, input)) {
		buf[strlen(buf)-1] = '\0';
		if (!trie_insert(root, buf)) {
			printf("Unable to insert \"%s\" in trie!\n", buf);
			goto trie_destroy;
		}
	}

	//
	fclose(input);

	return root;

trie_destroy:
	trie_node_destroy(root);

file_close:
	fclose(input);

error:
	return NULL;
}

static void __trie_test(trie_node_t *root)
{
	uint32_t mask;

	// print leafs by prefix
	printf("==============================================================\n");
	printf("Print leafs by prefix \"ab\"...\n");
	trie_print_leafs_by_prefix(root, "ab");
	printf("==============================================================\n");
	//
	printf("==============================================================\n");
	printf("Print leafs by prefix \"ca\"...\n");
	trie_print_leafs_by_prefix(root, "ca");
	printf("==============================================================\n");

	// trie lookup
	printf("==============================================================\n");
	printf("String: hello [%s]\n", trie_search(root, "hello") ? "found" : "not_found");
	printf("==============================================================\n");
	printf("String: helloa [%s]\n", trie_search(root, "helloa") ? "found" : "not_found");
	printf("==============================================================\n");
	printf("String: hei [%s]\n", trie_search(root, "hei") ? "found" : "not_found");
	printf("==============================================================\n");
	printf("String: car [%s]\n", trie_search(root, "car") ? "found" : "not_found");
	printf("==============================================================\n");
	printf("String: adasd [%s]\n", trie_search(root, "adasd") ? "found" : "not_found");
	printf("==============================================================\n");
	printf("String: zzcas [%s]\n", trie_search(root, "zzcas") ? "found" : "not_found");
	printf("==============================================================\n");
	printf("String: rasad [%s]\n", trie_search(root, "rasad") ? "found" : "not_found");
	printf("==============================================================\n");

	// trie longest prefix
	printf("==============================================================\n");
	printf("Longest prefix: hello [%d]\n", trie_longest_prefix(root, "hello"));
	printf("==============================================================\n");
	printf("Longest prefix: helloa [%d]\n", trie_longest_prefix(root, "helloa"));
	printf("==============================================================\n");
	printf("Longest prefix: hei [%d]\n", trie_longest_prefix(root, "hei"));
	printf("==============================================================\n");
	printf("Longest prefix: car [%d]\n", trie_longest_prefix(root, "car"));
	printf("==============================================================\n");
	printf("Longest prefix: adasd [%d]\n", trie_longest_prefix(root, "adasd"));
	printf("==============================================================\n");
	printf("Longest prefix: zzcas [%d]\n", trie_longest_prefix(root, "zzcas"));
	printf("==============================================================\n");
	printf("Longest prefix: rasad [%d]\n", trie_longest_prefix(root, "rasad"));
	printf("==============================================================\n");

	// trie regex
	printf("==============================================================\n");
	printf("Print all words of 5 letters in list \"abcdefghijkl\"...\n");
	mask = trie_letter_list_to_mask("abcdefghijkl");

	trie_regex_t regex1 = {
		.word_len = 5,
		.word_list = {
			[0] = {.opt = VAL_IN_LIST, .mask = mask },
			[1] = {.opt = VAL_IN_LIST, .mask = mask },
			[2] = {.opt = VAL_IN_LIST, .mask = mask },
			[3] = {.opt = VAL_IN_LIST, .mask = mask },
			[4] = {.opt = VAL_IN_LIST, .mask = mask },
		}
	};

	trie_print_leafs_by_regex(root, &regex1);
	printf("==============================================================\n");
	printf("==============================================================\n");
	printf("Print all words of 6 letters where:\n");
	printf("Letter 0 not in list: \"abcdefghijkl\"...\n");
	printf("Letter 1 in list    : \"abcdefghijkl\"...\n");
	printf("Letter 2 in list    : \"lmn\"...\n");
	printf("Letter 3 in list    : \"abcdef\"...\n");
	printf("Letter 4 not in list: \"xyz\"...\n");
	printf("Letter 5 not in list: \"abc\"...\n");

	trie_regex_t regex2 = {
		.word_len = 6,
		.word_list = {
			[0] = {.opt = VAL_NOT_IN_LIST, .mask = trie_letter_list_to_mask("abcdefghijkl") },
			[1] = {.opt = VAL_IN_LIST,     .mask = trie_letter_list_to_mask("abcdefghijkl") },
			[2] = {.opt = VAL_IN_LIST,     .mask = trie_letter_list_to_mask("lmn") },
			[3] = {.opt = VAL_IN_LIST,     .mask = trie_letter_list_to_mask("abcdef") },
			[4] = {.opt = VAL_NOT_IN_LIST, .mask = trie_letter_list_to_mask("xyz") },
			[5] = {.opt = VAL_NOT_IN_LIST, .mask = trie_letter_list_to_mask("abc") },
		}
	};

	trie_print_leafs_by_regex(root, &regex2);
	printf("==============================================================\n");
}


/*****************************************************************************/

#define LETTER_LIST_SIZE		15	// list of letters displayed


static char *__hangman_get_random_word(trie_node_t *root, int word_len)
{
	uint32_t mask_all;
	kslist_node_t *it;
	trie_regex_t regex;
	word_entry_t *entry;
	kslist_head_t word_list;
	int list_len, random_idx;

	// init list
	kslist_head_init(&word_list);

	// get words with all letters
	mask_all = trie_letter_list_to_mask("abcdefghijklmnopqrstuvwxyz");

	// create regex
	regex.word_len = word_len;
	for (int i = 0; i < regex.word_len; i++) {
		regex.word_list[i].opt = VAL_IN_LIST;
		regex.word_list[i].mask = mask_all;
	}

	// get all words
	assert(!trie_get_leafs_by_regex(root, &regex, &word_list));

	// count entries
	list_len = 0;
	kslist_for_each(it, &word_list)
		list_len++;

	// select a random number in entries
	srand(time(NULL));
	random_idx = 1 + rand() % (list_len);

	kslist_for_each(it, &word_list) {
		if (random_idx) {
			random_idx--;
			continue;
		}

		entry = container_of(it, word_entry_t, node);
		return strdup(entry->word);
	}

	return NULL;

	// TODO: Free list memory
}


static void __hangman_get_letter_list(char *letter_list, char *word)
{
	int random_offset;
	char random_letter;
	bool unique_letter;

	srand(time(NULL));

	// reset buffer
	memset(letter_list, 0, LETTER_LIST_SIZE);

	// add unique letters left in the word at random offsets
	for (int i = 0; i < strlen(word); i++) {
		unique_letter = true;

		//
		for (int j = 0; j < i; j++) {
			if (word[j] == word[i]) {
				unique_letter = false;
				break;
			}
		}

		//
		if (!unique_letter)
			continue;

		//
		do {
			random_offset = rand() % LETTER_LIST_SIZE;
		} while (letter_list[random_offset] != 0);

		letter_list[random_offset] = word[i];
	}

	// add random letters to fill up the list
	for (int i = 0; i < LETTER_LIST_SIZE; i++) {
		if (letter_list[i])
			continue;

		//
		while (1) {
			random_letter = 'a' + rand() % 26;

			//
			unique_letter = true;
			for (int j = 0; j < LETTER_LIST_SIZE; j++) {
				if (letter_list[j] == random_letter) {
					unique_letter = false;
					break;
				}
			}

			//
			if (!unique_letter)
				continue;

			//
			letter_list[i] = random_letter;
			break;
		}
	}
}


static void __hangman_print(int mistakes)
{
	switch(mistakes) {
	case 0:
		printf("  +---+\n  |   |\n  |\n  |\n  |\n-----");
		break;
	case 1:
		printf("  +---+\n  |   |\n  |   O\n  |\n  |\n-----");
		break;
	case 2:
		printf("  +---+\n  |   |\n  |   O\n  |   |\n  |\n-----");
		break;
	case 3:
		printf("  +---+\n  |   |\n  |   O\n  |  /|\n  |\n-----");
		break;
	case 4:
		printf("  +---+\n  |   |\n  |   O\n  |  /|\\\n  |\n-----");
		break;
	case 5:
		printf("  +---+\n  |   |\n  |   O\n  |  /|\\\n  |  /\n-----");
		break;
	case 6:
		printf("  +---+\n  |   |\n  |   O\n  |  /|\\\n  |  / \\\n-----");
		break;
	default:
		printf("Hangman figure error");
	}

	printf("\n");
}


static void __hangman_solver(trie_node_t *root)
{
	bool found;
	trie_regex_t regex;
	char *word_to_guess;
	char letter, letter_list[LETTER_LIST_SIZE];
	int mistakes, word_len, word_to_guess_mask, letter_list_mask;
	uint32_t mask_all= trie_letter_list_to_mask("abcdefghijklmnopqrstuvwxyz");

	/**************************************************************************
	 * Choose word length
	 *************************************************************************/
	while (1) {
		// clear terminal
		printf("\033[H\033[J");

		// select word length
		printf("Enter the length of the word[5, 6, 7]: ");
		scanf("%d", &word_len);

		// validate word len
		if (word_len != 5 && word_len != 6 && word_len != 7)
			continue;

		break;
	}

	printf("word length = %d\n", word_len);

	/**************************************************************************
	 * Choose random word of specified length and set mask (only first letter
	 * will be displayed at first).
	 *
	 * A letter in word is displayed if coresponding bit is 1.
	 *************************************************************************/
	word_to_guess = __hangman_get_random_word(root, word_len);
	word_to_guess_mask = (1 << 0);

	// TODO: Debug
	printf("Word to guess = %s\n", word_to_guess);

	/**************************************************************************
	 * Get letters list for the game. This contain unique letters from the
	 * word to guess (except the first one) and some other random letters
	 *
	 * Also create a mask that help to displayed only letters that are not yet
	 * selected by player.
	 *
	 * If bit is 1, display the letter because was not selected by the player,
	 * otherwise not display because was already chosen.
	 *************************************************************************/
	__hangman_get_letter_list(letter_list, word_to_guess+1);
	letter_list_mask = (1 << (LETTER_LIST_SIZE+1)) - 1;

	// TODO: Debug
	printf("Letter list = %.*s\n", LETTER_LIST_SIZE, letter_list);

	/**************************************************************************
	 * Start game
	 *************************************************************************/
	mistakes = 0;

	while (mistakes < 6) {
		//
		// clear terminal
		//
		printf("\033[H\033[J");

		//
		// display word
		//
		printf("WORD: ");
		for (int i = 0; i < word_len; i++) {
			if (word_to_guess_mask & (1 << i))
				printf("%c ", word_to_guess[i]);	// display letter
			else
				printf("_ ");						// hidden letter
		}
		printf("\n");

		//
		// display letters
		//
		printf("LETTERS: { ");
		for (int i = 0; i < LETTER_LIST_SIZE; i++) {
			if (letter_list_mask & (1 << i))
				printf("%c ", letter_list[i]);		// display letter
		}
		printf("}\n");

		//
		// display hangman
		//
		__hangman_print(mistakes);

		//
		// display hints of all possible words
		//
		char single_letter[2];
		regex.word_len = word_len;
		for (int i = 0; i < strlen(word_to_guess); i++) {
			regex.word_list[i].opt = VAL_IN_LIST;

			if (word_to_guess_mask & (1 << i)) {
				// letter displayed (perfect match)
				single_letter[0] = word_to_guess[i];
				single_letter[1] = '\0';
				regex.word_list[i].mask = trie_letter_list_to_mask(single_letter);
			} else {
				// letter not displayed (all letters)
				regex.word_list[i].mask = mask_all;
			}
		}

		//
		// pick a letter in list
		//
		found = false;
		getchar();
		while (1) {
			printf("Select a letter in list: ");
			scanf("%c", &letter);
			printf("\n");

			// make sure letter is in list
			for (int i = 0; i < LETTER_LIST_SIZE; i++) {
				if (letter == letter_list[i]) {
					// disable letter in mask
					letter_list_mask &= ~(1 << i);
					found = true;
					break;
				}
			}

			// make sure letter is in list
			if (found)
				break;
			else
				continue;
		}

		//
		// reveal word letters (if found)
		//
		found = false;
		for (int i = 0; i < strlen(word_to_guess); i++)	{
			if (word_to_guess[i] == letter) {
				word_to_guess_mask |= (1 << i);
				found = true;
			}
		}

		if (!found)
			mistakes++;

		//
		// check win
		//
		if (word_to_guess_mask == (1 << (word_len)) - 1)
			break;
	}

	printf("==============================================================\n");
	if (mistakes < 6) {
		printf("GAME WIN !!!\n");
	} else {
		printf("GAME LOST. Word to guess = %s\n", word_to_guess);
	}
	printf("==============================================================\n");
}


/*****************************************************************************/

int main()
{
	trie_node_t *root;

	// create trie
	root = __trie_create();
	if (!root)
		return -1;

	// test
	//__trie_test(root);

	// hangman
	__hangman_solver(root);

	// free trie
	trie_node_destroy(root);

#if 0

	printf("trie_get_leafs_by_prefix \"a\"...\n");

	kslist_node_t *it;
	word_entry_t *entry;
	KSLIST_HEAD(words_list);

	if (trie_get_leafs_by_prefix(root, "a", &words_list)) {
		printf("Unable to get list of words by prefix!\n");
		return -1;
	}

	kslist_for_each(it, &words_list) {
		entry = container_of(it, word_entry_t, node);

		printf("%s\n", entry->word);
	}

#endif

#if 0
	printf("trie_print_leafs_by_prefix \"a\"...\n");
	trie_print_leafs_by_prefix(root, "a");
#endif

#if 0
	// insert trie data
	assert(trie_insert(root, "hello"));
	assert(trie_insert(root, "helloa"));
	assert(trie_insert(root, "hei"));
	assert(trie_insert(root, "mama"));
	assert(trie_insert(root, "mata"));
	assert(trie_insert(root, "tata"));
	assert(trie_insert(root, "masin"));
	assert(trie_insert(root, "avion"));
	assert(trie_insert(root, "elico"));

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
	assert(trie_insert(root, "ahead"));
#endif

#if 0
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
#endif

#if 0
	trie_print_leafs_by_prefix(root, "ma");

	printf("Use regex!\n");

	uint32_t mask = trie_letter_list_to_mask("abcdefghijklmnopqrstuvwxyz");

	trie_regex_t regex = {
		.word_len = 6,
		.word_list = {
			[0] = {.opt = VAL_IN_LIST, .mask = mask },
			[1] = {.opt = VAL_IN_LIST, .mask = mask },
			[2] = {.opt = VAL_IN_LIST, .mask = mask },
			[3] = {.opt = VAL_IN_LIST, .mask = mask },
			[4] = {.opt = VAL_IN_LIST, .mask = mask },
			[5] = {.opt = VAL_IN_LIST, .mask = mask },
			[5] = {.opt = VAL_NOT_IN_LIST, .mask = trie_letter_list_to_mask("t") },
		}
	};

	trie_print_leafs_by_regex(root, &regex);
#endif

	return 0;
}
