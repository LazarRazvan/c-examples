/**
 * Singly linked list test.
 * Copyright (C) 2022 Lazar Razvan.
 *
 * Test for singly linked list implementation using integer values as data.
 */

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "list/singly_linked_list.h"

/********************************* GLOBALS ************************************/
#define DATA_SIZE		10
#define MAX_NAME		32

// Global of data entries for each node
typedef struct data_info_s {

	int		id;
	char	name[MAX_NAME];

} data_info_t;

data_info_t data_info[DATA_SIZE] = {
	{ .id = 0, .name = "Jordan" },
	{ .id = 1, .name = "Bryant" },
	{. id = 2, .name = "Kareem" },
	{. id = 3, .name = "Shaquille" },
	{. id = 4, .name = "Duncan" },
	{ .id = 5, .name = "Nowitzki" },
	{ .id = 6, .name = "James" },
	{ .id = 7, .name = "Mutombo" },
	{ .id = 8, .name = "Howard" },
	{ .id = 9, .name = "Divac" }
};

/********************************** STATIC ************************************/

/**
 * Singly linked list data print callback.
 */
static void __slist_data_print(void *data)
{
	printf("id = %d; name = %s\n", ((data_info_t *)data)->id,
			((data_info_t *)data)->name);
}

/**
 * Singly linked list data compare callback.
 */
static int __slist_data_cmp(void *data1, void *data2)
{
	return ((data_info_t *)data1)->id - ((data_info_t *)data2)->id;
}

/**
 * Create a singly linked list using data_info array information.
 *
 * @head	: Singly linked list head.
 *
 * Return 0 on success and <0 otherwise.
 */
static int __slist_create(slist_t **head)
{
	int rv = 0;
	data_info_t *data = NULL;

	for (int i = DATA_SIZE-1; i >= 0; i--) {
		data = malloc(sizeof(data_info_t));
		if (!data) {
			rv = -1; goto error;
		}
		memcpy(data, &data_info[i], sizeof(data_info_t));

		if (slist_insert_first(head, (void *)data)) {
			rv = -2; goto error;
		}
	}

	return 0;

error:
	printf("Singly linked list error %d\n", rv);

	return rv;
}

/*********************************** TESTS ************************************/
/**
 * Singly linked list loop detection test.
 */
static int __slist_loop_test(void)
{
	int rv = 0;
	slist_t *head = NULL, *loop_s = NULL, *node1 = NULL, *node2 = NULL;

	printf("==============================================================\n");
	printf("%s\n", __func__);
	printf("==============================================================\n");

	/**
	 * List create, print and check for loops.
	 */
	if (__slist_create(&head)) {
		rv = -1; goto error;
	}

	//
	printf("[%s] List print:\n", __func__);
	slist_print(head, __slist_data_print);
	printf("\n");

	//
	if (slist_has_loop(head))
		printf("[%s] List loop detected!\n", __func__);

	/**
	 * Create a loop inside list between 9-th node and 3-th node, check for
	 * loop and detect loop starting node.
	 */
	printf("[%s] Create loop inside list!\n", __func__);
	node1 = slist_get_nth(head, 3);
	node2 = slist_get_nth(head, 9);

	if (!node1 || !node2) {
		rv = -2; goto error_free;
	}

	node2->next = node1;

	//
	if (slist_has_loop(head))
		printf("[%s] List loop detected!\n", __func__);

	//
	loop_s = slist_loop_start(head);
	if (!loop_s) {
		rv = -3; goto error_free;
	}

	printf("[%s] Loop starting node:\n", __func__);
	__slist_data_print((void *)loop_s->data);

	/**
	 * Remove list loop and free space.
	 */
	printf("[%s] Remove loop from list!\n", __func__);
	node2->next = NULL;

	//
	printf("[%s] List print:\n", __func__);
	slist_print(head, __slist_data_print);
	printf("\n");

	//
	slist_destroy(&head);

	return 0;

error_free:
	slist_destroy(&head);

error:
	printf("[%s] Error: %d\n", __func__, rv);

	return rv;
}

/**
 * Singly linked list palindrome test.
 */
