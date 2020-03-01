app:   open("/dev/rtc")
------------------------------
kernel sysopen 
		rtc_dev_fops->open
			rtc_dev_open
				//根据次设备号找到以前用"rtc_device_register"注册的rtc_device
					s3c_rtc->open
app: 	ioctl
------------------------------------
kernel sys_ioctl 
	rtc_dev_fops->read 
		rtc_dev_read






 