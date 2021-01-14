ifneq ($(KERNELRELEASE),)
	obj-m := globalvar.o
#obj-m 指编译成外部模块
else
	KERNELDIR := /lib/modules/$(shell uname -r)/build

	PWD := $(shell pwd)
modules:
	make -C $(KERNELDIR) M=$(PWD) modules	
endif
clean:
	make -C $(KERNELDIR) M=$(PWD) clean
