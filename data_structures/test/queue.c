/**
 * Graph Adjancency Matric test.
 * Copyright (C) 2022 Lazar Razvan.
 */
#include <stddef.h>
#include <stdio.h>
#include <assert.h>

#include "queue/queue.h"

/********************************** STATIC ************************************/

/**
 * Queue data print callback.
 */
static void __queue_data_print(void *data)
{
	if (!data)
		return;

	printf("elem = %d\n", *((int *)data));
}

/******************************************************************************/

int main()
{
	int rv = 0;
	void *elem;
	queue_t *queue = NULL;
	int test_arr[] = {1,2,3,4,5,6,7,8,9};

	/**
	 * Create queue.
	 */
	queue = queue_create(4);
	if (!queue) {
		rv = -1; goto error;
	}

	/**
	 * Display queue.
	 */
	printf("Queue display:\n");
	queue_display(queue, __queue_data_print);

	/**
	 * Add elements to queue.
	 */
	printf("Enqueue\n");
	assert(!queue_enqueue(queue, &test_arr[0]));
	assert(!queue_enqueue(queue, &test_arr[1]));
	assert(!queue_enqueue(queue, &test_arr[2]));
	assert(!queue_enqueue(queue, &test_arr[3]));

	/**
	 * Display queue.
	 */
	printf("Queue display:\n");
	queue_display(queue, __queue_data_print);

	/**
	 * Remove elements from queue.
	 */
	printf("Dequeue\n");
	while (!queue_is_empty(queue)) {
		elem = queue_dequeue(queue);
		__queue_data_print(elem);
	}

	/**
	 * Display queue.
	 */
	printf("Queue display:\n");
	queue_display(queue, __queue_data_print);

	/**
	 * Queue destroy.
	 */
	queue_destroy(queue);

	return 0;

error:
	printf("Queue error %d\n", rv);

	return rv;
}
