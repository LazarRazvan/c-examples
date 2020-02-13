#include "buffer.h"

#define MSG_SIZE	10
#define	MSG		"Hello"
#define RING_SIZE	1024	/* order */
#define NUM_WRITES	10	/* number of messages each writer will add */

struct struct_t {
	int thread_id;
	char msg[MSG_SIZE];
};

struct ring_buffer *r_buf;
/* Keep track of all messages and stop when read all */
pthread_mutex_t	readers_mutex = PTHREAD_MUTEX_INITIALIZER;
int read_messages;
