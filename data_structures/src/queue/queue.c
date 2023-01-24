/**
 * Queue implementation with array.
 * Copyright (C) 2022 Lazar Razvan.
 */

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

#include "queue/queue.h"

/******************************** PUBLIC API **********************************/

/****************************** CREATE/DESTROY ********************************/
/**
 * Create a queue with a given size.
 *
 * @size	: Queue size.
 *
 * Return queue on success and NULL otherwise.
 */
queue_t *queue_create(unsigned int size)
{
	queue_t *queue = NULL;

	/* Allocate space for queue */
	queue = malloc(sizeof(queue_t));
	if (!queue)
		return NULL;

	/* Allocate space for array */
	queue->q = malloc(size * sizeof(void *));
	if (!queue->q) {
		free(queue);
		return NULL;
	}

	/* Initialization */
	queue->size = size;
	queue->_h = &queue->q[0];
	queue->_t = &queue->q[0];

	return queue;
}

/**
 * Destroy (free space) for a queue.
 */
void queue_destroy(queue_t *queue)
{
	if (!queue)
		return;

	free(queue->q);
	free(queue);
}

/********************************** DISPLAY ***********************************/

/**
 * Display queue elements.
 *
 * @queue	: Queue data structure
 * @_cb		: Print callback.
 */
void queue_display(queue_t *queue, print_cb _cb)
{
	void **it;

	if (!queue)
		return;

	for (it = queue->_h; it != queue->_t; it++)
		_cb(*it);
}

/****************************** ENQUEUE/DEQUEUE *******************************/

/**
 * Add an element to queue.
 *
 * @queue	: Queue data structure.
 * @elem	: Element to be added to queue.
 *
 * Return 0 on success and <0 otherwise.
 */
int queue_enqueue(queue_t *queue, void *elem)
{
	if (!queue || !elem)
		return -1;

	/* Check if queue is full */
	if (queue->_t >= &queue->q[0] + queue->size)
		return -2;

	/* Enqueue element */
	*(queue->_t++) = elem;

	return 0;
}

/**
 * Remove an element from queue.
 *
 * @queue	: Queue data structure.
 *
 * Return element on success and <0 otherwise.
 */
void *queue_dequeue(queue_t *queue)
{
	void *elem;

	if (!queue)
		return NULL;

	/* Check if queue is empty */
	if (queue->_h == queue->_t)
		return NULL;

	elem = *queue->_h;
	queue->_h++;

	return elem;
}

/*********************************** EMPTY ************************************/
/**
 * Check if queue is empty.
 *
 * @queue	: Queue data structure.
 *
 * Return true if queue is empty and false otherwise.
 */
bool queue_is_empty(queue_t *queue)
{
	if (!queue)
		return false;

	if (queue->_h == queue->_t)
		return true;

	return false;
}

/************************************ SIZE ************************************/
/**
 * Return the size of the queue.
 */

unsigned int queue_size(queue_t *queue)
{
	if (!queue)
		return 0;

	return queue->_t - queue->_h;
}

