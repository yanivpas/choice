#pragma once

#include "../status.h"
#include "../sys_dir/sys_dir.h"

#define CON_PATTERN "connection_%i"

struct con_context {
    unsigned int fd;
};

chc_status_t con_read(void *context);
chc_status_t con_write(void *context, const char *buffer, size_t buffer_size);
chc_status_t con_create(unsigned int fd, struct syd_obj **obj);
chc_status_t con_destroy(struct syd_obj *obj);
