
int takeover(unsigned int fd, struct file **filp);
int takeover_free(struct file *filp);
int takeover_socket(unsigned int fd, struct socket **sock);
int takeover_socket_free(struct socket *sock);
