typedef enum pipe_indexes_e {
    PIPE_READ = 0,
    PIPE_WRITE
} pipe_indexes_t;

static void _wait(void);
static choiced_status_t _daemon_main(pid_t pid);
static choiced_status_t _father_process(pid_t pid);
static choiced_status_t _child_process(void);
choiced_status_t fork_me(void);
