#include "named_pipes.h"

/* Base messages */
const char ack_msg[] = "Massage has been received";
const char fin_msg[] = "Connection has been closed";

/*
 * Process the received message.
 * If the message is for end of transmission, send final
 * message and close connection. Otherwise, send ack
 * message to client.
 *
 * Return 0 if close connection and 1 otherwise.
 */
int process_message(struct pipe_msg *p_msg)
{
	if (p_msg->end_transmission == 'y') {
		create_message(p_msg, fin_msg);
		return 0;
	}
	create_message(p_msg, ack_msg);
	return 1;
}

/*
 * Start the communication with clients. Once a message is received,
 * it is processed and response is sent back to the client
 */
void start_communication(void)
{
	struct pipe_msg p_msg;
	int stop = 1;

	while(stop) {
		read_from_pipe(&p_msg);	
		display_msg(p_msg);
		/* Check end of transmission */
		stop = process_message(&p_msg);
		display_msg(p_msg);
		write_to_pipe(&p_msg);
	}	
}

int main()
{
	/* Create the pipe */
	if(mkfifo(PIPE, PERM)) {
		fprintf(stderr, "Fail to create pipe [%d:%s]\n", errno, strerror(errno));
		return -1;
	}

	printf("Server starts with PID = %d\n", getpid());
	start_communication();

	/* close the pipe */
	unlink(PIPE);
	return 0;
}
