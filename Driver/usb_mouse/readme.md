#书写USB驱动程序

		USB驱动程序框架
		APP：USB 设备驱动程序
kernel------------------------
		USB总线驱动程序
---------------------------------
		  主机控制器
	UHCI OHCI EHCI 
		  
		  
		  
		  
1、make menuconfig 去掉原来的USB驱动
->Device Drivers
	->HID Device
< > USB Human Interface Device (full HID) support 
2、make uImage 并使用新的内核启动
3、insmod .ko
4、在开发板接入拔出USB鼠标

nfs 30000000 192.168.100.9:/work/nfs_root/uImage_nohid;bootm 30000000
网络挂载新的uImage用新内核启动
移动鼠标看效果





