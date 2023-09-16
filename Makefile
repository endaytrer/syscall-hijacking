obj-m = modhello.o
modhello-objs = hello.o hook.o

.PHONY: all install uninstall clean


all: modhello.ko

modhello.ko: hello.c hook.S
	make clean
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules

install: modhello.ko
	insmod $^
	
uninstall: modhello.ko
	rmmod $^

clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean