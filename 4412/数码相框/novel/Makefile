
CROSS_COMPILE = arm-linux-
AS		= $(CROSS_COMPILE)as
LD		= $(CROSS_COMPILE)ld
CC		= $(CROSS_COMPILE)gcc
CPP		= $(CC) -E
AR		= $(CROSS_COMPILE)ar
NM		= $(CROSS_COMPILE)nm

STRIP		= $(CROSS_COMPILE)strip
OBJCOPY		= $(CROSS_COMPILE)objcopy
OBJDUMP		= $(CROSS_COMPILE)objdump

TOPDIR := $(PWD)
# export导出的变量是给子目录下的Makefile使用的
export AS LD CC CPP AR NM STRIP OBJCOPY OBJDUMP TOPDIR 

CFLAGS := -Wall -O2 -g
CFLAGS += -I $(PWD)/include 

LDFLAGS := -lm -lfreetype

export CFLAGS LDFLAGS


obj-y += main.o 
obj-y += display/
obj-y += draw/ 
obj-y += encoding/
obj-y += fonts/ 

TARGET = novel

all:
	make -C ./ -f $(TOPDIR)/Makefile.build
	$(CC) $(LDFLAGS) -o $(TARGET) built-in.o

clean:
	rm -rf $(shell find -name "*.o")
	rm -rf $(TARGET)

disclean:
	rm -rf $(shell find -name "*.o")
	rm -rf $(shell find -name "*.d")
	rm -rf $(TARGET)
