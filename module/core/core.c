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
#include "internal.h"

struct cor_entry {
    struct syd_obj *connection;
    struct list_head list;
};
static LIST_HEAD(cor_list);

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
        *fd = (unsigned int) local_fd;
    }

    return status;
}

static chc_status_t cor_add_connection(unsigned int fd)
{
    STATUS_INIT(status);
    struct cor_entry *entry = NULL;
    struct syd_obj *connection = NULL;

    STATUS_ASSIGN(status, con_create(fd, &connection));
    if (STATUS_IS_ERROR(status)) {
        goto cleanup;
    }

    entry = (struct cor_entry *)vzalloc(sizeof(*entry));
    if (NULL == entry) {
        STATUS_LABEL(status, CHC_COR_VZALLOC);
        goto cleanup;
    }
    entry->connection = connection;

    STATUS_LABEL(status, CHC_SUCCESS);
cleanup:
    if (STATUS_IS_SUCCESS(status)) {
        list_add(&(entry->list), &cor_list);
    } else {
        if (NULL != connection) {
            (void)con_destroy(connection);
        }
        if (NULL != entry) {
            vfree(entry);
        }
    }
    return status;
}

chc_status_t cor_write(void *context, const char *buffer, size_t buffer_size)
{
    STATUS_INIT(status);
    unsigned int fd = 0;

    /* TODO: takeover fd */
    STATUS_ASSIGN(status, cor_parse_fd(buffer, buffer_size, &fd));
    if (STATUS_IS_ERROR(status)) {
        goto cleanup;
    }

    STATUS_ASSIGN(status, cor_add_connection(fd));
    if (STATUS_IS_ERROR(status)) {
        goto cleanup;
    }

    STATUS_LABEL(status, CHC_SUCCESS);
cleanup:
    return status;
}

static struct syd_ops g_cor_ops = {
    .read = cor_read,
    .write = cor_write,
};

int cor_init(void)
{
    STATUS_INIT(status);

    STATUS_ASSIGN(status, syd_create(COR_NAME, NULL, &g_cor_ops));
    if (STATUS_IS_ERROR(status)) {
        goto cleanup;
    }

    STATUS_LABEL(status, CHC_SUCCESS);
cleanup:
    return status;
}

void cor_exit(void)
{
    struct cor_entry *position = NULL;
    struct cor_entry *tmp = NULL;

    list_for_each_entry_safe(position, tmp, &cor_list, list) {
        list_del(&(position->list));
        con_destroy(position->connection);
        vfree(position);
    }
}
