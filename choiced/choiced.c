#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "choiced.h"

choiced_status_t _daemon_main(int pipe_fds[], pid_t pid)
{
	choiced_status_t result = CHOICE_SUCCESS;
	while (1) {
		sleep(60);
	}
	return result;
}

choiced_status_t _father_process(int pipe_fds[], pid_t pid)
{
	choiced_status_t result = CHOICE_SUCCESS;

	write(pipe_fds[PIPE_WRITE], (void *)&pid, sizeof(pid));
	
	/* Cleaning up before forcefully exiting the process */
	CLOSE_PIPE_BOTHWAY(pipe_fds)

	/* Kill the father to make the kid a daemon */
	exit(result);

	return result;
}

choiced_status_t _child_process(int pipe_fds[])
{
	choiced_status_t result = CHOICE_SUCCESS;
	pid_t pid = 0;

	read(pipe_fds[PIPE_READ], (void *)&pid, sizeof(pid));

	CLOSE_PIPE_BOTHWAY(pipe_fds)

	result = _daemon_main(pipe_fds, pid);

	return result;
}

choiced_status_t fork_me(int pipe_fds[])
{
	choiced_status_t result = CHOICE_SUCCESS;
	pid_t pid = 0;

	pid = fork();
	if (-1 == pid) {
		result = CHOICE_FORK_FAILED;
		goto l_cleanup;
	} else if (!pid) {
		result = _child_process(pipe_fds);
	} else {
		result = _father_process(pipe_fds, pid);
	}

l_cleanup:
    return result;
}

int main(int argc, char * argv[])
{
	choiced_status_t result = CHOICE_SUCCESS;
	int pipe_fds[2] = {-1};

    if (7 == argc) {
        printf("I am a leet not a 1337\n");
    }

	if (-1 == pipe(pipe_fds)) {
		result = CHOICE_PIPE_FAILED;
		goto l_cleanup;
	}

	result = fork_me(pipe_fds);
	
l_cleanup:
    return result;
}
