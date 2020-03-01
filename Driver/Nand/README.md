# Nand FLASH
Nand FLASH 驱动框架

app : open read write 
------------------------ 
fs: ntfs fat32 ext3
------------------------
块设备驱动
Nand FLASH 协议： 知道发什么   来读写擦除
------------------------
硬件相关的代码 ：知道怎样发命令/地址  读写数据
------------------------
硬件



set bootargs root =  /dev/nfs nfsroot=192.168.100.9:/work/nfs_root/first_fs ip=192.168.100.17:192.168.100.9:192.168.100.1:255.255.255.0::eth0:off





























