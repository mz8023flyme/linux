Flash 驱动框架
	app : open write read 
------------------------------
VFS sysopen syswrite sysread 
------------------------------
FS
------------------------------
字符设备 |  EXT2  YAFFS	|
	     |-------------------
	     |		块设备
	     |--------------------
	     |RAMBLOCK|硬盘|   MTD
		 |		  |    |--------------|
		 |		  |	   |NAND|   NOR   |
		 |		  |    |协议|---------|
		 |		  |    |    |jedec|cid|

		 
		 
测试 通过配置内核
 1).通过配置内核支持NOR FLASH
 --> Device Drivers
     --> Memory Technology Device (MTD) support 
         --> Mapping drivers fro chip access
         <M> CFI Flash device in physical memory map
         (0x0) Physical start address of flash mapping     //起始地址
         (0x1000000) Physical length of flash mapping    //内存大小，16M，要大于实际的大小
         (2) Bank owidth in octets                        //位宽为16位
 2).make modules
 cp drivers/mtd/maps/physmap.ko  /home/study/nfs_home    
  


