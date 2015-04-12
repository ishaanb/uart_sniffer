wake_line_kobject-objs := led_interrupt.o
KERNELDIR ?= /lib/modules/$(shell uname -r)/build 
PWD := $(shell pwd)

obj-m = wake_line_kobject.o

build: 
	make -C $(KERNELDIR) M=${PWD} modules

clean: 
	make -C ${KERNELDIR} M=${PWD} clean
