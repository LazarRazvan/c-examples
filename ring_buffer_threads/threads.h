/* Ring buffer design
 * Copyright (C) 2020 Lazar Razvan
 */

#include "buffer.h"

#define MSG_SIZE	10
#define	MSG		"Hello"
#define RING_SIZE	32	/* multiple of 2 (for overflow situations) */
#define NUM_WRITES	10	/* number of messages each writer will add */

struct struct_t {
	unsigned long thread_id;
	char msg[MSG_SIZE];
};

struct ring_buffer *r_buf;
/* Keep track of all messages and stop when read all */
pthread_mutex_t	readers_mutex = PTHREAD_MUTEX_INITIALIZER;
int read_messages;
