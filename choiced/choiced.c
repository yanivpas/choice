#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>

#include "choiced.h"

#define PRINT(x) syslog(LOG_INFO, x)

choiced_status_t _daemon_main(int pipe_fds[], pid_t pid)
{
	choiced_status_t status = CHOICE_SUCCESS;

    openlog("choiced", 0, LOG_USER);
    PRINT("I am alive!!!");

	for (;;) {
		sleep(60);
	}
	return status;
}

choiced_status_t _father_process(int pipe_fds[], pid_t pid)
{
	choiced_status_t status = CHOICE_SUCCESS;

	write(pipe_fds[PIPE_WRITE], (void *)&pid, sizeof(pid));
	
	/* Cleaning up before forcefully exiting the process */
	CLOSE_PIPE_BOTHWAY(pipe_fds)

	/* Kill the father to make the kid a daemon */
	exit(status);

	return status;
}

choiced_status_t _child_process(int pipe_fds[])
{
	choiced_status_t status = CHOICE_SUCCESS;
	pid_t pid = -1;
    pid_t sid = -1;

	read(pipe_fds[PIPE_READ], (void *)&pid, sizeof(pid));

	CLOSE_PIPE_BOTHWAY(pipe_fds)

    sid = setsid();
    if (0 > sid) {
        status = CHOICE_SETSID_FAILD;
        goto l_cleanup;
    }

	status = _daemon_main(pipe_fds, pid);

l_cleanup:
	return status;
}

choiced_status_t fork_me(int pipe_fds[])
{
	choiced_status_t status = CHOICE_SUCCESS;
	pid_t pid = 0;

	pid = fork();
	if (-1 == pid) {
		status = CHOICE_FORK_FAILED;
		goto l_cleanup;
	} else if (!pid) {
		status = _child_process(pipe_fds);
	} else {
		status = _father_process(pipe_fds, pid);
	}

l_cleanup:
    return status;
}

int main(int argc, char * argv[])
{
	choiced_status_t status = CHOICE_SUCCESS;
	int pipe_fds[2] = {-1};

	if (7 == argc) {
		printf("I am a leet not a 1337\n");
	}

	if (-1 == pipe(pipe_fds)) {
		status = CHOICE_PIPE_FAILED;
		goto l_cleanup;
	}

	status = fork_me(pipe_fds);

l_cleanup:
	return status;
}
