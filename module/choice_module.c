#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include "sys_dir/sys_dir.h"


static int choice_init(void)
{
    printk(KERN_NOTICE"Choice is loaded\n");
    (void)syd_init();
    return 0;
}

static void choice_exit(void)
{
    syd_exit();
}

module_init(choice_init);
module_exit(choice_exit);

MODULE_AUTHOR("Ohad");
MODULE_LICENSE("GPL");
