obj-m += overlord.o
overlord-objs := main.o idt.o syscall.o msr.o
ccflags-y := -std=gnu99 -Wno-declaration-after-statement

all:
	sudo make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	sudo make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
