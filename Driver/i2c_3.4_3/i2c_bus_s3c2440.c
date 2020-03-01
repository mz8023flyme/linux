#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/i2c.h>
#include <linux/init.h>
#include <linux/time.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/platform_device.h>
#include <linux/pm_runtime.h>
#include <linux/clk.h>
#include <linux/cpufreq.h>
#include <linux/slab.h>
#include <linux/io.h>
#include <linux/of_i2c.h>
#include <linux/of_gpio.h>
#include <plat/gpio-cfg.h>
#include <mach/regs-gpio.h>

#include <asm/irq.h>

#include <plat/regs-iic.h>
#include <plat/iic.h>


enum s3c24xx_i2c_state {
	STATE_IDLE,
	STATE_START,
	STATE_READ,
	STATE_WRITE,
	STATE_STOP
};


struct s3c2440_i2c_regs{
    unsigned int iiccon;
    unsigned int iicstat;
    unsigned int iicadd;
    unsigned int iicds;
    unsigned int iiclc;
        
};
static struct s3c2440_i2c_regs * s3c2440_i2c_reg;

struct s3c2440_i2c_xfer_data{
    struct i2c_msg *msgs;
    int msg_num;
    int cur_msg;
    int state;
    int err;
    int cur_ptr;
    wait_queue_head_t wait;
};

static struct s3c2440_i2c_xfer_data s3c2440_i2c_xfer_data;

static int islastmsg(void)
{
    return (s3c2440_i2c_xfer_data.cur_msg==s3c2440_i2c_xfer_data.msg_num-1);
}
static int isEndData(void)
{
    return (s3c2440_i2c_xfer_data.cur_ptr>=(s3c2440_i2c_xfer_data.msgs->len-1));
}
static int islastdata(void)
{
    return (s3c2440_i2c_xfer_data.cur_ptr>=(s3c2440_i2c_xfer_data.msgs->len));
}


static void s3c2440_i2c_init(void)
{
    struct clk* clk;
    clk=clk_get(NULL,"i2c");
    clk_enable(clk);
    
    s3c2410_gpio_cfgpin(S3C2410_GPE(14),S3C2410_GPE14_IICSCL);
    s3c2410_gpio_cfgpin(S3C2410_GPE(15), S3C2410_GPE15_IICSDA);


    /* bit[7] = 1, 使能ACK
     * bit[6] = 0, IICCLK = PCLK/16
     * bit[5] = 1, 使能中断
     * bit[3:0] = 0xf, Tx clock = IICCLK/16
     * PCLK = 50MHz, IICCLK = 3.125MHz, Tx Clock = 0.195MHz
     */
    s3c2440_i2c_reg->iiccon = (1<<7) | (0<<6) | (1<<5) | (0xf);  // 0xaf

    s3c2440_i2c_reg->iicadd  = 0x10;     // S3C24xx slave address = [7:1]
    s3c2440_i2c_reg->iicstat = 0x10;     // I2C串行输出使能(Rx/Tx)
}


static void s3c2440_i2c_start(void)
{

   s3c2440_i2c_xfer_data.state= STATE_START;

    if(s3c2440_i2c_xfer_data.msgs->flags & I2C_M_RD) //读
    {
          s3c2440_i2c_reg->iicds = s3c2440_i2c_xfer_data.msgs->addr<<1;
          s3c2440_i2c_reg->iicstat = 0xb0; //主机接收启动
    }
    else // 写
    {
        s3c2440_i2c_reg->iicds = s3c2440_i2c_xfer_data.msgs->addr<<1;
        s3c2440_i2c_reg->iicstat = 0xf0; //主机接收启动
    }
}
static void s3c2440_i2c_stop(int err)
{
    s3c2440_i2c_xfer_data.state= STATE_STOP;
    s3c2440_i2c_xfer_data.err = err;
    if(s3c2440_i2c_xfer_data.msgs->flags & I2C_M_RD) /*读*/
    {
        s3c2440_i2c_reg->iicstat = 0x90;
        s3c2440_i2c_reg->iiccon = 0xaf;
        ndelay(50);
    }
    else
    {
        s3c2440_i2c_reg->iicstat = 0xd0;
        s3c2440_i2c_reg->iiccon=0xaf;
        ndelay(50);
    }
    /*唤醒*/
    wake_up(&s3c2440_i2c_xfer_data.wait);
}


