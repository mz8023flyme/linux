网卡驱动框架：
------------------
APP :  socket
----------------
--------------- 若干层网络协议
---------------------
hard_start_xmit   /\
		||		  ||
		\/     netif_rx
-------------------------  sk_buff
硬件相关的驱动程序(要提供hard_start_xmit，有数据时要用netif_rx上报)
-------------------------
		硬件
写程序
/*1.分配一个net_drivce结构体*/
/*2.设置*/
2,1 发包函数 hard_start_xmit
2,2 收到数据时 (在中断里面netif_rx)上报数据
3,3 其他设置
/*3.注册*/
register_netdev

测试
insmod vrit_net,ko
ifconfig vnet0 3.3.3.3
ping 3.3.3.3
ping 3.3.3.4










