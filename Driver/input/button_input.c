#include <linux/module.h>
#include <linux/version.h>

#include <linux/init.h>
#include <linux/fs.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/sched.h>
#include <linux/pm.h>
#include <linux/sysctl.h>
#include <linux/proc_fs.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/input.h>
#include <linux/irq.h>
#include <asm/io.h>
#include <asm/arch/regs-gpio.h>

#include <asm/gpio.h>

static  struct input_dev *buttons_dev;
static unsigned char key_val;
static struct timer_list buttons_timer;
static struct pin_desc 		*irq_pd;

struct pin_desc{
	int irq;
	char*name;
	unsigned int pin;
	unsigned int key_val;
};

/* 键值: 按下时, 0x01, 0x02, 0x03, 0x04 */
/* 键值: 松开时, 0x81, 0x82, 0x83, 0x84 */


static struct pin_desc pins_desc[4] = {
 	{IRQ_EINT0,"S1",S3C2410_GPF0, KEY_L},
 	{IRQ_EINT2,"S2",S3C2410_GPF2, KEY_S},
 	{IRQ_EINT11,"S3",S3C2410_GPG3, KEY_ENTER},
    {IRQ_EINT19,"S4",S3C2410_GPG11, KEY_BACK},
};

static void buttons_irq(int irq,void * dev_id)
{
	/**/
	irq_pd = (struct pin_desc*) dev_id;
	mod_timer(&buttons_timer,jiffies+HZ/100);
	return IRQ_RETVAL(IRQ_HANDLED);
}

static void buttons_timer_function (unsigned long data)
{
	struct pin_desc* pindesc =irq_pd;

	unsigned int pinval;

	if (!pindesc)
		return;
	
	pinval = s3c2410_gpio_getpin(pindesc->pin);

	if (pinval)
	{
		/* 松开 */
		input_event(buttons_dev,EV_KEY,pindesc->key_val,0);
		input_sync(buttons_dev);
	}
	else
	{
		/* 按下 */
		input_event(buttons_dev,EV_KEY,pindesc->key_val,1);
		input_sync(buttons_dev);
	}

    //ev_press = 1;                  /* 表示中断发生了 */
   // wake_up_interruptible(&button_waitq);   /* 唤醒休眠的进程 */
}


static int buttons_init()
{
	int i;
	/*1. 分配一个input_dev结构体  */ 
	buttons_dev=input_allocate_device();

	/*2. 设置 */
	/*2.1 能产生哪类事件*/
	set_bit(EV_KEY,buttons_dev->evbit);
	set_bit(EV_REL,buttons_dev->evbit);
	

	/*2.2 表示能产生哪些事件*/
	set_bit(KEY_L,buttons_dev->keybit);
	set_bit(KEY_S,buttons_dev->keybit);
	set_bit(KEY_ENTER,buttons_dev->keybit);
	set_bit(KEY_BACK,buttons_dev->keybit);
    /*3. 注册   */
	input_register_device(buttons_dev);

	/*4. 硬件相关的操作   */ 

	init_timer(&buttons_timer);

	buttons_timer.function=buttons_timer_function;
	add_timer(&buttons_timer);
	
	for (i=0;i<4;i++)
	{
		request_irq(pins_desc[i].irq, buttons_irq, IRQT_BOTHEDGE, pins_desc[i].name ,&pins_desc[i]);
	}
	 



	return 0;
}



static int buttons_exit()
{
	int i ;
	for(i=0;i<4;i++)
	{
		free_irq(pins_desc[i].irq,&pins_desc[i]);
	}
	del_timer(&buttons_timer);
	input_unregister_device(buttons_dev);
	input_free_device(buttons_dev);
	return 0;
}

module_init(buttons_init);
module_exit(buttons_exit);
MODULE_LICENSE("GPL");


































