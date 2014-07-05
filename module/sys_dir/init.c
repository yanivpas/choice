#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/list.h>
#include <linux/vmalloc.h>
#include <asm/uaccess.h>

#include "../status.h"
#include "../streamer/streamer.h"

#define SYD_ROOT_PATH "choice"
#define SYD_PID_PATH "pid"
#define SYD_MAGIC 0x1
#define SYD_USTRING_SIZE 8
#define SYD_FD_BASE 10

/* TODO define golbal struct to hold data like this */
static struct proc_dir_entry* sys_dir_root;
struct connection {
    unsigned int fd;
    struct list_head list;
};
static LIST_HEAD(connections_list);

static chc_status_t sys_dir_parse_fd(
        const char *ustring,
        size_t ustring_size,
        unsigned int *fd)
{
    chc_status_t status = CHC_INIT;
    /* adding 1 to promise null termination */
    char kstring[SYD_USTRING_SIZE + 1] = {0};
    unsigned long uncopied_bytes = 0;
    int parse_return_code = 0;
    unsigned long local_fd = 0;

    if (SYD_USTRING_SIZE < ustring_size) {
        SET_STATUS(status, CHC_SYD_OVERFLOW);
        goto cleanup;
    }

    uncopied_bytes = copy_from_user(kstring, ustring, ustring_size);
    if (0 != uncopied_bytes) {
        SET_STATUS(status, CHC_SYD_COPY_FROM_USER);
        goto cleanup;
    }

    parse_return_code = kstrtoul(kstring, SYD_FD_BASE, &local_fd);
    if (0 != parse_return_code) {
        SET_STATUS(status, CHC_SYD_KSTRTOUL);
        goto cleanup;
    }

cleanup:
    if (CHC_SUCCESS == status) {
        *fd = (unsigned long) local_fd;
    }

    return status;
}

static ssize_t sys_dir_write(struct file *file, const char *buf, size_t count, loff_t *pos)
{
    chc_status_t status = CHC_INIT;
    unsigned int parsed_fd = 0;
    struct connection *new_connection = NULL;

    status = sys_dir_parse_fd(buf, count, &parsed_fd);
    if (CHC_SUCCESS != status) {
        goto cleanup;
    }

    printk("%d\n", parsed_fd);

    /* TODO: add connection and vfree at exit
     * new_connection = (struct connection *)vzalloc(sizeof(*new_connection));
     * if (NULL != new_connection) {
     *     SET_STATUS(status, CHC_SYD_VZALLOC);
     *     goto cleanup;
     * }

     * connections_list.
     */

cleanup:
    if (CHC_SUCCESS != status) {
        if (NULL != new_connection) {
            vfree(new_connection);
        }
    }

    return count;
}

static int sys_dir_show(struct seq_file *m, void *v)
{
    seq_printf(m, "choice modulce is alive! cmf\n");
    return 0;
}

static int sys_dir_open(struct inode *inode, struct file *file)
{
    return single_open(file, sys_dir_show, NULL);
}

static const struct file_operations sys_dir_fops = {
    .owner = THIS_MODULE,
    .open = sys_dir_open,
    .read = seq_read,
    .write = sys_dir_write,
    .llseek = seq_lseek,
    .release = single_release,
};

/*** public functions ***/
int sys_dir_init(void)
{
    chc_status_t status = CHC_INIT;
    struct proc_dir_entry *pid_entry;

    sys_dir_root = proc_mkdir(SYD_ROOT_PATH, 0);
    if (!sys_dir_root) {
        SET_STATUS(status, CHC_SYD_PROC_MKDIR);
        goto cleanup;
    }

    pid_entry = proc_create_data(
        SYD_PID_PATH,
        S_IRUGO|S_IWUSR,
        sys_dir_root,
        &sys_dir_fops,
        /* TODO understand 'data' parameter, now it's magic... */
        (void *)SYD_MAGIC);
    if (!pid_entry) {
        SET_STATUS(status, CHC_SYD_PROC_CREATE_DATA);
        goto cleanup;
    }

    status = CHC_SUCCESS;
cleanup:
    return status;
}

void sys_dir_exit(void)
{
    /* struct list_head *position = NULL;
     *
     * list_for_each(position, &connections_list) {
     *     position
     * }
     */

    proc_remove(sys_dir_root);
}
