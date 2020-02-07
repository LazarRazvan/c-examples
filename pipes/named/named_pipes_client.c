#include "named_pipes.h"

/*
 * Start communication with server. Build messages from stdin.
 * If 'y' is pass, server will close the connection, sending a
 * fin message to client. If 'n' is added in messages, communication
 * is still on
 */
void start_communication(void)
{
	struct pipe_msg p_msg;
	int stop = 1;

	while(stop) {
		/* take input from stdin */
		create_message(&p_msg, NULL);
		stop = p_msg.end_transmission == 'y' ? 0 : 1;
		display_msg(p_msg);
		write_to_pipe(&p_msg);
		read_from_pipe(&p_msg);
		display_msg(p_msg);
	}
}

int main()
{
	/* Since server is running, pipe is created */
	printf("Client starts with PID = %d\n", getpid());
	start_communication();
	return 0;
}
