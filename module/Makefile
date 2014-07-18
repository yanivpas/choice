MODULE_NAME = choice
KDIR = "/lib/modules/$(shell uname -r)/build"

obj-m += $(MODULE_NAME).o
$(MODULE_NAME)-objs := choice_module.o streamer/fdtakeover.o streamer/streamer.o sys_dir/sys_dir.o core/core.o daemon_ipc/dip.o spoon/binfmt_spoon.o


all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean
