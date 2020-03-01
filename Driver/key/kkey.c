#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <asm/arch/regs-gpio.h>
#include <asm/hardware.h>

static struct class *keys_class;
static struct class_device	*keys_class_devs;
//static struct class_device	*keys_class_devs[4];

static unsigned long gpiof_idr;
static unsigned long gpiog_idr;


#define GPIO_OFT(x) ((x) - 0x56000000)
#define GPFCON  (*(volatile unsigned long *)(gpiof_idr + GPIO_OFT(0x56000050)))
#define GPFDAT  (*(volatile unsigned long *)(gpiof_idr + GPIO_OFT(0x56000054)))

#define GPGCON  (*(volatile unsigned long *)(gpiog_idr + GPIO_OFT(0x56000060)))
#define GPGDAT  (*(volatile unsigned long *)(gpiog_idr + GPIO_OFT(0x56000064)))


static int s3c24xx_keys_open(struct inode *inode, struct file *file)
{
		/* 配置为输入引脚         */    
	GPFCON &=~(0x3<<(0*2)|0x3<<(2*2));
		
	GPGCON &=~(0x3<<(3*2));

	return 0;
}
static int s3c24xx_keys_read(struct file *filp, char __user *buff, 
                                         size_t count, loff_t *offp)
{
	unsigned char keyval[3];

	int regval;
	
	regval=GPFDAT; 

	keyval[0]=(regval & (1<<0)) ? 1 : 0 ;
	keyval[1]=(regval & (1<<2)) ? 1 : 0 ;

	regval=GPGDAT;
	keyval[2]=(regval & (1<<3)) ? 1 : 0 ;

	copy_to_user(buff, keyval, sizeof(keyval));
		
	return (sizeof(keyval ));
}


/* 这个结构是字符设备驱动程序的核心
 * 当应用程序操作设备文件时所调用的open、read、write等函数，
 * 最终会调用这个结构中指定的对应函数
 */
static struct file_operations s3c24xx_keys_fops = {
    .owner  =   THIS_MODULE,    /* 这是一个宏，推向编译模块时自动创建的__this_module变量 */
    .open   =   s3c24xx_keys_open,     
	.read	=	s3c24xx_keys_read,		   
};

int major;
static int  s3c24xx_keys_init(void)
{
   //int i=0;  //循环变量

   gpiof_idr = ioremap(0x56000000, 0x100000);
	if (!gpiof_idr) {
		return -EIO;
	}
	gpiog_idr = ioremap(0x56000000, 0x100000);
	if (!gpiog_idr) {
		return -EIO;
	}
	major=register_chrdev(0,"button",&s3c24xx_keys_fops);
	
	keys_class = class_create(THIS_MODULE, "button");
	if (IS_ERR(keys_class))
			return PTR_ERR(keys_class);
//	keys_class_devs[0] = class_device_create(keys_class, NULL, MKDEV(major, 0), NULL, "keys");
//	for (i = 1; i < 4; i++)
//	{ 
//		keys_class_devs[i] = class_device_create(keys_class, NULL, MKDEV(major, i), NULL, "key%d", i);
//		if (unlikely(IS_ERR(keys_class_devs[i])))
//			return PTR_ERR(keys_class_devs[i]);
//	}

	keys_class_devs = class_device_create(keys_class, NULL, MKDEV(major, 0), NULL, "button");

	printk( "key_drv initialized\n");

	return 0;
}

static int  s3c24xx_keys_exit(void)
{
//	int i;//循环变量

	unregister_chrdev(major,"button" );
	
// 	for (i = 0; i < 4; i++)
// 	{
 //		class_device_unregister(keys_class_devs[i]);
// 	} 
	class_device_unregister(keys_class_devs);

	class_destroy(keys_class);
	
	iounmap(gpiof_idr);
	iounmap(gpiog_idr);


	return 0;
}

module_init(s3c24xx_keys_init);
module_exit(s3c24xx_keys_exit);

MODULE_LICENSE("GPL");







































