KERN_DIR = /work/system/linux-3.4.2

all:
	make -C $(KERN_DIR) M=`pwd` modules 

clean:
	make -C $(KERN_DIR) M=`pwd` modules clean
	rm -rf modules.order

obj-m	+=tsl2561_dev.o
obj-m	+=tsl2561_drv.o
