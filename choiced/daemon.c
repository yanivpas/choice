#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <fcntl.h>

#include "dctl.h"
#include "daemon.h"
#include "kernel_ipc.h"

#define PRINT(x) syslog(LOG_INFO, x)

static void _wait(void)
{
    for (;;) {
        sleep(60);
    }
}

static choiced_status_t _daemon_main(pid_t pid)
{
    STATUS_INIT(status);

    if (!is_choice_running()) {
        PRINT("Choice is not running");
        STATUS_LABEL(status, CHOICE_NOT_RUNNING);
        goto l_cleanup;
    }

    PRINT("I am alive!!!");

    STATUS_ASSIGN(status, write_pid(pid));
    if (STATUS_IS_ERROR(status)) {
        goto l_cleanup;
    }

    _wait();

l_cleanup:
    return status;
}

static choiced_status_t _father_process(pid_t pid)
{
    STATUS_INIT(status);

    STATUS_LABEL(status, CHOICE_SUCCESS);

    /* Kill the father to make the kid a daemon */
    exit((int)status);

    return status;
}

static choiced_status_t _child_process(void)
{
    STATUS_INIT(status);
    pid_t pid = -1;
    pid_t sid = -1;

    pid = getpid();

    sid = setsid();
    if (0 > sid) {
        STATUS_LABEL(status, CHOICE_SETSID_FAILD);
        goto l_cleanup;
    }

    openlog("choiced", 0, LOG_USER);

    STATUS_ASSIGN(status, _daemon_main(pid));

l_cleanup:
    return status;
}

choiced_status_t fork_me(void)
{
    STATUS_INIT(status);
    pid_t pid = 0;

    pid = fork();
    if (-1 == pid) {
        STATUS_LABEL(status, CHOICE_FORK_FAILED);
        goto l_cleanup;
    } else if (!pid) {
        STATUS_ASSIGN(status, _child_process());
    } else {
        STATUS_ASSIGN(status, _father_process(pid));
    }

l_cleanup:
    return status;
}

int main(int argc, char * argv[])
{
    STATUS_INIT(status);

    if (7 == argc) {
        printf("I am a leet not a 1337\n");
    }

    STATUS_ASSIGN(status, fork_me());

    return status;
}
