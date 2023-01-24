/**
 * Queue implementation with array.
 * Copyright (C) 2022 Lazar Razvan.
 *
 * Note that elements are not popped out from the queue array but instead two
 * pointers are increased.
 */

#include <stdbool.h>

/****************************** DATA STRUCTURE ********************************/

typedef struct queue_s {

	void 			**q;		// queue array
	void			**_h;		// queue head pointer
	void			**_t;		// queue tail pointer
	unsigned int	size;		// queue size

} queue_t;

/******************************** PUBLIC API **********************************/

/**
 * Create/Destroy.
 */
queue_t *queue_create(unsigned int);
void queue_destroy(queue_t *);

/**
 * Display.
 */
// Print callback
typedef void (*print_cb)(void *);
void queue_display(queue_t *, print_cb);

/**
 * Enqueue/Dequeue.
 */
int queue_enqueue(queue_t *, void *);
void *queue_dequeue(queue_t *);

/**
 * Empty.
 */
bool queue_is_empty(queue_t *);

/**
 * Size.
 */
unsigned int queue_size(queue_t *);

