#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include "fcntl.h"
#include "errno.h"
#include "sys/stat.h"
#include "sys/types.h"

#define PIPE	"/tmp/my_pipe"
#define PERM	0666

#define MSG_SIZE	1024

extern int errno;

/* Format of messages on pipe */
struct pipe_msg {
	pid_t pid;		/* pid of sender process */
	char end_transmission;	/* end of transmission */
	char msg[MSG_SIZE];	/* payload */
};

/* Exposed API */
void display_msg(struct pipe_msg p_msg);
int read_from_pipe(struct pipe_msg *p_msg);
int write_to_pipe(const struct pipe_msg *p_msg);
void create_message(struct pipe_msg *p_msg);
