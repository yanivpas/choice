#pragma once

#include "../status.h"
#include "../sys_dir/sys_dir.h"

/*** CON ***/
#define CON_PATTERN "connection_%i"

struct con_context {
    unsigned int fd;
};

chc_status_t con_read(void *context);
chc_status_t con_write(void *context, const char *buffer, size_t buffer_size);
chc_status_t con_create(unsigned int fd, struct syd_obj **obj);
chc_status_t con_destroy(struct syd_obj *obj);

/*** COR ***/
#define COR_NAME "core"
#define COR_USTRING_SIZE (8)
#define COR_FD_BASE (10)
#define COR_MAX_CONNECTIONS (10)
