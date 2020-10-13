obj-m := elevator.o

KERNELDIR ?= /lib/modules/$(shell uname -r)/build/
PWD := `pwd`

all:
	make -C $(KERNELDIR) M=$(PWD) modules

clean:
	rm -f *.ko *.o Module* *mod*
