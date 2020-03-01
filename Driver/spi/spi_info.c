#include <linux/module.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/spi/spi.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/jiffies.h>
#include <linux/delay.h>
#include <linux/io.h>

#include <linux/gpio.h>
#include <mach/regs-gpio.h>
#include <plat/gpio-cfg.h>



static struct spi_board_info s3c2440_spi_info[] ={

    {
        .modalias = "spi_flash",
        .max_speed_hz = 10000000,
        .bus_num = 1 ,
        .mode    = SPI_MODE_0,
        .chip_select = S3C2410_GPG(2),
        
    },
};




static int spi_info_init(void)
{

   return  spi_register_board_info(s3c2440_spi_info,ARRAY_SIZE(s3c2440_spi_info));

}


module_init(spi_info_init);
MODULE_LICENSE("GPL");








 




 





