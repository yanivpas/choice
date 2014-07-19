#include <sys/stat.h>

#include "status.h"
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
