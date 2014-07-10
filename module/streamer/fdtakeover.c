#include <linux/file.h>
#include <linux/syscalls.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/net.h>

/* TODO: change the return value to status in all the functions */
/* TODO: Add a documentation to all functions */
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

void takeover_free(struct file *filp)
{
    /* FIXME: should we use this? */
}

int takeover_socket(unsigned int fd, struct socket **sock)
{
    int retval = 0;
    struct file *filp = NULL;

    retval = takeover(fd, &filp);
    *sock = sock_from_file(filp, &retval);
    
    return retval;
}

void takeover_socket_free(struct socket *sock)
{
    /* TODO */
}
