 /*
  *  参考
  *  drivers/mtd/devices/mtdram.c
  *   drivers/mtd/devices/m25p80.c
  *
  */
/*
 * 首先：构造注册spi_drver
 * 然后：在spi_drver的prob 里构造注册mtd_info  
 */

#include <linux/init.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <sound/core.h>
#include <linux/spi/spi.h>
#include <asm/uaccess.h>
#include <linux/timer.h>
 
#include <mach/hardware.h>
#include <mach/regs-gpio.h>
 
#include <linux/gpio.h>
#include <plat/gpio-cfg.h>
 
#include <linux/mtd/cfi.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>

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


static struct spi_device * spi_flash;

/*
static void SPIFlashSendAddr(unsigned int addr)
{
    unsigned  char tx_buf[3];
    
    tx_buf[0]=addr >> 16;
    tx_buf[1]=addr >> 8;
    tx_buf[2]=addr & 0xff;
    spi_write (spi_flash, tx_buf, 3);
}
*/
/* 
 * 
 */
void SPIFlashReadID(int *pMID, int *pDID)
{
    unsigned  char tx_buf[4];
    unsigned  char rx_buf[2];
    
    tx_buf[0]=0x90;
    tx_buf[1]=0;
    tx_buf[2]=0;
    tx_buf[2]=0;
    
    spi_write_then_read(spi_flash, tx_buf, 4, rx_buf, 2)  ;

    *pMID = rx_buf[0];
    *pDID = rx_buf[1];
}

static void SPIFlashWriteEnable(int enable)
{
    unsigned char val = enable ? 0x06:0x04;
        
    spi_write(spi_flash, &val, 1);
}

static unsigned char SPIFlashReadStatusReg1(void)
{
    unsigned char val,cmd=0x05;
    spi_write_then_read(spi_flash, &cmd , 1, &val, 1);
    return val;
}

static unsigned char SPIFlashReadStatusReg2(void)
{
    unsigned char val,cmd=0x35;
    spi_write_then_read(spi_flash, &cmd , 1, &val, 1);
    return val;
}

static void SPIFlashWaitWhenBusy(void)
{
    while (SPIFlashReadStatusReg1() & 1)
    {
        /*休眠一段时间*/
        set_current_state(TASK_INTERRUPTIBLE);
        schedule_timeout(HZ/100);/*休眠10ms后再次判断*/
    }
}

static void SPIFlashWriteStatusReg(unsigned char reg1, unsigned char reg2)
{    
    unsigned  char tx_buf[3];
    
    SPIFlashWriteEnable(1);  
    
    tx_buf[0]=0x01;
    tx_buf[1]=reg1;
    tx_buf[2]=reg2;

    spi_write(spi_flash, tx_buf, 3);
    SPIFlashWaitWhenBusy();
}

static void SPIFlashClearProtectForStatusReg(void)
{
    unsigned char reg1, reg2;

    reg1 = SPIFlashReadStatusReg1();
    reg2 = SPIFlashReadStatusReg2();

    reg1 &= ~(1<<7);
    reg2 &= ~(1<<0);

    SPIFlashWriteStatusReg(reg1, reg2);
}


static void SPIFlashClearProtectForData(void)
{
    /* cmp=0,bp2,1,0=0b000 */
    unsigned char reg1, reg2;

    reg1 = SPIFlashReadStatusReg1();
    reg2 = SPIFlashReadStatusReg2();

    reg1 &= ~(7<<2);
    reg2 &= ~(1<<6);

    SPIFlashWriteStatusReg(reg1, reg2);
}

/* erase 4K */
void SPIFlashEraseSector(unsigned int addr)
{

    unsigned  char tx_buf[4];
    tx_buf[0]= 0x20;
    tx_buf[1]=addr >> 16;
    tx_buf[2]=addr >> 8;
    tx_buf[3]=addr & 0xff;

    SPIFlashWriteEnable(1);  

    spi_write(spi_flash, tx_buf, 4);

    SPIFlashWaitWhenBusy(); 
}

/* program */
void SPIFlashProgram(unsigned int addr, unsigned char *buf, int len)
{
    
    unsigned  char tx_buf[4];
    
    struct spi_transfer	t[] = {
        {
            .tx_buf	= tx_buf,
            .len	= 4,
        },
        {
            .tx_buf	= buf,
            .len	= len,
        },
    };
	struct spi_message	m;

    tx_buf[0]= 0x02;
    tx_buf[1]=addr >> 16;
    tx_buf[2]=addr >> 8;
    tx_buf[3]=addr & 0xff;

    SPIFlashWriteEnable(1);  
    spi_message_init(&m);
	spi_message_add_tail(&t[0], &m);
    spi_message_add_tail(&t[1], &m);
    spi_sync(spi_flash, &m);
    SPIFlashWaitWhenBusy();
    
}

