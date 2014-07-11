typedef enum choiced_status_e {
    CHOICE_SUCCESS = 0,
    CHOICE_FORK_FAILED,
    CHOICE_PIPE_FAILED,
    CHOICE_SETSID_FAILD
} choiced_status_t;

typedef enum pipe_indexes_e {
    PIPE_READ = 0,
    PIPE_WRITE
} pipe_indexes_t;

#define CLOSE_PIPE_BOTHWAY(pipe_fds)                \
    close(pipe_fds[PIPE_READ]);                     \
    close(pipe_fds[PIPE_WRITE]);                    \

choiced_status_t fork_me(int pipe_fds[]);
choiced_status_t _daemon_main(int pipe_fds[], pid_t pid);
choiced_status_t _father_process(int pipe_fds[], pid_t pid);
choiced_status_t _child_process(int pipe_fds[]);
