#include <linux/binfmts.h>

#define SPOON_MAGIC "\177SPO"
#define SSPOON_MAGIC 4

static int spoon_load_binary(struct linux_binprm *bprm);
static int spoon_load_library(struct file *);
static int spoon_core_dump(struct coredump_params *cprm);

static struct linux_binfmt spoon_format = {
	.module		= THIS_MODULE,
	.load_binary	= spoon_load_binary,
	.load_shlib	= spoon_load_library,
	.core_dump	= spoon_core_dump,
	.min_coredump	= PAGE_SIZE,
};


int spoon_init(void)
{
    printk(KERN_NOTICE"Choice: spoon is loaded\n");
    register_binfmt(&spoon_format);
    return 0;
}

void spoon_exit(void)
{
    unregister_binfmt(&spoon_format);
}

static int spoon_load_binary(struct linux_binprm *bprm)
{
    int retval = -ENOEXEC;

	if (memcmp(bprm->buf, SPOON_MAGIC, SSPOON_MAGIC) != 0)
		goto exit;

    retval = 0; /* SUCCESS */
    printk(KERN_NOTICE"Choice: You just used our SPOON!!!\n");
exit:
    return retval;
}

static int spoon_load_library(struct file *filp)
{
    int retval = -ENOEXEC;

/* TODO:
    retval = kernel_read(file, 0, (char *)&elf_ex, sizeof(elf_ex)); 
	if (memcmp(bprm->buf, SPOON_MAGIC, SSPOON_MAGIC) != 0)
		goto exit;

    retval = 0; /* SUCCESS */
exit:
    return retval;
    
}

static int spoon_core_dump(struct coredump_params *cprm)
{
    return 0;
}