void SPIFlashRead(unsigned int addr, unsigned char *buf, int len)
{
     
    unsigned  char tx_buf[4];
    
    struct spi_transfer	t[] = {
        {
            .tx_buf	= tx_buf,
            .len	= 4,
        },
        {
            .rx_buf	= buf,
            .len	= len,
        },
    };
	struct spi_message	m;

    tx_buf[0]= 0x03;
    tx_buf[1]=addr >> 16;
    tx_buf[2]=addr >> 8;
    tx_buf[3]=addr & 0xff;

       
    spi_message_init(&m);
	spi_message_add_tail(&t[0], &m);
    spi_message_add_tail(&t[1], &m);
    spi_sync(spi_flash, &m);
     
   
}


static  void SPIFlashInit(void)
{
    SPIFlashClearProtectForStatusReg();
    SPIFlashClearProtectForData();
}
    
static struct mtd_info spi_flash_dev;

static int __devinit spi_flash_erase(struct mtd_info *mtd, struct erase_info *instr)
{
    unsigned int addr = instr->addr;
    unsigned int len =0;
    /*判断参数*/
    if((addr & (spi_flash_dev.erasesize -1))||(instr->len & (spi_flash_dev.erasesize -1)) )
    {
        return -EINVAL; 
    }
    for(len=0;len<instr->len;len+=4096)
    {
        SPIFlashEraseSector(addr);
        addr+=4096;
    }
    
    instr->state = MTD_ERASE_DONE;
    mtd_erase_callback(instr);

    return 0;
}

static int spi_flash_read(struct mtd_info *mtd, loff_t from, size_t len,
		      size_t *retlen, u_char *buf)
{
    SPIFlashRead(from  ,buf,   len) ;
    *retlen = len;
    return 0 ;
}

static int spi_flash_write(struct mtd_info *mtd, loff_t to, size_t len,
		       size_t *retlen, const u_char *buf)
{
    unsigned int addr = to;
    unsigned int wlen =0;
    /*判断参数*/
    if((to & (spi_flash_dev.erasesize -1))||(len & (spi_flash_dev.erasesize -1)) )
    {
        return -EINVAL; 
    }

    for (wlen =0;wlen<len;wlen+=256)
    {
        SPIFlashProgram(addr, (unsigned char *) buf,wlen);
        addr+=256;
        buf+=256;
    }
    *retlen = len;
    return 0 ;
}

static int __devinit spi_flash_probe(struct spi_device * spi)
{
    int mid,did;
    /*
     * 构造注册一个mtd_info      
     * mts_device_register(master,parts,nr_parts);
     */
    spi_flash = spi;
    
  

    
    spi_flash_dev.name = "spi_flash";
    spi_flash_dev.type = MTD_NORFLASH;
    spi_flash_dev.flags = MTD_CAP_NANDFLASH;
    spi_flash_dev.size = 0x200000;
    spi_flash_dev.writesize = 1;
    spi_flash_dev.writebufsize = 4096;
    spi_flash_dev.erasesize = 4096;
     
    spi_flash_dev.owner = THIS_MODULE;
    spi_flash_dev._erase = spi_flash_erase;
    spi_flash_dev._read = spi_flash_read;
    spi_flash_dev._write = spi_flash_write;
 
    mtd_device_register(&spi_flash_dev, NULL , 0);

    s3c2410_gpio_cfgpin(spi->chip_select,S3C2410_GPIO_OUTPUT);
    SPIFlashInit();
    SPIFlashReadID(&mid, &did);
    printk("SPI flash ID: %02x   %02x\n",mid,did);
    printk("[%s][%d]\n",__FUNCTION__,__LINE__);
    return 0;
}
static int __devinit spi_flash_remove(struct spi_device *spi)
{
    mtd_device_unregister(&spi_flash_dev );
    printk("[%s][%d]\n",__FUNCTION__,__LINE__);
    return 0;
}



static struct spi_driver spi_flash_drv = {
    .driver = {
        .name = "spi_flash",
        .owner = THIS_MODULE,
    },
    .probe = spi_flash_probe,
    .remove  = __devexit_p(spi_flash_remove),
};


static int spi_flash_init(void)
{
    printk("[%s][%d]\n",__FUNCTION__,__LINE__);
    spi_register_board_info(s3c2440_spi_info,ARRAY_SIZE(s3c2440_spi_info));
    return spi_register_driver(&spi_flash_drv);
}
static int spi_flash_exit(void)
{
    printk("[%s][%d]\n",__FUNCTION__,__LINE__);
    spi_unregister_driver(&spi_flash_drv);

     
}



module_init(spi_flash_init);
module_exit(spi_flash_exit);
MODULE_LICENSE("GPL");









 





