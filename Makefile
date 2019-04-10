obj-m += overlord.o
overlord-objs := main.o idt.o syscall.o msr.o

all:
	sudo make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	sudo make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
