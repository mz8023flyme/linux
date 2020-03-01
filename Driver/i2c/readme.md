写i2c驱动程序

1.分配一个 i2c_drive结构体
2.设置
	attach_adapter//它直接调用i2c_prob （adap,设备地址，发现这个设备后要调用的设备）
	detach_client //卸载这个驱动后，如果之前发现能够支持的设备。则调用









