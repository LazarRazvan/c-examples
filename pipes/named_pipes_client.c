#include "named_pipes.h"

void start_communication(void)
{
	struct pipe_msg p_msg;

	while(1) {
		create_message(&p_msg);
		display_msg(p_msg);
		write_to_pipe(&p_msg);
		break;
	}	
}
int main()
{
	/* Since server is running, pipe is created */
	start_communication();
	return 0;
}
