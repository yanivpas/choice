#include <linux/file.h>
#include <linux/syscalls.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/net.h>

#include "status.h"

/* TODO: change the return value to status in all the functions */
/* TODO: Add a documentation to all functions */
chc_status_t takeover(unsigned int fd, struct file **filp)
{
	STATUS_INIT(status);

	*filp = fget(fd);
	if (NULL == filp) {
		STATUS_SET(status, CHC_FTO_FGET);
		goto l_exit;
	}

	if (-1 == sys_close(fd)) {
		STATUS_SET(status, CHC_FTO_CLOSE);
		goto l_exit;
	}

l_exit:
	return status;
}

chc_status_t takeover_socket(unsigned int fd, struct socket **sock)
{
    STATUS_INIT(status);
    struct file *filp = NULL;
	int retval = 0;

    STATUS_ASSIGN(status, takeover(fd, &filp));
    if (STATUS_IS_ERROR(status)) {
        retval = -1;
    }
    *sock = sock_from_file(filp, &retval);

    return status;
}

void takeover_free(struct file *filp)
{
    /* FIXME: should we use this? */
    fput(filp);
}

void takeover_socket_free(struct socket *sock)
{
    sock_release(sock);
}
