#include <linux/kernel.h>

#ifdef DEBUG
#define SET_STATUS(status, value) {     \
status = value;                         \
prints(status, __FILE__, __LINE__);     \
}
#else
#define SET_STATUS(status, value) status = value
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

void prints(chc_status_t status, char * file, int line)
{
    char * string = NULL;

    switch(status) {
    case CHC_SUCCESS:
        string = CHC_SUCCESS_STRING;
        break;

    case CHC_SYD_OVERFLOW:
        string = CHC_SYD_OVERFLOW_STRING;
        break;

    case CHC_SYD_PROC_MKDIR:
        string = CHC_SYD_PROC_MKDIR_STRING;
        break;

    case CHC_SYD_PROC_CREATE_DATA:
        string = CHC_SYD_PROC_CREATE_DATA_STRING;
        break;

    case CHC_SYD_VZALLOC:
        string = CHC_SYD_VZALLOC_STRING;
        break;

    case CHC_SYD_COPY_FROM_USER:
        string = CHC_SYD_COPY_FROM_USER_STRING;
        break;

    case CHC_SYD_KSTRTOUL:
        string = CHC_SYD_KSTRTOUL_STRING;
        break;

    case CHC_INIT:
        string = CHC_INIT_STRING;
        break;

    default:
        string = CHC_UNKNOWN_STRING;
    }

    printk(KERN_ERR ";%s;%d: %s\n", file, line, string);
}
