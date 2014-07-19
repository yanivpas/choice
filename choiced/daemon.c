#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>

#include "status.h"
#include "daemon.h"
#include "kernel_ipc.h"

#define PRINT(x) syslog(LOG_INFO, x)

static choiced_status_t _daemon_main(int pipe_fds[], pid_t pid)
{
    STATUS_INIT(status);

    openlog("choiced", 0, LOG_USER);

    if (!is_choice_running()) {
        PRINT("Choice is not running");
        goto l_exit;
    }

    PRINT("I am alive!!!");

    for (;;) {
        sleep(60);
    }

l_exit:
    return status;
}

static choiced_status_t _father_process(int pipe_fds[], pid_t pid)
{
    STATUS_INIT(status);

    write(pipe_fds[PIPE_WRITE], (void *)&pid, sizeof(pid));
    
    /* Cleaning up before forcefully exiting the process */
    CLOSE_PIPE_BOTHWAY(pipe_fds);

    STATUS_LABEL(status, CHOICE_SUCCESS);

    /* Kill the father to make the kid a daemon */
    exit((int)status);

    return status;
}

static choiced_status_t _child_process(int pipe_fds[])
{
    STATUS_INIT(status);
    pid_t pid = -1;
    pid_t sid = -1;

    read(pipe_fds[PIPE_READ], (void *)&pid, sizeof(pid));

    CLOSE_PIPE_BOTHWAY(pipe_fds)

    sid = setsid();
    if (0 > sid) {
        STATUS_LABEL(status, CHOICE_SETSID_FAILD);
        goto l_cleanup;
    }

    STATUS_ASSIGN(status, _daemon_main(pipe_fds, pid));

l_cleanup:
    return status;
}

choiced_status_t fork_me(int pipe_fds[])
{
    STATUS_INIT(status);
    pid_t pid = 0;

    pid = fork();
    if (-1 == pid) {
        STATUS_LABEL(status, CHOICE_FORK_FAILED);
        goto l_cleanup;
    } else if (!pid) {
        STATUS_ASSIGN(status, _child_process(pipe_fds));
    } else {
        STATUS_ASSIGN(status, _father_process(pipe_fds, pid));
    }

l_cleanup:
    return status;
}

int main(int argc, char * argv[])
{
    STATUS_INIT(status);
    int pipe_fds[2] = {-1};

    if (7 == argc) {
        printf("I am a leet not a 1337\n");
    }

    if (-1 == pipe(pipe_fds)) {
        STATUS_LABEL(status, CHOICE_PIPE_FAILED);
        goto l_cleanup;
    }

    STATUS_ASSIGN(status, fork_me(pipe_fds));

l_cleanup:
    return status;
}
