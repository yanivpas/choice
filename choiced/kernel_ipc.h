#include <stdbool.h>

#define KERNEL_IPC_FILE_PATH "/proc/choice/dip"

bool is_choice_running(void);
choiced_status_t write_pid(pid_t pid);
