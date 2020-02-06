#include "named_pipes.h"

void start_communication(void)
{
	struct pipe_msg p_msg;

	while(1) {
		read_from_pipe(&p_msg);	
		display_msg(p_msg);
		break;
	}	
}
int main()
{
	/* Create the pipe */
	if(mkfifo(PIPE, PERM)) {
		fprintf(stderr, "Fail to create pipe [%d:%s]\n", errno, strerror(errno));
		return -1;
	}

	start_communication();
	return 0;
}
