#define DAEMON_IPC_FILE "tmp"
#define FREE(addr) {                    \
    if (NULL != (addr)) {               \
        vfree(addr);                    \
        addr = NULL;                    \
    }                                   \
}

int dip_init(void);
void dip_exit(void);
