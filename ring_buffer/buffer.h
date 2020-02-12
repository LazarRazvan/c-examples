/* Ring buffer design
 * Copyright (C) 2020 Lazar Razvan
 */

#include "stddef.h"
#include "string.h"
#include "errno.h"
#include "stdlib.h"
#include "stdio.h"

#define BUFFER_FULL	1
#define BUFFER_EMPTY	2

#define ON_ERR(x) \
do { \
	fprintf(stderr, "%s [%d: %s\n", __func__, (x), strerror(x)); \
} while(0) \

extern int errno;

/* Structure use for a ring buffer */
struct ring_buffer {
	void	**buffer;	/* buffer */
	size_t	elem_size;	/* sizeof elements in buffer */
	void	**head;		/* pointer to head of buffer */
	void	**tail;		/* pointer to end of buffer */
	size_t	size;		/* size of buffer */
	char	state;		/* buffer state */
};


struct ring_buffer * ring_buffer_init(size_t elem_size, size_t size);
void ring_buffer_free(struct ring_buffer *r_buffer);
int ring_buffer_add(struct ring_buffer *r_buffer, void *elem);
int ring_buffer_get(struct ring_buffer *r_buffer, void *elem);
