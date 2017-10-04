obj-m := rtfs.o

CFLAGS_rtfs.o := -DDEBUG

DIR := $(shell pwd)

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(DIR) modules
	
run:
	cd $(DIR)
	sudo insmod ./rtfs.ko
	dmesg | grep rtfs
	sudo rmmod rtfs

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(DIR) clean
