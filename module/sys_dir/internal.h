#pragma once

#define SYD_MAGIC (0x10)
#define SYD_ROOT_PATH "choice"

chc_status_t syd_obj_create(char *name, void *context, struct syd_ops *ops, struct syd_obj **obj);
chc_status_t syd_obj_destroy(struct syd_obj *obj);
static int syd_open(struct inode *inode, struct file *file);
static ssize_t syd_write(struct file *file, const char *buf, size_t count, loff_t *pos);
static int syd_show(struct seq_file *file, void *p);
