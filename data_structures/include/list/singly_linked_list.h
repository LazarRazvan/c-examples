/**
 * Singly linked list implementation.
 * Copyright (C) 2022 Lazar Razvan.
 */

#include <stdbool.h>

/****************************** DATA STRUCTURE ********************************/

typedef struct node {

	void			*data;
	struct node		*next;

} slist_t;

/******************************** PUBLIC API **********************************/

/**
 * Callbacks.
 */
// Compare
typedef int (*cmp_cb)(void *, void *);
// Print
typedef void (*print_cb)(void *);

/**
 * Insert.
 */
// Insert as first element
int slist_insert_first(slist_t **, void *);
// Insert after a given element
int slist_insert_after(slist_t *, void *, void *, cmp_cb);
// Insert before a given element
int slist_insert_before(slist_t **, void *, void *, cmp_cb);
// Insert at the end of the list
int slist_insert_last(slist_t **, void *);

/**
 * Delete.
 */
// Delete first element
int slist_delete_first(slist_t **);
// Delete after a given element
int slist_delete_after(slist_t *, void *, cmp_cb);
// Delete after a given element
int slist_delete_before(slist_t **, void *, cmp_cb);
// Delete at the end of the list
int slist_delete_last(slist_t **);

/**
 * Get.
 */
// Get n-th node of list
slist_t *slist_get_nth(slist_t *, unsigned int);

/**
 * Print.
 */
// Print
void slist_print(slist_t *, print_cb);
// Reverse print
void slist_print_reverse(slist_t *, print_cb);
// Print middle element
void slist_print_middle(slist_t *, print_cb);

/**
 * Loop.
 */
// Check if list has a loop
bool slist_has_loop(slist_t *head);
// Get loop start node
slist_t *slist_loop_start(slist_t *head);

/**
 * Palindrome.
 */
// Check if a list is palindrome
bool slist_is_palindrome(slist_t **, slist_t *, cmp_cb);

/**
 * Duplicates.
 */
// Remove duplicates in sorted list
void slist_ordered_delete_duplicates(slist_t *, cmp_cb);

/**
 * Destroy.
 */
// Destroy list
void slist_destroy(slist_t **head);
