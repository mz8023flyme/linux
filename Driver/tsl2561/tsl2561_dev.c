#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/i2c.h>
#include <linux/err.h>
#include <linux/regmap.h>
#include <linux/slab.h>


static struct i2c_board_info   tsl2561_info={
 I2C_BOARD_INFO("tsl2561", 0x39)
};
static struct i2c_client * tsl2561_client;

static int tsl2561_dev_init(void)
{
    struct i2c_adapter * i2c_adap;


    i2c_adap = i2c_get_adapter(0);

    tsl2561_client = i2c_new_device( i2c_adap, &tsl2561_info);

    i2c_put_adapter(i2c_adap);
    
    if(tsl2561_client)
    {
        return 0;
    }
    else
    {
        printk("can't new device");
        return -ENODEV;
    }
}
static int tsl2561_dev_exit(void)
{
    i2c_unregister_device(tsl2561_client);
    return 0;
}

module_init(tsl2561_dev_init);
module_exit(tsl2561_dev_exit) ;
MODULE_LICENSE("GPL");











