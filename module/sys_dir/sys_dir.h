#pragma once

#include "../status.h"

#define SYD_MAX_NAME (10)

struct syd_ops {
    chc_status_t (*read) (void *context);
    chc_status_t (*write) (void *context, const char *buffer, size_t buffer_size);
};

struct syd_obj {
    void *context;
    char *name;
    struct syd_ops *ops;
};

/* destroyed at syd_exit */
chc_status_t syd_create(char *name, void *context, struct syd_ops *ops);
int syd_init(void);
void syd_exit(void);
