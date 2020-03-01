#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/i2c.h>
#include <linux/err.h>
#include <linux/regmap.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <linux/fs.h>


//可以不用知道究竟使用哪个I2C控制器
/*

static const unsigned short addr_list[]={0x39,0x50,I2C_CLIENT_END};

static int at24c02_detect(struct i2c_client *client, struct i2c_board_info *info)
{
     printk("%s : addr = 0x%x\n",__FUNCTION__,client->addr);
     if(client->addr==0x50)
     {
        strlcpy(info->type,"at24c02",I2C_NAME_SIZE);
        return 0;
     }
     else if (client->addr==0x39)
     {
        strlcpy(info->type,"TSL2561",I2C_NAME_SIZE);
        return 0;
     }
     else
        return -ENODEV;
}
*/
/*1、分配/设置 i2c_driver */

static const struct i2c_device_id at24c02_id_table[] = {
	{ "at24c02", 0 },
	{}
};
static struct i2c_client * at24c02_cline;


static ssize_t at24c02_read(struct file *file, char __user *buf, size_t count, loff_t *off)
{
     
    unsigned char addr ,data;
    copy_from_user(&addr, buf, 1);
    data=i2c_smbus_read_byte_data(at24c02_cline, addr);
    copy_to_user(buf, &data,1);
    return 1;
  
    
}

static ssize_t at24c02_write (struct file *file, const char __user *buf, size_t count, loff_t *off)
{
    unsigned char ker_buff[2];
    unsigned char addr ,data;
    
    copy_from_user(ker_buff, buf, 2);    
    addr = ker_buff[0];
    data = ker_buff[1];

    if(!i2c_smbus_write_byte_data(at24c02_cline, addr,data))
    {
        printk("addr = %d, data = 0x%02x\n", addr, data);
        return 2;
    }
    else
        return -EIO;
}


static major ;
static struct class *class;


static struct file_operations at24c02_fop={
    
     .owner = THIS_MODULE,
     .read   = at24c02_read,
     .write = at24c02_write,
};

static int __devinit at24c02_probe(struct i2c_client * client, const struct i2c_device_id *id)
{
    printk("[%s][%d]\n",__FUNCTION__,__LINE__);
    at24c02_cline=client;
    major = register_chrdev(0, "at24c02", &at24c02_fop );
    class = class_create(THIS_MODULE, "at24c02");
    device_create(class, NULL, MKDEV(major,0), NULL,"at24c02");// 创建/dev/at24c02
    
    return 0;
}

static int __devexit at24c02_remove(struct i2c_client *client)
{
    printk("[%s][%d]\n",__FUNCTION__,__LINE__);
    device_destroy(  class, MKDEV(major,0));
    class_destroy(class);
    unregister_chrdev(  major,"at24c02");
    return 0 ;
}
static struct i2c_driver at24c02_driver = {
   // .class = I2C_CLASS_HWMON,  //表示去哪里找这个设备
	.driver	= {
		.name	= "abcdefg",
 		.owner	= THIS_MODULE,
	},
	.probe		= at24c02_probe,
	.remove		= __devexit_p(at24c02_remove),
	.id_table	= at24c02_id_table,
	//.detect = at24c02_detect,
	//.address_list = addr_list,
};
static int eeprom_drv_init(void)
{
   /*1、注册i2c_driver*/
    i2c_add_driver(&at24c02_driver);

    return 0;
}

static int eeprom_drv_exit(void)
{
    i2c_del_driver(&at24c02_driver);
    return 0;
}
module_init(eeprom_drv_init);
module_exit(eeprom_drv_exit);

MODULE_LICENSE("GPL");





 











