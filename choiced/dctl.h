#define CLOSE(fd)                           \
    if (-1 != fd) {                         \
        close(fd);                          \
    }

#define STATUS_INIT(status) choiced_status_t status = CHOICE_INIT
#define STATUS_LABEL(status, value) status = (value)
#define STATUS_ASSIGN(status, value) status = (value)
#define STATUS_IS_SUCCESS(status) (CHOICE_SUCCESS == status)
#define STATUS_IS_ERROR(status) (!STATUS_IS_SUCCESS(status))

typedef enum {
    CHOICE_INIT = -1,
    CHOICE_SUCCESS = 0,
    CHOICE_NULL_ARGUMENT,
    CHOICE_FORK_FAILED,
    CHOICE_PIPE_FAILED,
    CHOICE_SETSID_FAILD,
    CHOICE_NOT_RUNNING,
    CHOICE_OPEN_FAILED,
    CHOICE_WRITE_FAILED
} choiced_status_t;
