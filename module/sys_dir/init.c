#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

static struct proc_dir_entry* sys_dir;

static ssize_t sys_dir_write(struct file *file, const char *buf, size_t count, loff_t *pos)
{
    //TODO
    return 0;
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


int sys_dir_init(void)
{
    sys_dir = proc_create("choice_module", 0, NULL, &sys_dir_fops);

    if (!sys_dir) {
        return -ENOMEM;
    }

    return 0;
}

void  sys_dir_exit(void)
{
    remove_proc_entry("choice_module", NULL);
}
