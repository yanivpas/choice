#include <linux/kernel.h>
#include <linux/vmalloc.h>

#include "../status.h"
#include "../sys_dir/sys_dir.h"
#include "core.h"
#include "internal.h"

chc_status_t con_read(void *context)
{
    STATUS_INIT(status);
    struct con_context *local_context = NULL;

    if (NULL == context) {
        STATUS_LABEL(status, CHC_CON_INVALID);
        goto cleanup;
    }

    local_context = (struct con_context *)context;

    /* TODO */

    STATUS_LABEL(status, CHC_SUCCESS);
cleanup:
    return status;
}

chc_status_t con_write(void *context, const char *buffer, size_t buffer_size)
{
    STATUS_INIT(status);
    struct con_context *local_context = NULL;

    if ((NULL == context) || (NULL == buffer)) {
        STATUS_LABEL(status, CHC_CON_INVALID);
        goto cleanup;
    }

    local_context = (struct con_context *)context;

    /* TODO */

    STATUS_LABEL(status, CHC_SUCCESS);
cleanup:
    return status;
}

static struct syd_ops g_con_ops = {
    .read = con_read,
    .write = con_write,
};

/* TODO: don't malloc name - on the stack */
chc_status_t con_create(unsigned int fd, struct syd_obj **obj)
{
    STATUS_INIT(status);
    int snprintf_result = 0;
    char *name = NULL;
    struct con_context *context = NULL;
    struct syd_obj *local_obj = NULL;

    if (NULL == obj) {
        STATUS_LABEL(status, CHC_CON_INVALID);
        goto cleanup;
    }

    context = (struct con_context *)vzalloc(sizeof(*context));
    if (NULL == context) {
        STATUS_LABEL(status, CHC_CON_VZALLOC);
        goto cleanup;
    }
    context->fd = fd;

    name = (char *)vzalloc(sizeof(COR_MAX_NAME));
    if (NULL == name) {
        STATUS_LABEL(status, CHC_CON_VZALLOC);
        goto cleanup;
    }
    /* TODO: validate null termination */
    snprintf_result = snprintf(name, COR_MAX_NAME, CON_PATTERN, fd);
    if (0 > snprintf_result) {
        STATUS_LABEL(status, CHC_CON_SNPRINTF);
        goto cleanup;
    }

    local_obj = (struct syd_obj *)vzalloc(sizeof(*local_obj));
    if (NULL == local_obj) {
        STATUS_LABEL(status, CHC_CON_VZALLOC);
        goto cleanup;
    }

    STATUS_ASSIGN(status, syd_create(name, context, &g_con_ops));
    if (STATUS_IS_ERROR(status)) {
        goto cleanup;
    }

    STATUS_LABEL(status, CHC_SUCCESS);
cleanup:
    if (STATUS_IS_SUCCESS(status)) {
        *obj = local_obj;
    } else {
        if (NULL != local_obj) {
            vfree(local_obj);
        }
        if (NULL != context) {
            vfree(context);
        }
        if (NULL != name) {
            vfree(name);
        }
    }

    return status;
}

chc_status_t con_destroy(struct syd_obj *obj)
{
    STATUS_INIT(status);

    if (NULL == obj) {
        STATUS_LABEL(status, CHC_CON_INVALID);
        goto cleanup;
    }

    vfree(obj->ops);
    vfree(obj->name);
    vfree(obj->context);
    vfree(obj);

    STATUS_LABEL(status, CHC_SUCCESS);
cleanup:
    return status;
}
