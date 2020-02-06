#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include "sys/wait.h"
#include "sys/types.h"

#define READ_END	0
#define WRITE_END	1

#define MSG_SIZE	25

#define TEST_SAME	'1'
#define TEST_DIFF	'2'

const char msg[] = "Hello from pid";

/*
 * Read/write to/from a pipe in the same process
 */
void same_process(void)
{
	int pipe_fd[2];	/* 2 ends of the pipe */
	char msg_buf[MSG_SIZE];

	printf("Process pid = %d\n", getpid());

	/* Create the pipe */
	if (pipe(pipe_fd)) {
		fprintf(stderr, "Fail to create pipe\n");
		return;
	}

	memset(msg_buf, 0, MSG_SIZE);
	/* Write message to pipe */
	sprintf(msg_buf, "%s %d", msg, getpid());
	write(pipe_fd[WRITE_END], &msg_buf, MSG_SIZE);

	memset(msg_buf, 0, MSG_SIZE);
	/* Read data from pipe */
	read(pipe_fd[READ_END], &msg_buf, MSG_SIZE);

	printf("[PID = %d] %s\n", getpid(), msg_buf);
}

/*
 * Read/write to/from pipe using 2 processes
 */
void different_processes(void)
{
	int pipe_fd[2];
	int status;
	pid_t pid;
	char msg_buf[MSG_SIZE];
	
	/* Create the pipe */
	if (pipe(pipe_fd)) {
		fprintf(stderr, "Fail to create pipe\n");
		return;
	}

	/* flush buffer */
	memset(msg_buf, 0, MSG_SIZE);

	switch(pid = fork()) {
	case -1:
		fprintf(stderr, "Fail to create child process\n");
		return;
	case 0:
		printf("Child PID = %d\n", getpid());
		/* child process read from pipe*/
		close(pipe_fd[WRITE_END]);
		read(pipe_fd[READ_END], &msg_buf, MSG_SIZE);
		printf("[PID = %d] %s\n", getpid(), msg_buf);

		exit(0);
	default:
		printf("Parent PID = %d\n", getpid());
		/* parent process write to pipe */
		close(pipe_fd[READ_END]);
		sprintf(msg_buf, "%s %d", msg, getpid());
		write(pipe_fd[WRITE_END], &msg_buf, MSG_SIZE);
	}
	
	/* wait for child process to finish */
	waitpid(pid, &status, 0);

	printf("Child exits with status %d\n", status);
}

int main()
{
	char test_no;

	while(scanf("%c", &test_no)) {
		fgetc(stdin);
		switch(test_no) {
		case TEST_SAME:
			printf("Pipes using same process ...\n");
			same_process();
			break;
		case TEST_DIFF:
			printf("Pipes using different processes ...\n");
			different_processes();
			break;
		default:
			goto exit;
			break;
		}
	}
exit:
	return 0;
}
