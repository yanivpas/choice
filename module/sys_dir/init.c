#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

/*** defines ***/
#define SYS_DIR_ROOT_PATH "choice"
#define SYS_DIR_PID_PATH "pid"
#define SYS_DIR_MAGIC 0x1

/*** enums ***/
typedef enum {
    SYS_DIR_STATUS_SUCCESS = 0,

    SYS_DIR_STATUS_PROC_MKDIR_FAILED,
    SYS_DIR_STATUS_PROC_CREATE_DATA_FAILED,
    
    SYS_DIR_STATUS_INIT
} sys_dir_status_t;

/*** structs ***/
// TODO define golbal struct to hold data like this
static struct proc_dir_entry* sys_dir_root;

/*** private functions ***/
static ssize_t sys_dir_write(struct file *file, const char *buf, size_t count, loff_t *pos)
{
    //TODO
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

//TODO get pid of calling process (task_tgid_vnr from include/linux/sched.h)
//FIXME we might want to use pid_nr from include/linux/pid.h

/*** public functions ***/
int sys_dir_init(void)
{
    sys_dir_status_t status = SYS_DIR_STATUS_INIT;
    struct proc_dir_entry *pid_entry;

    sys_dir_root = proc_mkdir(SYS_DIR_ROOT_PATH, 0);
    if (!sys_dir_root) {
        status = SYS_DIR_STATUS_PROC_MKDIR_FAILED;
        goto cleanup;
    }

    pid_entry = proc_create_data(
        SYS_DIR_PID_PATH,
        S_IRUGO|S_IWUSR,
        sys_dir_root,
        &sys_dir_fops,
        //TODO understand 'data' parameter, now it's magic...
        (void *)SYS_DIR_MAGIC);
    if (!pid_entry) {
        status = SYS_DIR_STATUS_PROC_CREATE_DATA_FAILED;
        goto cleanup;
    }

    status = SYS_DIR_STATUS_SUCCESS;
cleanup:
    return status;
}

void sys_dir_exit(void)
{
    proc_remove(sys_dir_root);
}
