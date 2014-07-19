#include <linux/vmalloc.h>
#include "../sys_dir/sys_dir.h"
#include "../status.h"
#include "dip.h"

static struct syd_obj g_syd_ipc = {
    .context = NULL,
    .name = DAEMON_IPC_FILE,
    .ops = NULL,
};

chc_status_t _read_ipc_file(void * context)
{
    STATUS_INIT(status);
    STATUS_LABEL(status, CHC_SUCCESS);
    return status;
}

chc_status_t _write_ipc_file(
        void *context,
        const char *buffer,
        size_t buffer_size)
{

    STATUS_INIT(status);
    STATUS_LABEL(status, CHC_SUCCESS);
    return status;
}

chc_status_t _init_syd_ipc(void)
{
    STATUS_INIT(status);

    g_syd_ipc.ops = (struct syd_ops *)vzalloc(sizeof(*g_syd_ipc.ops));
    if (NULL == g_syd_ipc.ops) {
        STATUS_LABEL(status, CHC_DIP_VZALLOC);
        goto l_exit;
    }

    g_syd_ipc.ops->read = _read_ipc_file;
    g_syd_ipc.ops->write = _write_ipc_file;

    STATUS_LABEL(status, CHC_SUCCESS);

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

    STATUS_ASSIGN(status, syd_create(&g_syd_ipc));
    if (STATUS_IS_ERROR(status)) {
        goto l_exit;
    }

    STATUS_LABEL(status, CHC_SUCCESS);

l_exit:
    return (int) status;
}

void dip_exit(void)
{
    FREE(g_syd_ipc.ops);
}
