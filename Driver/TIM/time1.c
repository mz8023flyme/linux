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

static struct class *time_class;
static struct class_device	*time_class_devs;
static struct timer_list 	buttons_time;
static struct pin_desc *irq_pd;

static unsigned long gpiof_idr;
static unsigned long gpiog_idr;
static unsigned char keyvalue;  //������ֵ�������
int major; 						 //���豸��
static volatile int event_press;   //���������¼���־����
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
	irq_pd = (struct pin_desc *)dev_id;
	
	mod_timer(&buttons_time,jiffies+HZ/100);
	
	return IRQ_HANDLED ;
}

static int time_open(struct inode *inode, struct file *file)
{
		/* ����Ϊ�ж�����         */    
	request_irq(IRQ_EINT0,button_irq,IRQT_FALLING,"S1",&pins_desc[0]);
	request_irq(IRQ_EINT2,button_irq,IRQT_FALLING,"S2",&pins_desc[1]);
	request_irq(IRQ_EINT11,button_irq,IRQT_FALLING,"S3",&pins_desc[2]);
	
	return 0;
}
static int time_read(struct file *filp, char __user *buff, 
                                         size_t count, loff_t *offp)
{
	
	/* û�а��������� */
	wait_event_interruptible(button_waitq, event_press);
	
	copy_to_user(buff, &keyvalue,1);

	event_press=0;

	return 0;
}
										 
static int time_close (struct inode *inode, struct file *file)
{
	free_irq(IRQ_EINT0,&pins_desc[0]);
	free_irq(IRQ_EINT2,&pins_desc[1]);
	free_irq(IRQ_EINT11,&pins_desc[2]);
	
	return 0;
}
static unsigned time_poll(struct file *file, poll_table *wait)
{
	unsigned int mask = 0;
	poll_wait(file, &button_waitq, wait); // ������������

	if (event_press)
		mask |= POLLIN | POLLRDNORM;

	return mask;
}

static int time_fasync (int fd,struct file *filp,int on)
{
	printk("signal_fasync\n");
	return fasync_helper(fd,filp,on,&button_fasync);
}


/* ����ṹ���ַ��豸��������ĺ���
 * ��Ӧ�ó�������豸�ļ�ʱ�����õ�open��read��write�Ⱥ�����
 * ���ջ��������ṹ��ָ���Ķ�Ӧ����
 */
static struct file_operations time_fops = {
     .owner   =   THIS_MODULE,    /* ����һ���꣬�������ģ��ʱ�Զ�������__this_module���� */
     .open    =   time_open,     
     .read	  =	  time_read,	
 	 .release =   time_close,
 	 .poll    =   time_poll,
	 .fasync  =   time_fasync,
	 
};

static void buttons_timer_function (unsigned long data)
{
	struct pin_desc* pindesc =irq_pd;
	unsigned int pinval;

	if(!pindesc)
		return  ;
	
	pinval=s3c2410_gpio_getpin(pindesc->pin);

	if(pinval)
	{
		/*�ɿ�*/
		keyvalue  = 0x80|pindesc->key_value;
	}
	else
	{
		/*����*/
		keyvalue = pindesc->key_value;
	}

	event_press=1; 
	
	wake_up_interruptible(&button_waitq);

	kill_fasync(&button_fasync , SIGIO , POLL_IN);
}
static int  time_init(void)
{

	init_timer(&buttons_time);
	//buttons_timer.data = (unsigned long ) SCpnt;
	//buttons_timer.expires = jiffies + 100*HZ ;  /*10s*/ 
	add_timer(&buttons_time);
	buttons_time.function = buttons_timer_function;
	

   gpiof_idr = ioremap(0x56000000, 0x100000);
	if (!gpiof_idr) {
		return -EIO;
	}
	gpiog_idr = ioremap(0x56000000, 0x100000);
	if (!gpiog_idr) {
		return -EIO;
	}
	major=register_chrdev(0,"timer",&time_fops);
	
	time_class = class_create(THIS_MODULE, "timer");
	if (IS_ERR(time_class))
			return PTR_ERR(time_class);

	time_class_devs = class_device_create(time_class, NULL, MKDEV(major, 0), NULL, "timer");

	printk( "timer install\n");

	return 0;
}

static int  time_exit(void)
{

	unregister_chrdev(major,"timer" );

	class_device_unregister(time_class_devs);

	class_destroy(time_class);
	
	iounmap(gpiof_idr);
	iounmap(gpiog_idr);

	printk(" timer rmmoded\n");
	return 0;
}

module_init(time_init);
module_exit(time_exit);

MODULE_LICENSE("GPL");




















































































