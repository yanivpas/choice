#include <linux/file.h>
#include <linux/syscalls.h>
#include <linux/fs.h>
#include <linux/kernel.h>

int takeover(unsigned int fd, struct file **filp)
{
    int retval = 0;
    
    *filp = fget(fd);
    if (NULL == filp) {
        retval = -1;
        goto exit;
    }

    retval = sys_close(fd);

exit:
    return retval;
}
