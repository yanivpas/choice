MODULE_NAME = choice_module
KDIR = "/lib/modules/$(shell uname -r)/build"

obj-m += $(MODULE_NAME).o
$(MODULE_NAME)-y := sys_dir/init.o


all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean
