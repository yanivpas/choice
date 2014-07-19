#define CLOSE_PIPE_BOTHWAY(pipe_fds)                \
    close(pipe_fds[PIPE_READ]);                     \
    close(pipe_fds[PIPE_WRITE]);                    \

typedef enum pipe_indexes_e {
    PIPE_READ = 0,
    PIPE_WRITE
} pipe_indexes_t;

static choiced_status_t _daemon_main(int pipe_fds[], pid_t pid);
static choiced_status_t _father_process(int pipe_fds[], pid_t pid);
static choiced_status_t _child_process(int pipe_fds[]);
choiced_status_t fork_me(int pipe_fds[]);
