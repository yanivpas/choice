#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/list.h>
#include <linux/vmalloc.h>
#include <asm/uaccess.h>

#include "../status.h"
#include "../streamer/streamer.h"
#include "../sys_dir/sys_dir.h"
#include "core.h"

#define COR_USTRING_SIZE (8)
#define COR_FD_BASE (10)

static struct syd_obj *g_cor_syd = NULL;

struct connection {
    unsigned int fd;
    struct list_head list;
};
static LIST_HEAD(connections_list);

chc_status_t cor_read(void *context)
{
    return 0;
}

static chc_status_t cor_parse_fd(const char *ustring, size_t ustring_size, unsigned int *fd)
{
    STATUS_INIT(status);
    /* adding 1 to guarantee null termination */
    char kstring[COR_USTRING_SIZE + 1] = {0};
    unsigned long uncopied_bytes = 0;
    int parse_return_code = 0;
    unsigned long local_fd = 0;

    if (COR_USTRING_SIZE < ustring_size) {
        STATUS_LABEL(status, CHC_COR_OVERFLOW);
        goto cleanup;
    }

    uncopied_bytes = copy_from_user(kstring, ustring, ustring_size);
    if (0 != uncopied_bytes) {
        STATUS_LABEL(status, CHC_COR_COPY_FROM_USER);
        goto cleanup;
    }

    parse_return_code = kstrtoul(kstring, COR_FD_BASE, &local_fd);
    if (0 != parse_return_code) {
        STATUS_LABEL(status, CHC_COR_KSTRTOUL);
        goto cleanup;
    }

    STATUS_LABEL(status, CHC_SUCCESS);
cleanup:
    if (STATUS_IS_SUCCESS(status)) {
        *fd = (unsigned long) local_fd;
    }

    return status;
}

chc_status_t cor_write(void *context, const char *buffer, size_t buffer_size)
{
    STATUS_INIT(status);
    unsigned int parsed_fd = 0;
    struct connection *new_connection = NULL;

    /* TODO: takeover fd and create its own file */
    STATUS_ASSIGN(status, cor_parse_fd(buffer, buffer_size, &parsed_fd));
    if (STATUS_IS_ERROR(status)) {
        goto cleanup;
    }

    new_connection = (struct connection *)vzalloc(sizeof(*new_connection));
    if (NULL == new_connection) {
        STATUS_LABEL(status, CHC_COR_VZALLOC);
        goto cleanup;
    }
    new_connection->fd = parsed_fd;

    list_add(&(new_connection->list), &connections_list);

    STATUS_LABEL(status, CHC_SUCCESS);
cleanup:
    if (STATUS_IS_ERROR(status)) {
        if (NULL != new_connection) {
            vfree(new_connection);
        }
    }

    return status;
}

int cor_init(void)
{
    STATUS_INIT(status);

    g_cor_syd = (struct syd_obj *)vzalloc(sizeof(*g_cor_syd));
    if (NULL == g_cor_syd) {
        STATUS_LABEL(status, CHC_COR_VZALLOC);
        goto cleanup;
    }
    g_cor_syd->context = NULL;
    g_cor_syd->name = COR_NAME;
    g_cor_syd->ops->read = cor_read;
    g_cor_syd->ops->write = cor_write;

    STATUS_ASSIGN(status, syd_create(g_cor_syd));
    if (STATUS_IS_ERROR(status)) {
        goto cleanup;
    }

    STATUS_LABEL(status, CHC_SUCCESS);
cleanup:
    return status;
}

void cor_exit(void)
{
    struct connection *position = NULL;
    struct connection *tmp = NULL;

    list_for_each_entry_safe(position, tmp, &connections_list, list) {
        list_del(&(position->list));
        vfree(position);
    }

    vfree(g_cor_syd);
}
