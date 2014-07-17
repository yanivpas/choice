#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/list.h>
#include <linux/vmalloc.h>
#include <linux/string.h>
#include <asm/uaccess.h>

#include "../status.h"
#include "../streamer/streamer.h"

#define SYD_ROOT_PATH "choice"

/* TODO define golbal struct to hold data like this */
static struct proc_dir_entry* syd_root;

struct syd_entry {
    struct syd_obj *obj;
    struct list_head list;
};
static LIST_HEAD(syd_entries);

static int syd_show(struct seq_file *m, void *v)
{
    seq_printf(m, "choice modulce is alive! cmf\n");
    return 0;
}

static int syd_open(struct inode *inode, struct file *file)
{
    return single_open(file, syd_show, NULL);
}

chc_status_t syd_get_obj(struct file *file, syd_obj *obj)
{
    STATUS_INIT(status);
    syd_obj local_obj = {0};
    char *name = NULL;
    struct syd_entry *position = NULL;

    /* TODO: change comparison to some pointer. inode if possible */
    name = file->f_path.dentry->d_iname;
    printk(KERN_INFO "orig: %s\n", name);
    list_for_each_entry(position, &syd_entries, list) {
        printk(KERN_INFO "cur: %s\n", position->obj->name);
        if (0 == strncmp(position->obj->name, name, SYD_MAX_NAME)) {
            local_obj = position->obj;
            break;
        }
    }

    if (NULL == local_obj) {
        STATUS_SET(status, CHC_SYD_FILE_NOT_EXIST);
        goto cleanup;
    }

    STATUS_SET(status, CHC_SUCCESS);
    cleanup:
    if (CHC_SUCCESS == status) {
        *obj = local_obj;
    }

    return status;
}

static ssize_t syd_write(struct file *file, const char *buf, size_t count, loff_t *pos)
{
    STATUS_INIT(status);

    status = syd_get_obj(file);
    if (CHC_SUCCESS != status) {
        goto cleanup;
    }

    STATUS_SET(status, CHC_SUCCESS);
cleanup:
    /* TODO: maybe calculate something? */
    return count;
}

static const struct file_operations syd_fops = {
    .owner = THIS_MODULE,
    .open = syd_open,
    .read = seq_read,
    .write = syd_write,
    .llseek = seq_lseek,
    .release = single_release,
};

chc_status_t syd_create(struct syd_obj obj)
{
    STATUS_INIT(status);
    struct file_operations fops = {0};
    struct syd_entry *new_entry = NULL;
    struct proc_dir_entry *pid_entry;

    pid_entry = proc_create_data(
        obj->name,
        S_IRUGO | S_IWUSR,
        syd_root,
        &syd_fops,
        (void *)SYD_MAGIC);
    if (NULL == pid_entry) {
        STATUS_SET(status, CHC_SYD_PROC_CREATE_DATA);
        goto cleanup;
    }

    new_entry = (struct syd_entry *)vzalloc(sizeof(*new_entry));
    if (NULL == new_entry) {
        STATUS_SET(status, CHC_SYD_VZALLOC);
        goto cleanup;
    }
    new_entry->obj = obj;
    list_add(&(new_entry->list), &syd_entries);

    STATUS_SET(status, CHC_SUCCESS);
cleanup:
    return status;
}

chc_status_t syd_init(void)
{
    STATUS_INIT(status);

    syd_root = proc_mkdir(SYD_ROOT_PATH, 0);
    if (NULL == syd_root) {
        STATUS_SET(status, CHC_SYD_PROC_MKDIR);
        goto cleanup;
    }

    STATUS_SET(status, CHC_SUCCESS);
cleanup:
    return status;
}

void syd_exit(void)
{
    struct syd_entry *position = NULL;
    struct syd_entry *tmp = NULL;

    list_for_each_entry_safe(position, tmp, &syd_entries, list) {
        list_del(&(position->list));
        vfree(position);
    }

    proc_remove(syd_root);
}
