#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

static struct proc_dir_entry* sys_dir;

static const struct file_operations sys_dir_fops = {
    .owner = THIS_MODULE,
    .open = sys_dir_open,
    .read = sys_dir_read,
    .write = sys_dir_write,
};

static ssize_t sys_dir_read(struct file *file, char __user *, size_t, loff_t *)
{
    //TODO
    return 0;
}

static int sys_dir_write(struct file *file, const char *buf, size_t count, loff_t *pos)
{
    //TODO
    return 0;
}

static int sys_dir_show(struct seq_file, *m, void *v)
{
    seq_printf(m, "choice modulce is alive! cmf");
    return 0;
}

static int sys_dir_open(struct inode *inode, struct file *file)
{
    return single_open(file, sys_dir_show, NULL);
}

int sys_dir_init(void)
{
    choice_file = proc_create("choice_module", 0, NULL, &choice_fops);

    if (!choice_fops) {
        return -ENOMEM;
    }

    return 0;
}

void  sys_dir_exit(void)
{
    remove_proc_entry("choice_module", NULL);
}