static int __slist_palindrome_test(void)
{
	int rv = 0;
	data_info_t *data = NULL;
	slist_t *old_head = NULL, *head = NULL;

	printf("==============================================================\n");
	printf("%s\n", __func__);
	printf("==============================================================\n");

	/**
	 * List create (1 -> 2 -> 3 -> 2 -> 1).
	 */
	//
	data = malloc(sizeof(data_info_t));
	if (!data) {
		rv = -1; goto error;
	}
	memcpy(data, &data_info[1], sizeof(data_info_t));

	if (slist_insert_first(&head, (void *)data)) {
		rv = -2; goto error;
	}

	//
	data = malloc(sizeof(data_info_t));
	if (!data) {
		rv = -3; goto error;
	}
	memcpy(data, &data_info[2], sizeof(data_info_t));

	if (slist_insert_last(&head, (void *)data)) {
		rv = -4; goto error;
	}

	//
	data = malloc(sizeof(data_info_t));
	if (!data) {
		rv = -5; goto error;
	}
	memcpy(data, &data_info[3], sizeof(data_info_t));

	if (slist_insert_last(&head, (void *)data)) {
		rv = -6; goto error;
	}

	//
	data = malloc(sizeof(data_info_t));
	if (!data) {
		rv = -7; goto error;
	}
	memcpy(data, &data_info[2], sizeof(data_info_t));

	if (slist_insert_last(&head, (void *)data)) {
		rv = -8; goto error;
	}

	//
	data = malloc(sizeof(data_info_t));
	if (!data) {
		rv = -9; goto error;
	}
	memcpy(data, &data_info[1], sizeof(data_info_t));

	if (slist_insert_last(&head, (void *)data)) {
		rv = -10; goto error;
	}

	/**
	 * Print list
	 */
	printf("[%s] List print:\n", __func__);
	slist_print(head, __slist_data_print);
	printf("\n");


	/**
	 * Palindrome check.
	 */
	old_head = head;
	if (slist_is_palindrome(&old_head, head, __slist_data_cmp))
		printf("[%s] List is palindrome!\n", __func__);
	else
		printf("[%s] List is not palindrome!\n", __func__);

	/**
	 * Remove first element, print list and palindrome check.
	 */
	if (slist_delete_first(&head)) {
		rv = -11; goto error;
	}

	//
	printf("[%s] List print:\n", __func__);
	slist_print(head, __slist_data_print);
	printf("\n");

	//
	old_head = head;
	if (slist_is_palindrome(&old_head, head, __slist_data_cmp))
		printf("[%s] List is palindrome!\n", __func__);
	else
		printf("[%s] List is not palindrome!\n", __func__);

	/**
	 * Free space.
	 */
	slist_destroy(&head);

	return 0;

error:
	slist_destroy(&head);
	printf("[%s] Error: %d\n", __func__, rv);

	return rv;
}

/**
 * Singly ordered linked list duplicates remove test.
 */
static int __slist_ordered_duplicates_test(void)
{
	int rv = 0;
	slist_t *head = NULL;
	data_info_t *data = NULL;

	printf("==============================================================\n");
	printf("%s\n", __func__);
	printf("==============================================================\n");

	/**
	 * List create (1 -> 1 -> 2 -> 2 -> 2 -> 3 -> 3).
	 */
	//
	data = malloc(sizeof(data_info_t));
	if (!data) {
		rv = -1; goto error;
	}
	memcpy(data, &data_info[1], sizeof(data_info_t));

	if (slist_insert_first(&head, (void *)data)) {
		rv = -2; goto error;
	}

	//
	data = malloc(sizeof(data_info_t));
	if (!data) {
		rv = -3; goto error;
	}
	memcpy(data, &data_info[1], sizeof(data_info_t));

	if (slist_insert_last(&head, (void *)data)) {
		rv = -4; goto error;
	}

	//
	data = malloc(sizeof(data_info_t));
	if (!data) {
		rv = -5; goto error;
	}
	memcpy(data, &data_info[2], sizeof(data_info_t));

	if (slist_insert_last(&head, (void *)data)) {
		rv = -6; goto error;
	}

	//
	data = malloc(sizeof(data_info_t));
	if (!data) {
		rv = -7; goto error;
	}
	memcpy(data, &data_info[2], sizeof(data_info_t));

	if (slist_insert_last(&head, (void *)data)) {
		rv = -8; goto error;
	}

	//
	data = malloc(sizeof(data_info_t));
	if (!data) {
		rv = -9; goto error;
	}
	memcpy(data, &data_info[2], sizeof(data_info_t));

	if (slist_insert_last(&head, (void *)data)) {
		rv = -10; goto error;
	}

	//
	data = malloc(sizeof(data_info_t));
	if (!data) {
		rv = -11; goto error;
	}
	memcpy(data, &data_info[3], sizeof(data_info_t));

	if (slist_insert_last(&head, (void *)data)) {
		rv = -12; goto error;
	}

	//
	data = malloc(sizeof(data_info_t));
	if (!data) {
		rv = -13; goto error;
	}
	memcpy(data, &data_info[3], sizeof(data_info_t));

	if (slist_insert_last(&head, (void *)data)) {
		rv = -14; goto error;
	}

	/**
	 * Print list
	 */
	printf("[%s] List print:\n", __func__);
	slist_print(head, __slist_data_print);
	printf("\n");

	/**
	 * Remove duplicates in ordered list.
	 */
	printf("[%s] Remove duplicates!\n", __func__);
	slist_ordered_delete_duplicates(head, __slist_data_cmp);

	/**
	 * Print list
	 */
	printf("[%s] List print:\n", __func__);
	slist_print(head, __slist_data_print);
	printf("\n");

	/**
	 * Free space.
	 */
	slist_destroy(&head);

	return 0;

error:
	slist_destroy(&head);
	printf("[%s] Error: %d\n", __func__, rv);

	return rv;
}

/******************************************************************************/

int main()
{
	int rv = 0;

	/**
	 * Loop detection test.
	 */
	if (__slist_loop_test()) {
		rv = -1; goto error;
	}

	/**
	 * Palindrome test.
	 */
	if (__slist_palindrome_test()) {
		rv = -2; goto error;
	}

	/**
	 * Ordered list duplicates remove test.
	 */
	if (__slist_ordered_duplicates_test()) {
		rv = -3; goto error;
	}

	return 0;

error:
	printf("Singly linked list error %d\n", rv);

	return rv;
}
