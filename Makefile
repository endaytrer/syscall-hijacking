MOD_C_SRCS := $(wildcard *.c)
MOD_C_SRCS := $(filter-out $(wildcard *.mod.c), $(MOD_C_SRCS))
MOD_C_HDRS := $(wildcard *.h)
MOD_ASM_SRCS := $(wildcard *.S)

MOD_C_OBJS := $(patsubst %.c, %.o, $(MOD_C_SRCS))
MOD_ASM_OBJS := $(patsubst %.S, %.o, $(MOD_ASM_SRCS))


modhello-objs := entry.o netmodify.o hook.o mysyscall.o
obj-m := modhello.o


.PHONY: all install uninstall clean

all: modhello.ko

modhello.ko: $(MOD_C_SRCS) $(MOD_C_HDRS) $(MOD_ASM_SRCS)
	make clean
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules

install: modhello.ko
	insmod $^
	
uninstall:
	rmmod modhello.ko

clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean