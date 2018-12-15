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



struct pin_desc{
	int irq;
	char*name;
	unsigned int pin;
	unsigned int key_val;
};

static struct pin_desc pins_desc[4] = {
 	{IRQ_EINT0,"S1",S3C2410_GPF0, KEY_L},
 	{IRQ_EINT2,"S2",S3C2410_GPF2, KEY_S},
 	{IRQ_EINT11,"S3",S3C2410_GPG3, KEY_ENTER},
    {IRQ_EINT19,"S4",S3C2410_GPG11, KEY_BACK},
};


static struct input_dev *key_led_dev;
static struct timer_list key_timer;
static struct pin_desc 		*irq_pd;

static void key_led_irq(int irq,void * dev_id)
{
	/**/
	irq_pd = (struct pin_desc*) dev_id;
    //key_timer.data= irq_pd->pin;
	mod_timer(&key_timer,jiffies+HZ/100);
	return IRQ_RETVAL(IRQ_HANDLED);
}
static int key_timer_function(unsigned long data)
{
    struct pin_desc * pindesc = irq_pd;
    unsigned int pinval;
    if (!pindesc)
            return;
    pinval = s3c2410_gpio_getpin(pindesc->pin);
    if(pinval)
    {
        /*按键弹起      上报事件*/   
        //input_report_key(key_led_dev ,pindesc->key_val,0);
        input_event(key_led_dev,EV_KEY,pindesc->key_val,0);
        input_sync(key_led_dev);
    }
    else
    {
        /*按键按下       上报事件*/
        //input_report_key(key_led_dev ,pindesc->key_val,0);
        input_event(key_led_dev,EV_KEY,pindesc->key_val,1);
        input_sync(key_led_dev);
    }


}


static int key_led_open(struct input_dev *dev)
{
    int i;
    /*设置IO为中断输入方式*/
    s3c2410_gpio_cfgpin(S3C2410_GPF0, S3C2410_GPF0_EINT0);
    s3c2410_gpio_cfgpin(S3C2410_GPF2, S3C2410_GPF2_EINT2);
    s3c2410_gpio_cfgpin(S3C2410_GPG3, S3C2410_GPG3_EINT11);
    s3c2410_gpio_cfgpin(S3C2410_GPG11, S3C2410_GPG11_EINT19);
    
    /*设置IO为输出方式*/
    s3c2410_gpio_cfgpin(S3C2410_GPF4, S3C2410_GPF4_OUTP);
    s3c2410_gpio_setpin(S3C2410_GPF4, 1);

    s3c2410_gpio_cfgpin(S3C2410_GPF5, S3C2410_GPF5_OUTP);
    s3c2410_gpio_setpin(S3C2410_GPF5, 1);

    s3c2410_gpio_cfgpin(S3C2410_GPF6, S3C2410_GPF6_OUTP);
    s3c2410_gpio_setpin(S3C2410_GPF6, 1);
    for (i=0;i<4;i++)
    {
        request_irq(pins_desc[i].irq, key_led_irq, IRQT_BOTHEDGE, pins_desc[i].name ,&pins_desc[i]);
    }
    


    printk("input key_led open!\n");
    return 0;
}


static int led_event (struct input_dev *dev, unsigned int type, unsigned int code, int value)
{
    printk("led event!\n");
    printk("value: 0x%x\n", value);
    printk("it's work\n");
    if(code==LED_MUTE)
    {
        printk("led do \n");
        if(value == 0xAA)//点亮
             s3c2410_gpio_setpin(S3C2410_GPF4, 0);
        else if(value == 0xEE)//熄灭
             s3c2410_gpio_setpin(S3C2410_GPF4, 1);
        return 0;
    }
    return -1;
}


static int  key_led_init()
{
    int i;
	/*1. 分配一个input_dev结构体  */ 
	key_led_dev=input_allocate_device();
    key_led_dev->name = "key_led";
    /*2. 设置 */
	/*2.1 能产生哪类事件*/
    set_bit(EV_LED, key_led_dev->evbit);
    //set_bit(EV_SND, input_subsys_dev->evbit);//声音
    set_bit(EV_KEY,key_led_dev->evbit);
    
    /*2.2 表示能产生哪些事件*/
    set_bit(LED_MUTE,key_led_dev->keybit);
    set_bit(KEY_L,key_led_dev->keybit);
    set_bit(KEY_S,key_led_dev->keybit);    
    set_bit(KEY_ENTER,key_led_dev->keybit);
    set_bit(KEY_BACK,key_led_dev->keybit);
    /* [cgw]: 分配输入设备的open方法，用户操作open(/dev/xxx, ...)时调用 */
    key_led_dev->open = key_led_open;
    /* [cgw]: 分配输入设备的event方法，用户在应用程序write()时 */
    key_led_dev->event = led_event;
    /*3. 注册   */
	input_register_device(key_led_dev);
    /*4. 硬件相关的操作   */ 
    init_timer(&key_timer);
    key_timer.function=key_timer_function;
    add_timer(&key_timer);
    printk("input key_led init!\n");
    return 0;
}

static int key_led_exit()
{
    int i;
    /* [cgw]: 释放按键IO中断 */
    for (i = 0; i < 4; i++)
    {
        free_irq(pins_desc[i].irq, &pins_desc[i]);
    }
    del_timer(&key_timer);
    input_unregister_device(key_led_dev);
    input_free_device(key_led_dev);

    
    return 0;
}



module_init(key_led_init);
module_exit(key_led_exit);
MODULE_LICENSE("GPL");














