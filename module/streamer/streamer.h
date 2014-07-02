
int takeover(unsigned int fd, struct file **filp);
void takeover_free(struct file *filp);
int takeover_socket(unsigned int fd, struct socket **sock);
void takeover_socket_free(struct socket *sock);
