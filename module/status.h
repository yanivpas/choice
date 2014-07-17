#pragma once

#include <linux/kernel.h>

/* TODO set global */
#define DEBUG 1

#define STATUS_INIT(status) chc_status_t status = CHC_INIT
#ifdef DEBUG
#define STATUS_LABEL(status, value) {                   \
status = (value);                                       \
prints(status, value##_STRING, __FILE__, __LINE__);     \
}
#else
#define STATUS_LABEL(status, value) status = (value)
#endif

#define STATUS_ASSIGN(status, value) status = (value)
#define STATUS_IS_SUCCESS(status) (CHC_SUCCESS == status)
#define STATUS_IS_ERROR(status) (!STATUS_IS_SUCCESS(status))

#define CHC_SUCCESS_STRING "success"

#define CHC_SYD_PROC_MKDIR_STRING "proc_mkdir"
#define CHC_SYD_VZALLOC_STRING "vzalloc"
#define CHC_SYD_PROC_CREATE_DATA_STRING "proc_create_data"
#define CHC_SYD_FILE_NOT_EXIST_STRING "no such file"

#define CHC_COR_VZALLOC_STRING "vzalloc"
#define CHC_COR_COPY_FROM_USER_STRING "copy_from_user"
#define CHC_COR_KSTRTOUL_STRING "kstrtoul"
#define CHC_COR_OVERFLOW_STRING "overflow"

#define CHC_INIT_STRING "init value"
#define CHC_UNKNOWN_STRING "unknown status value"
#define CHC_FTO_FGET_STRING "fget failed"
#define CHC_FTO_CLOSE_STRING "close failed"

typedef enum {
    CHC_SUCCESS = 0,

    CHC_SYD_PROC_MKDIR,
    CHC_SYD_VZALLOC,
    CHC_SYD_PROC_CREATE_DATA,
    CHC_SYD_FILE_NOT_EXIST,

    CHC_COR_VZALLOC,
    CHC_COR_COPY_FROM_USER,
    CHC_COR_KSTRTOUL,
    CHC_COR_OVERFLOW,

	CHC_FTO_FGET,
	CHC_FTO_CLOSE,

    CHC_INIT
} chc_status_t;

#ifdef DEBUG
static inline void prints(chc_status_t status, char *string, char *file, int line)
{
    printk(KERN_ERR "%s;%d: %s\n", file, line, string);
}
#endif
