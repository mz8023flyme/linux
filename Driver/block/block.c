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



static struct class *block_class;
static struct class_device	*block_class_devs;

static unsigned long gpiof_idr;
static unsigned long gpiog_idr;
static unsigned char keyvalue;  //按键键值保存变量
int major; 						 //主设备号
static volatile int eve_press;   //按键按下事件标志变量
static struct fasync_struct *button_fasync ;
//static atomic_t canopen = ATOMIC_INIT(1);//设置驱动是否打开的标志位为原子变量

static DECLARE_MUTEX(button_lock);  //定义互斥锁

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
	
	if(pinval)
	{
		/*松开*/
		keyvalue  = 0x80|pindesc->key_value;
	}
	else
	{
		/*按下*/
		keyvalue = pindesc->key_value;
	}

	eve_press=1; 
	
	wake_up_interruptible(&button_waitq);

	kill_fasync(&button_fasync , SIGIO , POLL_IN);
	
	return IRQ_HANDLED ;
}

static int block_open(struct inode *inode, struct file *file)
{
#if 0
	if(atomic_dec_and_test(&canopen) !=0)
	{
		atomic_inc(&canopen);
		return -EBUSY;
	}
#endif

	if (file->f_flags & O_NONBLOCK)  //非阻塞
	{
		if (down_trylock(&button_lock))
			return -EBUSY;
	}
	else
	{
		/* 获取信号量 */
		down(&button_lock);

	}
		/* 配置为中断引脚         */    
	request_irq(IRQ_EINT0,button_irq,IRQT_FALLING,"S1",&pins_desc[0]);
	request_irq(IRQ_EINT2,button_irq,IRQT_FALLING,"S2",&pins_desc[1]);
	request_irq(IRQ_EINT11,button_irq,IRQT_FALLING,"S3",&pins_desc[2]);
	
	return 0;
}
static int block_read(struct file *filp, char __user *buff, 
                                         size_t count, loff_t *offp)
{

	if(filp->f_flags & O_NONBLOCK)	//非阻塞
	{
		if(!eve_press)
			return -EAGAIN;
	}
	else
	{
		/* 没有按键就休眠 */
		wait_event_interruptible(button_waitq, eve_press);
	}
	copy_to_user(buff, &keyvalue,1);

	eve_press=0;

	return 0;
}
										 
static int block_close (struct inode *inode, struct file *file)
{
	//atomic_inc(&canopen);

	free_irq(IRQ_EINT0,&pins_desc[0]);   
	free_irq(IRQ_EINT2,&pins_desc[1]);
	free_irq(IRQ_EINT11,&pins_desc[2]);
	/* 释放信号量 */
	up(&button_lock);
	return 0;
}
static unsigned block_poll(struct file *file, poll_table *wait)
{
	unsigned int mask = 0;
	poll_wait(file, &button_waitq, wait); // 不会立即休眠

	if (eve_press)
		mask |= POLLIN | POLLRDNORM;

	return mask;
}

static int block_fasync (int fd,struct file *filp,int on)
{
	printk("block_fasync\n");
	return fasync_helper(fd,filp,on,&button_fasync);
}


/* 这个结构是字符设备驱动程序的核心
 * 当应用程序操作设备文件时所调用的open、read、write等函数，
 * 最终会调用这个结构中指定的对应函数
 */
static struct file_operations block_fops = {
     .owner   =   THIS_MODULE,    /* 这是一个宏，推向编译模块时自动创建的__this_module变量 */
     .open    =   block_open,     
     .read	  =	  block_read,	
 	 .release =   block_close,
 	 .poll    =   block_poll,
	 .fasync  =   block_fasync,
};


static int  block_init(void)
{

   gpiof_idr = ioremap(0x56000000, 0x100000);
	if (!gpiof_idr) {
		return -EIO;
	}
	gpiog_idr = ioremap(0x56000000, 0x100000);
	if (!gpiog_idr) {
		return -EIO;
	}
	major=register_chrdev(0,"block",&block_fops);
	
	block_class = class_create(THIS_MODULE, "block");
	if (IS_ERR(block_class))
			return PTR_ERR(block_class);

	block_class_devs = class_device_create(block_class, NULL, MKDEV(major, 0), NULL, "block");

	printk( "block install\n");

	return 0;
}

static int  block_exit(void)
{

	unregister_chrdev(major,"block" );

	class_device_unregister(block_class_devs);

	class_destroy(block_class);
	
	iounmap(gpiof_idr);
	iounmap(gpiog_idr);

	printk(" block rmmoded\n");
	return 0;
}

module_init(block_init);
module_exit(block_exit);

MODULE_LICENSE("GPL");




















































































