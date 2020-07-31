# Makefile2.6
ifneq ($(KERNELRELEASE),)

obj-m := HookHere.o

#HookDriver-objs := HookHere.o
else
PWD  := $(shell pwd)
KVER ?= $(shell uname -r)
#KDIR := /lib/modules/$(KVER)/build
KDIR := /home/kernel/BiscuitOS/output/linux-5.6-x86_64/linux/linux
all:
	$(MAKE) -C $(KDIR) M=$(PWD)
clean:
	rm -rf .*.cmd *.o *.mod.c *.ko .tmp_versions *.mod modules.order Module.symvers built-in.a
endif
