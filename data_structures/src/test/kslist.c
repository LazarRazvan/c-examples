/**
 * Generic singly linked list test.
 * Copyright (C) 2022 Lazar Razvan.
 *
 * Test for generic singly linked list implementation using integer values as
 * data.
 */

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "list/ksingly_linked_list.h"

/********************************* GLOBALS ************************************/

#define DATA_SIZE		40
#define MAX_NAME		32

// Global of data entries for each node
typedef struct data_info_s {

	kslist_node_t	list;
	int				id;

} data_info_t;


/********************************** STATIC ************************************/

/**
 * Singly linked list data print callback.
 */
static void __kslist_data_print(kslist_node_t *node)
{
	data_info_t *data = kslist_entry(node, data_info_t, list);
	printf("id = %d\n", data->id);
}

/**
 * Allocate memory for test structures.
 */
static data_info_t *__create_data(void)
{
	data_info_t *arr = NULL;

	arr = malloc(DATA_SIZE * sizeof(data_info_t));
	if (!arr)
		return NULL;

	for (int i = 0; i < DATA_SIZE; i++)
		arr[i].id = i;

	return arr;
}

/**
 * Create first generic singly linked list.
 */
static void __create_list1(data_info_t *data, kslist_head_t *list1)
{
	int i;

	// add items to head
	for (i = 0; i < 5; i++)
		kslist_push_head(list1, &data[i].list);

	// add items to tail
	for (i = 5; i < 10; i++)
		kslist_push_tail(list1, &data[i].list);
}

/**
 * Create second generic singly linked list.
 */
static void __create_list2(data_info_t *data, kslist_head_t *list2)
{
	int i;

	// add items to head
	for (i = 10; i < 15; i++)
		kslist_push_head(list2, &data[i].list);

	// add items to tail
	for (i = 15; i < 20; i++)
		kslist_push_tail(list2, &data[i].list);
}

static void __test_insert(data_info_t *data, kslist_head_t *list1,
							kslist_head_t *list2)
{
	data_info_t *ptr;
	kslist_node_t *node;

	printf("==============================================================\n");
	printf("%s ...\n", __func__);
	printf("==============================================================\n");

	// list1 validate
	{
		// head
		node = kslist_get_head(list1);
		assert(node);
		ptr = kslist_entry(node, data_info_t, list);
		assert(ptr);

		if (ptr != &data[4]) {
			printf("Invalid list1 head!\n");
			assert(0);
		}

		// tail
		node = kslist_get_tail(list1);
		assert(node);
		ptr = kslist_entry(node, data_info_t, list);
		assert(ptr);

		if (ptr != &data[9]) {
			printf("Invalid list1 tail!\n");
			assert(0);
		}
	}

	// list2 validate
	{
		// head
		node = kslist_get_head(list2);
		assert(node);
		ptr = kslist_entry(node, data_info_t, list);
		assert(ptr);

		if (ptr != &data[14]) {
			printf("Invalid list2 head!\n");
			assert(0);
		}

		// tail
		node = kslist_get_tail(list2);
		assert(node);
		ptr = kslist_entry(node, data_info_t, list);
		assert(ptr);

		if (ptr != &data[19]) {
			printf("Invalid list2 tail!\n");
			assert(0);
		}
	}

	printf("[%s: %d] list1:\n", __func__, __LINE__);
	kslist_print(list1, __kslist_data_print);
	//
	printf("[%s: %d] list2:\n", __func__, __LINE__);
	kslist_print(list2, __kslist_data_print);
}

static void __test_replace(data_info_t *data, kslist_head_t *list1,
							kslist_head_t *list2)
{
	data_info_t *ptr;
	kslist_node_t *node;

	printf("==============================================================\n");
	printf("%s ...\n", __func__);
	printf("==============================================================\n");

	// list1 replace head and tail
	{
		// head
		kslist_replace(list1, &data[4].list, &data[24].list);
		// tail
		kslist_replace(list1, &data[9].list, &data[29].list);
	}

	// list2 replace head and tail
	{
		// head
		kslist_replace(list2, &data[14].list, &data[34].list);
		// tail
		kslist_replace(list2, &data[19].list, &data[39].list);
	}

	// list1 validate
	{
		// head
		node = kslist_get_head(list1);
		assert(node);
		ptr = kslist_entry(node, data_info_t, list);
		assert(ptr);

		if (ptr != &data[24]) {
			printf("Invalid list1 head!\n");
			assert(0);
		}

		// tail
		node = kslist_get_tail(list1);
		assert(node);
		ptr = kslist_entry(node, data_info_t, list);
		assert(ptr);

		if (ptr != &data[29]) {
			printf("Invalid list1 tail!\n");
			assert(0);
		}
	}

	// list2 validate
	{
		// head
		node = kslist_get_head(list2);
		assert(node);
		ptr = kslist_entry(node, data_info_t, list);
		assert(ptr);

		if (ptr != &data[34]) {
			printf("Invalid list2 head!\n");
			assert(0);
		}

		// tail
		node = kslist_get_tail(list2);
		assert(node);
		ptr = kslist_entry(node, data_info_t, list);
		assert(ptr);

		if (ptr != &data[39]) {
			printf("Invalid list2 tail!\n");
			assert(0);
		}
	}

	printf("[%s: %d] list1:\n", __func__, __LINE__);
	kslist_print(list1, __kslist_data_print);
	//
	printf("[%s: %d] list2:\n", __func__, __LINE__);
	kslist_print(list2, __kslist_data_print);
}

static void __test_pop(data_info_t *data, kslist_head_t *list1,
						kslist_head_t *list2)
{
	kslist_node_t *node;

	printf("==============================================================\n");
	printf("%s ...\n", __func__);
	printf("==============================================================\n");

	// list1 validate
	{
		// head
		node = kslist_pop_head(list1);
		assert(node);
		if (node != &data[24].list) {
			printf("Invalid list1 pop head!\n");
			assert(0);
		}

		// tail
		node = kslist_pop_tail(list1);
		assert(node);
		if (node != &data[29].list) {
			printf("Invalid list1 pop tail!\n");
			assert(0);
		}
	}

	// list2 validate
	{
		// head
		node = kslist_pop_head(list2);
		assert(node);
		if (node != &data[34].list) {
			printf("Invalid list2 pop head!\n");
			assert(0);
		}

		// tail
		node = kslist_pop_tail(list2);
		assert(node);
		if (node != &data[39].list) {
			printf("Invalid list2 pop tail!\n");
			assert(0);
		}
	}

	printf("[%s: %d] list1:\n", __func__, __LINE__);
	kslist_print(list1, __kslist_data_print);
	//
	printf("[%s: %d] list2:\n", __func__, __LINE__);
	kslist_print(list2, __kslist_data_print);
}


/******************************************************************************/

int main()
{
	data_info_t *data;
	KSLIST_HEAD(list1);
	KSLIST_HEAD(list2);

	// allocate test data
	data = __create_data();

	// create first list
	__create_list1(data, &list1);

	// create second list
	__create_list2(data, &list2);

	// validate insert
	__test_insert(data, &list1, &list2);

	// validate replace
	__test_replace(data, &list1, &list2);

	// validate pop
	__test_pop(data, &list1, &list2);

	// free memory
	free(data);

	return 0;
}
