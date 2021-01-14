obj-m:=modu.o
CURRENT_PATH:=$(shell pwd)
LINUX_KERNEL:=$(shell uname -r)
LINUX_KERNEL_PATH:=/user/src/linux-headers-$(LINUX_KERNEL)
all:
	make -C /usr/src/linux-headers-5.4.0-47-generic M=$(CURRENT_PATH) modules
clean: 
	make -C /usr/src/linux-headers-5.4.0-47-generic M=$(CURRENT_PATH) clean
