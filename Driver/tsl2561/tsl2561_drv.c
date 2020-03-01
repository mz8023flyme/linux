#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/i2c.h>
#include <linux/err.h>
#include <linux/regmap.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <linux/fs.h>

static int major ;
static struct class * class;
static struct i2c_client * tsl2561_cline;

static ssize_t tsl2561_read (struct file *file, char __user *buf, size_t count, loff_t * len)
{
    unsigned char addr ;
    unsigned char data;
   // printk("[%s][%d]",__FUNCTION__,__LINE__);
    copy_from_user(&addr, buf ,1);
    data=i2c_smbus_read_byte_data(tsl2561_cline, addr);
    //printk("---%d---",data);
    copy_to_user(buf,&data, 1);
    return 1;
}

static ssize_t tsl2561_write (struct file *file , const char __user * buf, size_t count, loff_t *len)
{
      unsigned char addr ,data;
      unsigned char ker_buf[2];
      copy_from_user(ker_buf, buf ,2);
      addr =  ker_buf[0];
      data =  ker_buf[1];
      if(!(i2c_smbus_write_byte_data(tsl2561_cline, addr,data)))
      {
            printk("addr = %d, data = 0x%02x\n", addr, data);
            return 2;
      }
      else
      {
        return -EIO;
      }
       
}

static struct file_operations tsl2561_fops={
    .owner = THIS_MODULE,
    .read  = tsl2561_read,
    .write = tsl2561_write,
};




static int tsl2561_probe(struct i2c_client *client, const struct i2c_device_id * id)
{
    printk(" [%s][%d]\n",  __FUNCTION__, __LINE__);
    tsl2561_cline=client;
    major = register_chrdev(0, "tsl2561", &tsl2561_fops );
    class = class_create(THIS_MODULE, "tsl2561");
    device_create(class, NULL, MKDEV(major, 0), NULL,"tsl2561");
    return 0 ;
}
static int tsl2561_remove(struct i2c_client * client)
{
    printk(" [%s][%d]\n",  __FUNCTION__, __LINE__);
    device_destroy(class,  MKDEV(major, 0));
    class_destroy(class);
    unregister_chrdev(major,"tsl2561");
    return 0;
}

static const struct i2c_device_id tsl2561_id_table[] = {
	{ "tsl2561", 0 },
	{}
};


static struct i2c_driver tsl2561_driver={
    .driver	 = {
        .name = "tsl2561",
        .owner = THIS_MODULE,
    },
    .probe =tsl2561_probe ,
    .remove = __devexit_p(tsl2561_remove),
    .id_table = tsl2561_id_table,
};




static int  tsl2561_drv_init(void)
{
    /*1¡¢×¢²ái2c_driver*/
    i2c_add_driver(&tsl2561_driver);   
    return 0;
}
static int tsl2561_drv_exit(void)
{
    i2c_del_driver(&tsl2561_driver);
    return 0;
}

module_init(tsl2561_drv_init);
module_exit(tsl2561_drv_exit);
MODULE_LICENSE("GPL");






















