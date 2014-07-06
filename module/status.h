#pragma once

#include <linux/kernel.h>

/* TODO set global */
#define DEBUG 1

#define STATUS_INIT(status) chc_status_t status = CHC_INIT
#ifdef DEBUG
#define STATUS_SET(status, value) {                     \
status = value;                                         \
prints(status, value##_STRING, __FILE__, __LINE__);     \
}
#else
#define STATUS_SET(status, value) status = value
#endif

#define CHC_SUCCESS_STRING "success"
#define CHC_SYD_OVERFLOW_STRING "overflow"
#define CHC_SYD_PROC_MKDIR_STRING "proc_mkdir"
#define CHC_SYD_PROC_CREATE_DATA_STRING "proc_create_data"
#define CHC_SYD_VZALLOC_STRING "vzalloc"
#define CHC_SYD_COPY_FROM_USER_STRING "copy_from_user"
#define CHC_SYD_KSTRTOUL_STRING "kstrtoul"
#define CHC_INIT_STRING "init value"
#define CHC_UNKNOWN_STRING "unknown status value"

typedef enum {
    CHC_SUCCESS = 0,
    CHC_SYD_OVERFLOW,
    CHC_SYD_PROC_MKDIR,
    CHC_SYD_PROC_CREATE_DATA,
    CHC_SYD_VZALLOC,
    CHC_SYD_COPY_FROM_USER,
    CHC_SYD_KSTRTOUL,
    CHC_INIT
} chc_status_t;

#ifdef DEBUG
void prints(chc_status_t status, char *string, char *file, int line)
{
    printk(KERN_ERR "%s;%d: %s\n", file, line, string);
}
#endif
