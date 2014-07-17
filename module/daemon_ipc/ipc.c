#include <linux/vmalloc.h>
#include "../sys_dir/sys_dir.h"
#include "../status.h"
#include "ipc.h"

syd_obj g_syd_ipc = {0};

chc_status_t _read_ipc_file(void * context) {
    STATUS_INIT(status);
    return status;
}

chc_status_t _write_ipc_file(
        void *context,
        char *buffer,
        size_t buffer_size) {

    STATUS_INIT(status);
    return status;
}

int ipc_init(void) {
    STATUS_INIT(status);

    g_syd_ipc.context = NULL;

    g_syd_ipc.name = (char *)vzalloc(sizeof(DAEMON_IPC_FILE));
    if (NULL == g_syd_ipc) {
        STATUS_LABEL(status, CHC_DIP_VZALLOC);
        goto l_exit;
    }
    memcpy(g_syd_ipc.name, DAEMON_IPC_FILE, sizeof(DAEMON_IPC_FILE));

    g_syd_ipc.ops.read = _read_ipc_file;
    g_syd_ipc.ops.write = _write_ipc_file;

l_exit:
    return (int) status;
}
