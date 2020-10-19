obj-m += my_timer.o

KERNELDIR ?= /lib/modules/$(shell uname -r)/build/
PWD := `pwd`
all:
	make -C $(KERNELDIR) M=$(PWD) modules
clean:
	make -C $(KERNELDIR) M=$(PWD) clean
