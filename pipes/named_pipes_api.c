#include "named_pipes.h"

/* Print a message received from pipe */
void display_msg(struct pipe_msg p_msg)
{
	printf("PID = %d\nEND = %c\nMSG = %s\n", p_msg.pid, p_msg.end_transmission,
						 p_msg.msg);
}

/* Read message from pipe. Blocking */
int read_from_pipe(struct pipe_msg *p_msg)
{
	int read_fd;

	if ((read_fd = open(PIPE, O_RDONLY)) == -1) {
		fprintf(stderr, "Fail to open pipe for read [%d:%s]\n", errno,
									strerror(errno));
		return -1;
	}

	/*flush structure first */
	memset(p_msg, 0, sizeof(*p_msg));
	read(read_fd, p_msg, sizeof(*p_msg));
	close(read_fd);

	return 0;
}

/* Write message to pipe */
int write_to_pipe(const struct pipe_msg *p_msg)
{
	int write_fd;

	if ((write_fd = open(PIPE, O_WRONLY)) == -1) {
		fprintf(stderr, "Fail to open pipe for write [%d:%s]\n", errno,
									 strerror(errno));
		return -1;
	}

	write(write_fd, p_msg, sizeof(*p_msg));
	close(write_fd);

	return 0;
}

/* Create the message to be send on pipe */
void create_message(struct pipe_msg *p_msg)
{
	printf("Please enter message type : y/n (n end of transmission)\n");
	scanf("%c", &p_msg->end_transmission);
	printf("Please enter message payload\n");
	scanf("%s", p_msg->msg);
	p_msg->pid = getpid();
}
