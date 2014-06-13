#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include "sys_dir/sys_dir.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ohad");

static int choice_init(void)
{
    (void)sys_dir_init();
    return 0;
}

static void choice_cleanup(void)
{}

module_init(choice_init);
module_exit(choice_cleanup);