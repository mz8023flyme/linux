#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/i2c.h>
#include <linux/err.h>
#include <linux/regmap.h>
#include <linux/slab.h>

static struct i2c_board_info   at24c02_info ={

I2C_BOARD_INFO("at24c02", 0x50)

}; 
 
static struct i2c_client * at24c02_cline;

static int at24c02_dev_init(void)
{
    struct i2c_adapter * i2c_adap;

    
    i2c_adap = i2c_get_adapter(0); 
    at24c02_cline = i2c_new_device(i2c_adap , &at24c02_info );
    i2c_put_adapter(i2c_adap);
    if(at24c02_cline)
        return 0; 
    else
        return -ENODEV;
}

static int at24c02_dev_exit(void)
{
    i2c_unregister_device(at24c02_cline);
    return 0;
}

module_init (at24c02_dev_init);
module_exit (at24c02_dev_exit);
MODULE_LICENSE("GPL");

 











