KDIR := /lib/modules/$(shell uname -r)/build

obj-m += netlinkKernel.o

all:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
	gcc netlinkUser.c -o netlinkUser

clean:
	rm -rf *.o *.ko *.mod.* *.cmd .module* modules* Module* .*.cmd .tmp*
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

run:
	sudo insmod netlinkKernel.ko
	./netlinkUser

stop:
	sudo rmmod netlinkKernel
