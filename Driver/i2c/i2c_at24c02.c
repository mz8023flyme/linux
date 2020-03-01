/* 
 * 参考 ds1374.c
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/i2c.h>
#include <linux/rtc.h>
#include <linux/bcd.h>
#include <linux/mutex.h>
#include <linux/workqueue.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/jiffies.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

/*1.分配一个 i2c_drive结构体*/
static struct i2c_driver at24c02_driver;

static unsigned short ignore[] = { I2C_CLIENT_END };
static unsigned short normal_addr[] = { 0x50, I2C_CLIENT_END };
                                       /*地址是7位*/
struct i2c_client *at24c02_client;

static int major;
static struct  class *cls; 

static ssize_t at24c02_read (struct file *file, char __user * buf , size_t size, loff_t *offset)
{
    unsigned char addres;
    unsigned char data;
    struct i2c_msg msg[2];
    int ret;
    /* 
     * buf[0]=addres;
     * buf[1] =data;
     */
     if (size != 1)
		return -EINVAL;
    copy_from_user(&addres, buf, 1);

    /*数据传输三要素  源  目的  长度*/
    /*先写地址*/
    msg[0].addr  =  at24c02_client->addr; //目的
    msg[0].buf   =  &addres;               //源
    msg[0].len   =  1;                      //长度
    msg[0].flags =  0;                      //写

     /*开始读操作*/
    msg[1].addr  =  at24c02_client->addr;   //源
    msg[1].buf   =  &data;                  //目的
    msg[1].len   =  1;                      //长度
    msg[1].flags =  I2C_M_RD;               //读


    ret =  i2c_transfer(at24c02_client->adapter, msg, 2);
    if(ret==2)
    {
        copy_to_user(buf, &data, 1);
        return 1;

    }
    else
        return -EIO;

}
static ssize_t at24c02_write (struct file *file, const char __user *buf, size_t size, loff_t *offset)
{
    unsigned char val[2];
    struct i2c_msg msg[1];
    int ret;
    /* 
     * buf[0]=addres;
     * buf[1] =data;
     */
     if (size !=2)
		return -EINVAL;
    copy_from_user(val, buf, 2);

    /*数据传输三要素  源  目的  长度*/
    msg[0].addr  =  at24c02_client->addr; //目的
    msg[0].buf   =  val;                   //源
    msg[0].len   =  2;                      //长度
    msg[0].flags =  0;                      //写

    ret =  i2c_transfer(at24c02_client->adapter, msg, 1);
    if(ret==1)
        return 2;
    else
        return -EIO;

}

static struct file_operations at24c02_fops = {
    .owner  =  THIS_MODULE,
    .read   =  at24c02_read,
    .write  =  at24c02_write,
};

//static unsigned short  force_addr[]={ANY_I2C_BUS,0x60,I2C_CLIENT_END};
//static unsigned short * forces[]={force_addr,NULL};

static struct i2c_client_address_data addr_data = {
    .normal_i2c = normal_addr,
    .probe = ignore,
    .ignore = ignore,
    //.forces = force_addr,
};

static int at24c02_probe(struct i2c_adapter *adap, int addr, int kind)
{   
    printk("at24c02_probe  \n");

    /*构造一个client结构体*/
    at24c02_client = kzalloc(sizeof(struct i2c_client), GFP_KERNEL);
    strncpy(at24c02_client->name, "at24c02", I2C_NAME_SIZE);
    at24c02_client->addr = addr;
	at24c02_client->adapter = adap;
	at24c02_client->driver = &at24c02_driver; 
    //strcpy(client->name, "at24c02");
    i2c_attach_client(at24c02_client);

    major = register_chrdev(0,"at24c02", &at24c02_fops);
    
    cls = class_create(THIS_MODULE, "at24c02");
    class_device_create(cls,NULL,MKDEV(major,0),NULL,"at24c02");
    
    return 0;
}
static int at24c02_detach(struct i2c_client *client)
{
   
    printk("at24c02_detach  \n");

    class_device_destroy(cls,MKDEV(major,0));
    class_destroy(cls);
    unregister_chrdev(major, "at24c02");
    

    i2c_detach_client(client);
   
    kfree(i2c_get_clientdata(client));
		//destroy_workqueue(ds1374_workqueue);

    return 0;
}

static int at24c02_attach(struct i2c_adapter *adap)
{
	return i2c_probe(adap, &addr_data, at24c02_probe);
}


/*2.设置i2c_drive结构体*/
static struct i2c_driver at24c02_driver = {
	.driver = {
		.name	= "at24c02",
	},
	.attach_adapter = at24c02_attach,
	.detach_client = at24c02_detach,
};


static int i2c_at24c02_init()
{
    i2c_add_driver(&at24c02_driver);
    return 0;
}

static void i2c_at24c02_exit()
{
    i2c_del_driver(&at24c02_driver);
}


module_init(i2c_at24c02_init);
module_exit(i2c_at24c02_exit);
MODULE_LICENSE("GPL");




















