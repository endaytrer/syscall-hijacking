CC = gcc
LD = gcc
CFLAGS = -g -O0

obj-m = hello.o

.PHONY: all clean


all: hello.ko
hello.ko:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean