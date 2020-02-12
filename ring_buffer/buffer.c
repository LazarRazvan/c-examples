/* Ring buffer design
 * Copyright (C) 2020 Lazar Razvan
 */

#include "buffer.h"

/*
 * Init a ring buffer.
 *
 * @buffer:	Buffer from client
 * @elem_size:	Sizeof elements
 * @size:	Size of the buffer
 */
struct ring_buffer * ring_buffer_init(size_t elem_size, size_t size)
{
	int i;
	struct ring_buffer *r_buffer;

	r_buffer = (struct ring_buffer *) malloc(sizeof(*r_buffer));
	if (!r_buffer) {
		ON_ERR(errno);
		goto out_err;
	}

	r_buffer->buffer = (void **) malloc(size * sizeof(void*));
	if (!r_buffer->buffer) {
		ON_ERR(errno);
		goto out_err_1;
	}
	for (i = 0; i < size; i++) {
		r_buffer->buffer[i] = (void *) malloc(sizeof(void));
		if (!r_buffer->buffer[i]) {
			ON_ERR(errno);
			goto out_err_1;
		}
	}

	r_buffer->elem_size = elem_size;
	r_buffer->size = size;
	r_buffer->head = r_buffer->buffer;
	r_buffer->tail = r_buffer->buffer;
	r_buffer->state = BUFFER_EMPTY;

	return r_buffer;
out_err_1:
	free(r_buffer);
out_err:
	return NULL;
}

/*
 * Flush a ring buffer
 */
void ring_buffer_free(struct ring_buffer *r_buffer)
{
	int i;

	if (r_buffer) {
		for (i = 0; i < r_buffer->size; i++)
			free(r_buffer->buffer[i]);
		free(r_buffer->buffer);
		free(r_buffer);
		r_buffer = NULL;
	}
}

/*
 * Add an element in ring buffer.
 *
 * Please note that if the buffer is FULL, -1 is returned and the element
 * is not added. On success, 0 is returned.
 */
int ring_buffer_add(struct ring_buffer *r_buffer, void *elem)
{

	if (r_buffer->state == BUFFER_FULL)
		return -1;

	memcpy(*r_buffer->head, elem, r_buffer->elem_size);
	r_buffer->head++;

	/* reset head */
	if (r_buffer->head > r_buffer->buffer + (r_buffer->size - 1))
		r_buffer->head = r_buffer->buffer;

	/* check buffer */
	if (r_buffer->head == r_buffer->tail)
		r_buffer->state = BUFFER_FULL;

	return 0;
}

/*
 * Extract an element from ring buffer.
 *
 * If buffer is EMPTY, -1 is returned and there is no value inside elem.
 * On success, 0 is returned.
 */
int ring_buffer_get(struct ring_buffer *r_buffer, void *elem)
{

	if (r_buffer->state == BUFFER_EMPTY)
		return -1;

	memcpy(elem, *r_buffer->tail, r_buffer->elem_size);
	r_buffer->tail++;

	/* reset tail */
	if (r_buffer->tail > r_buffer->buffer + (r_buffer->size - 1))
		r_buffer->tail = r_buffer->buffer;

	/* check buffer */
	if (r_buffer->tail == r_buffer->head)
		r_buffer->state = BUFFER_EMPTY;

	return 0;
}

/*
 * Reset the ring buffer.
 */
void ring_buffer_reset()
{
	r_buffer->head = r_buffer->tail = r_buffer->buffer;
	r_buffer->state = BUFFER_EMPTY;
}
