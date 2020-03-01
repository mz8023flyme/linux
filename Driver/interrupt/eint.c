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

static struct class *eint_class;
static struct class_device	*eint_class_devs;

static unsigned long gpiof_idr;
static unsigned long gpiog_idr;
static unsigned char keyvalue;  //������ֵ�������
int major; 						 //���豸��
static volatile int event_press;   //���������¼���־����

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
	
	return IRQ_HANDLED ;
}

static int eint_open(struct inode *inode, struct file *file)
{
		/* ����Ϊ�ж�����         */    
	request_irq(IRQ_EINT0,button_irq,IRQT_FALLING,"S1",&pins_desc[0]);
	request_irq(IRQ_EINT2,button_irq,IRQT_FALLING,"S2",&pins_desc[1]);
	request_irq(IRQ_EINT11,button_irq,IRQT_FALLING,"S3",&pins_desc[2]);
	
	return 0;
}
static int eint_read(struct file *filp, char __user *buff, 
                                         size_t count, loff_t *offp)
{
	
	/* û�а��������� */
	wait_event_interruptible(button_waitq, event_press);
	
	copy_to_user(buff, &keyvalue,1);

	event_press=0;

	return 0;
}
										 
static int eint_close (struct inode *inode, struct file *file)
{
	free_irq(IRQ_EINT0,&pins_desc[0]);
	free_irq(IRQ_EINT2,&pins_desc[1]);
	free_irq(IRQ_EINT11,&pins_desc[2]);
	
	return 0;
}


/* ����ṹ���ַ��豸��������ĺ���
 * ��Ӧ�ó�������豸�ļ�ʱ�����õ�open��read��write�Ⱥ�����
 * ���ջ��������ṹ��ָ���Ķ�Ӧ����
 */
static struct file_operations eint_fops = {
     .owner   =   THIS_MODULE,    /* ����һ���꣬�������ģ��ʱ�Զ�������__this_module���� */
     .open    =   eint_open,     
     .read	  =	  eint_read,	
 	 .release =   eint_close,
};


static int  eint_init(void)
{

   gpiof_idr = ioremap(0x56000000, 0x100000);
	if (!gpiof_idr) {
		return -EIO;
	}
	gpiog_idr = ioremap(0x56000000, 0x100000);
	if (!gpiog_idr) {
		return -EIO;
	}
	major=register_chrdev(0,"eint",&eint_fops);
	
	eint_class = class_create(THIS_MODULE, "eint");
	if (IS_ERR(eint_class))
			return PTR_ERR(eint_class);

	eint_class_devs = class_device_create(eint_class, NULL, MKDEV(major, 0), NULL, "eint");

	printk( "eint  install\n");

	return 0;
}

static int  eint_exit(void)
{

	unregister_chrdev(major,"eint" );

	class_device_unregister(eint_class_devs);

	class_destroy(eint_class);
	
	iounmap(gpiof_idr);
	iounmap(gpiog_idr);

	printk(" eint rmmoded\n");
	return 0;
}

module_init(eint_init);
module_exit(eint_exit);

MODULE_LICENSE("GPL");




















































































