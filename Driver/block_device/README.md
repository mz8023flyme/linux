���豸����������

APP��open read write ��1.txt��
-----------------------------�ļ��Ķ�д
�ļ�ϵͳ��NTFS EXT4 FAT32   (���ļ��Ķ�дת��Ϊ��������д��д��
----------ll_rw_block--------�����Ķ�д
				1���Ѷ�д�������
				2�����ö��еĴ��������Ż�����˳�򡢺ϲ���
				
	���豸��������
-------------------------------
Ӳ����Ӳ�̣�flash


����
1������ gendisk:alloc_disk
2������
2.1������/���ö��У�request_queue_t  //���ṩ��д����
		blk_init_queue
2.2������gendisk������Ϣ  //���ṩ���ԣ���������
3��ע��add_disk


���ԣ�

1 insmod    *.ko
2 ��ʽ�� �� mkdosfs /dev/ramblock
3 �ҽ� :  mount /dev/ramblock  /tmp/
4 ��д�ļ�  cd /tmp/ �������д�ļ�
5 cd / ; umount /tmp/
6 /dev/ramblock > /mut/ramblock.bin
7 ��PC �ϲ鿴 ramblock.bin
 sudo mount -o loop ramblock.bin /mnt 












