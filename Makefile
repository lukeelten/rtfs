obj-m := rtfs.o

CFLAGS_rtfs.o := -DDEBUG

DIR := $(shell pwd)

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(DIR) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(DIR) clean
