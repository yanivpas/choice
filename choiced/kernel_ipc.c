#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "dctl.h"
#include "kernel_ipc.h"

bool is_choice_running(void)
{
    struct stat buf = {0};
    bool result = true;

    if (-1 == stat(KERNEL_IPC_FILE_PATH , &buf)) {
        result = false;
    }

    return result;
}

choiced_status_t write_pid(pid_t pid)
{
    STATUS_INIT(status);
    int dip_fd = 0;

    dip_fd = open(KERNEL_IPC_FILE_PATH, O_WRONLY);
    if (-1 == dip_fd) {
        STATUS_LABEL(status, CHOICE_OPEN_FAILED);
        goto l_cleanup;
    }

    printf("%d\n", dip_fd);
    if (sizeof(pid) != write(dip_fd, (void *)&pid, sizeof(pid))) {
        STATUS_LABEL(status, CHOICE_WRITE_FAILED);
        goto l_cleanup;
    }

    STATUS_LABEL(status, CHOICE_SUCCESS);

l_cleanup:
    CLOSE(dip_fd);
    return status;
}
