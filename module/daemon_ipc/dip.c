#include <linux/vmalloc.h>
#include "../sys_dir/sys_dir.h"
#include "../status.h"
#include "dip.h"

struct syd_obj *g_syd_ipc = NULL;

chc_status_t _read_ipc_file(void * context)
{
    STATUS_INIT(status);
    printk("read\n");
    return status;
}

chc_status_t _write_ipc_file(
        void *context,
        const char *buffer,
        size_t buffer_size)
{

    STATUS_INIT(status);
    printk("write\n");
    return status;
}

chc_status_t _init_syd_ipc(void)
{
    STATUS_INIT(status);

    g_syd_ipc = (struct syd_obj *)vzalloc(sizeof(*g_syd_ipc));
    if (NULL == g_syd_ipc) {
        STATUS_LABEL(status, CHC_DIP_VZALLOC);
        goto l_exit;
    }

    g_syd_ipc->context = NULL;

    g_syd_ipc->name = (char *)vzalloc(sizeof(DAEMON_IPC_FILE));
    if (NULL == g_syd_ipc->name) {
        STATUS_LABEL(status, CHC_DIP_VZALLOC);
        goto l_exit;
    }
    memcpy(g_syd_ipc->name, DAEMON_IPC_FILE, sizeof(DAEMON_IPC_FILE));

    g_syd_ipc->ops->read = _read_ipc_file;
    g_syd_ipc->ops->write = _write_ipc_file;

l_exit:
    return status;
}

int dip_init(void)
{
    STATUS_INIT(status);

    STATUS_ASSIGN(status, _init_syd_ipc());
    if (STATUS_IS_ERROR(status)) {
        goto l_exit;
    }

    STATUS_ASSIGN(status, syd_create(g_syd_ipc));
    if (STATUS_IS_ERROR(status)) {
        goto l_exit;
    }

l_exit:
    return (int) status;
}

void dip_exit(void)
{
    FREE(g_syd_ipc->name);
    FREE(g_syd_ipc);
}