static irqreturn_t s3c2440_i2c_irq(int irq,void* dev_id)
{
    unsigned int iicSt ;
    iicSt  = STATE_START; 
    if(iicSt & 0x8){ printk("Bus arbitration failed\n\r"); }
    switch (s3c2440_i2c_xfer_data.state)
    {
        case STATE_START: /*发出S和设备地址后产生中断*/
        {
            /*如果没有ACK，返回错误*/
            if(iicSt & S3C2410_IICSTAT_LASTBIT)
            {
                s3c2440_i2c_stop(-ENODEV);
                break;
            }
            if(islastmsg()&&isEndData())
            {
                s3c2440_i2c_stop(-ENODEV);
                break;
            }
            /*进入下一个状态*/
            if(s3c2440_i2c_xfer_data.msgs->flags & I2C_M_RD)
            {
                s3c2440_i2c_xfer_data.state = STATE_READ;
                goto next_read;
            }
            else
            {
                s3c2440_i2c_xfer_data.state = STATE_WRITE;
            }
        }
        case STATE_WRITE:
        {
            if(s3c2440_i2c_reg->iicstat & S3C2410_IICSTAT_LASTBIT)
            {
                s3c2440_i2c_stop(-ENODEV);
                break;

            }
            if(!isEndData())
            {
                s3c2440_i2c_reg->iicds = s3c2440_i2c_xfer_data.msgs->buf[s3c2440_i2c_xfer_data.cur_ptr];
                 s3c2440_i2c_xfer_data.cur_ptr++;
                 ndelay(50);
                 s3c2440_i2c_reg->iiccon = 0xaf;
                 break;
            }
            else if (!islastmsg())
            {
                /* 开始处理下一个消息*/
                s3c2440_i2c_xfer_data.msgs++;
                s3c2440_i2c_xfer_data.cur_ptr = 0;
                s3c2440_i2c_xfer_data.cur_msg++;
                s3c2440_i2c_xfer_data.state=STATE_START;
                /*开始发出起始信号和发出设备地址*/
                s3c2440_i2c_start();
                break;
            }
            else
            {
                /*是最后一个消息的最后一个数据*/
                s3c2440_i2c_stop(0);
                break;
            }
        }
        case STATE_READ:
        {
            /*读出数据*/
            s3c2440_i2c_xfer_data.msgs->buf[s3c2440_i2c_xfer_data.cur_ptr]=s3c2440_i2c_reg->iicds;
            s3c2440_i2c_xfer_data.cur_ptr++;
            
            next_read:
            if(!isEndData())
            {
                /*如果数据没读完继续读*/
                if(islastdata())
                {
                    s3c2440_i2c_reg->iiccon = 0x2f;
                    
                }
                else
                {
                    s3c2440_i2c_reg->iiccon = 0xaf;
                }
                break;
            }
            else if (!islastmsg())
            {
                /* 开始处理下一个消息*/
                s3c2440_i2c_xfer_data.msgs++;
                s3c2440_i2c_xfer_data.cur_ptr = 0;
                s3c2440_i2c_xfer_data.cur_msg++;
                s3c2440_i2c_xfer_data.state=STATE_START;
                /*开始发出起始信号和发出设备地址*/
                s3c2440_i2c_start();
                break;
            }
            else
            {
                s3c2440_i2c_stop(0);
                 break;
            }
        }

        default: break;
    }
    
    s3c2440_i2c_reg->iiccon &= ~(S3C2410_IICCON_IRQPEND);
  

    return IRQ_HANDLED;
}


static int s3c2440_i2c_xfer(struct i2c_adapter *adap, struct i2c_msg *msgs,
			   int num)
{
    //把num 个msg 的I2C数据发送出去
    unsigned int timeout;
    s3c2440_i2c_xfer_data.msgs = msgs;
    s3c2440_i2c_xfer_data.msg_num = num;
    s3c2440_i2c_xfer_data.cur_msg = 0;
    s3c2440_i2c_xfer_data.cur_ptr=0;
    s3c2440_i2c_xfer_data.err= -ENODEV;
    s3c2440_i2c_start( );
    /*休眠*/
    timeout= wait_event_timeout(s3c2440_i2c_xfer_data.wait,(s3c2440_i2c_xfer_data.state==STATE_STOP),HZ/5);
    if(0==timeout)
    {
        printk("s3c2440_i2c_xfer timeout");
        return -ETIMEDOUT;
    } 
    else
    {
        return s3c2440_i2c_xfer_data.err ;

    }
}


static u32 s3c2440_i2c_func(struct i2c_adapter *adap)
{
	return I2C_FUNC_SMBUS_EMUL|I2C_FUNC_I2C|I2C_FUNC_PROTOCOL_MANGLING;
}

static const struct i2c_algorithm s3c2440_i2c_algo = {
    .master_xfer   = s3c2440_i2c_xfer,
    .functionality = s3c2440_i2c_func,
};

/* 1.分配设置i2c_adapter
 *   
 */
static struct i2c_adapter s3c2440_i2c_adapter={
    .name = "s3c2440",
    .algo =&s3c2440_i2c_algo,
    .owner = THIS_MODULE,
};


static int i2c_bus_s3c2440_init(void)
{
    /*2.硬件相关的设置*/
    s3c2440_i2c_reg=ioremap(0x54000000, sizeof(struct s3c2440_i2c_regs));
    s3c2440_i2c_init();

    request_irq(IRQ_IIC,s3c2440_i2c_irq,0,"s3c2440_i2c_irq",NULL);

    init_waitqueue_head(&s3c2440_i2c_xfer_data.wait);
    /*3.注册i2c_adapter*/
    i2c_add_adapter(&s3c2440_i2c_adapter);
    return 0 ;
}
static void i2c_bus_s3c2440_exit(void)
{
    
    i2c_del_adapter(&s3c2440_i2c_adapter);
    free_irq(IRQ_IIC,NULL);
    iounmap(s3c2440_i2c_reg);
}
module_init(i2c_bus_s3c2440_init);
module_exit(i2c_bus_s3c2440_exit);
MODULE_LICENSE("GPL");



