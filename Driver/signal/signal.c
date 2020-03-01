#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/irq.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <asm/arch/regs-gpio.h>
#include <asm/hardware.h>
#include <linux/poll.h>

static struct class *signal_class;
static struct class_device	*signal_class_devs;

static unsigned long gpiof_idr;
static unsigned long gpiog_idr;
static unsigned char keyvalue;  //按键键值保存变量
int major; 						 //主设备号
static volatile int event_press;   //按键按下事件标志变量
static struct fasync_struct *button_fasync ;


static DECLARE_WAIT_QUEUE_HEAD(button_waitq);


#define GPIO_OFT(x) ((x) - 0x56000000)
#define GPFCON  (*(volatile unsigned long *)(gpiof_idr + GPIO_OFT(0x56000050)))
#define GPFDAT  (*(volatile unsigned long *)(gpiof_idr + GPIO_OFT(0x56000054)))

#define GPGCON  (*(volatile unsigned long *)(gpiog_idr + GPIO_OFT(0x56000060)))
#define GPGDAT  (*(volatile unsigned long *)(gpiog_idr + GPIO_OFT(0x56000064)))


struct pin_desc{
	unsigned int pin ;
	unsigned int key_value;
};
struct pin_desc pins_desc[3]={
	{S3C2410_GPF0,0X01},
	{S3C2410_GPF2,0X02},
	{S3C2410_GPG3,0X03},
};



static irqreturn_t button_irq(int irq,void *dev_id)
{
	struct pin_desc* pindesc = (struct pin_desc*)dev_id;
	unsigned int pinval;
	
	pinval=s3c2410_gpio_getpin(pindesc->pin);

	if(!pinval)
	{
		/*松开*/
		keyvalue  = 0x80|pindesc->key_value;
	}
	else
	{
		/*按下*/
		keyvalue = pindesc->key_value;
	}

	event_press=1; 
	
	wake_up_interruptible(&button_waitq);

	kill_fasync(&button_fasync , SIGIO , POLL_IN);
	
	return IRQ_HANDLED ;
}

static int signal_open(struct inode *inode, struct file *file)
{
		/* 配置为中断引脚         */    
	request_irq(IRQ_EINT0,button_irq,IRQT_FALLING,"S1",&pins_desc[0]);
	request_irq(IRQ_EINT2,button_irq,IRQT_FALLING,"S2",&pins_desc[1]);
	request_irq(IRQ_EINT11,button_irq,IRQT_FALLING,"S3",&pins_desc[2]);
	
	return 0;
}
static int signal_read(struct file *filp, char __user *buff, 
                                         size_t count, loff_t *offp)
{
	
	/* 没有按键就休眠 */
	wait_event_interruptible(button_waitq, event_press);
	
	copy_to_user(buff, &keyvalue,1);

	event_press=0;

	return 0;
}
										 
static int signal_close (struct inode *inode, struct file *file)
{
	free_irq(IRQ_EINT0,&pins_desc[0]);
	free_irq(IRQ_EINT2,&pins_desc[1]);
	free_irq(IRQ_EINT11,&pins_desc[2]);
	
	return 0;
}
static unsigned signal_poll(struct file *file, poll_table *wait)
{
	unsigned int mask = 0;
	poll_wait(file, &button_waitq, wait); // 不会立即休眠

	if (event_press)
		mask |= POLLIN | POLLRDNORM;

	return mask;
}

static int signal_fasync (int fd,struct file *filp,int on)
{
	printk("signal_fasync\n");
	return fasync_helper(fd,filp,on,&button_fasync);
}


/* 这个结构是字符设备驱动程序的核心
 * 当应用程序操作设备文件时所调用的open、read、write等函数，
 * 最终会调用这个结构中指定的对应函数
 */
static struct file_operations signal_fops = {
     .owner   =   THIS_MODULE,    /* 这是一个宏，推向编译模块时自动创建的__this_module变量 */
     .open    =   signal_open,     
     .read	  =	  signal_read,	
 	 .release =   signal_close,
 	 .poll    =   signal_poll,
	 .fasync  =   signal_fasync,
	 
};


static int  signal_init(void)
{

   gpiof_idr = ioremap(0x56000000, 0x100000);
	if (!gpiof_idr) {
		return -EIO;
	}
	gpiog_idr = ioremap(0x56000000, 0x100000);
	if (!gpiog_idr) {
		return -EIO;
	}
	major=register_chrdev(0,"signal",&signal_fops);
	
	signal_class = class_create(THIS_MODULE, "signal");
	if (IS_ERR(signal_class))
			return PTR_ERR(signal_class);

	signal_class_devs = class_device_create(signal_class, NULL, MKDEV(major, 0), NULL, "signal");

	printk( "signal install\n");

	return 0;
}

static int  signal_exit(void)
{

	unregister_chrdev(major,"signal" );

	class_device_unregister(signal_class_devs);

	class_destroy(signal_class);
	
	iounmap(gpiof_idr);
	iounmap(gpiog_idr);

	printk(" signal rmmoded\n");
	return 0;
}

module_init(signal_init);
module_exit(signal_exit);

MODULE_LICENSE("GPL");




















































































