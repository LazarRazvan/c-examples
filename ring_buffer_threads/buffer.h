/* Ring buffer design
 * Copyright (C) 2020 Lazar Razvan
 */

#include "stddef.h"
#include "string.h"
#include "errno.h"
#include "stdlib.h"
#include "stdio.h"
#include "pthread.h"

/* Change after first put */
#define BUFFER_READY	1
#define BUFFER_WAIT	2

#define ON_ERR(x) \
do { \
	fprintf(stderr, "%s [%d: %s\n", __func__, (x), strerror(x)); \
} while(0) \

extern int errno;

/* Structure use for a ring buffer */
struct ring_buffer {
	void			**buffer;	/* buffer */
	volatile unsigned int	head;		/* pointer to head of buffer */
	volatile unsigned int	tail;		/* pointer to end of buffer */
	size_t			elem_size;	/* sizeof elements in buffer */
	size_t			size;		/* size of buffer */
#ifdef MULTI_THREADING
	pthread_mutex_t		r_mutex;	/* synchronize threads */
#endif
};


struct ring_buffer * ring_buffer_init(size_t elem_size, size_t order);
void ring_buffer_free(struct ring_buffer *r_buffer);
int ring_buffer_put(struct ring_buffer *r_buffer, void *elem);
int ring_buffer_get(struct ring_buffer *r_buffer, void *elem);
