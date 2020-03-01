块设备驱动程序框架

APP：open read write “1.txt”
-----------------------------文件的读写
文件系统：NTFS EXT4 FAT32   (把文件的读写转化为扇区的速写读写）
----------ll_rw_block--------扇区的读写
				1、把读写放入队列
				2、调用队列的处理函数（优化、条顺序、合并）
				
	块设备驱动程序
-------------------------------
硬件：硬盘，flash


步骤
1、分配 gendisk:alloc_disk
2、设置
2.1、分配/设置队列：request_queue_t  //它提供读写能力
		blk_init_queue
2.2、设置gendisk其他信息  //它提供属性：比如容量
3、注册add_disk


测试：

1 insmod    *.ko
2 格式化 ： mkdosfs /dev/ramblock
3 挂接 :  mount /dev/ramblock  /tmp/
4 读写文件  cd /tmp/ 在里面读写文件
5 cd / ; umount /tmp/
6 /dev/ramblock > /mut/ramblock.bin
7 在PC 上查看 ramblock.bin
 sudo mount -o loop ramblock.bin /mnt 












