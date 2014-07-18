#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include "sys_dir/sys_dir.h"
#include "core/core.h"
#include "daemon_ipc/dip.h"
#include "spoon/spoon.h"


static int choice_init(void)
{
    syd_init();
    cor_init();
    dip_init();
    spo_init();

    printk(KERN_NOTICE"Choice is loaded\n");
    return 0;
}

static void choice_exit(void)
{
    dip_exit();
    cor_exit();
    syd_exit();
    spo_exit();
}

module_init(choice_init);
module_exit(choice_exit);

MODULE_AUTHOR("Ohad");
MODULE_LICENSE("GPL");
