MODULE_NAME = choice
KDIR = "/lib/modules/$(shell uname -r)/build"

obj-m += $(MODULE_NAME).o
$(MODULE_NAME)-objs := choice_module.o streamer/fdtakeover.o streamer/streamer.o sys_dir/init.o daemon_ipc/ipc.o


all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean
