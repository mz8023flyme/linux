#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/irq.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <asm/arch/regs-gpio.h>
#include <asm/hardware.h>
#include <linux/poll.h>
#include <linux/dma-mapping.h>



#define MEM_CP_NO_DMA  0
#define MEM_CP_DMA     1
#define DMA_LEN        (512*1024)

#define DMA0_BASS_ADDR   0x4B000000
#define DMA1_BASS_ADDR   0x4B000040
#define DMA2_BASS_ADDR   0x4B000080
#define DMA3_BASS_ADDR   0x4B0000C0




struct s3c_dma_regs {
	unsigned long disrc;
	unsigned long disrcc;
	unsigned long didst;
	unsigned long didstc;
	unsigned long dcon;
	unsigned long dstat;
	unsigned long dcsrc;
	unsigned long dcdst;
	unsigned long dmasktrig;
};


static volatile struct s3c_dma_regs *dma_regs;

static DECLARE_WAIT_QUEUE_HEAD(dma_waitq);
static volatile int ev_dma= 0 ;

static int major = 0;

static char *src;
static u32 src_phys;
static u32 dst_phys;
static char * dst;

static struct class *cls;

static int dma_2440_ioctl(struct inode *inode, struct file *file, unsigned int cmd , unsigned long arg)
{
    int i;
    
    memset(src,0xaa,DMA_LEN);
    memset(dst,0x55,DMA_LEN);
    
    switch (cmd)
    {
        case MEM_CP_NO_DMA:
        {
            for(i=0;i<DMA_LEN;i++)
            {   
                dst[i]=src[i];
            }
            if(memcmp(src,dst,DMA_LEN)==0)
            {
                printk("MEM_CP_NO_DMA success\n");
            }
            else
            {
                 printk("MEM_CP_NO_DMA fail\n");
            }
            break;
        }
        case MEM_CP_DMA :
        {
            ev_dma = 0;
            /*把源 目的 长度 告诉dma*/
            dma_regs->disrc  = src_phys;            //源的物理地址
            dma_regs->disrcc = (0<<1)|(0<<0);       //源位于AHBA总线，地址递增
            dma_regs->didst  = dst_phys;            //目的的物理地址
            dma_regs->didstc = (0<<2)|(0<<1)|(0<<0);        //目的位于AHB总线，地址递增
            dma_regs->dcon   = (1<<30)|(1<<29)|(1<<27)|(0<<28)|(0<<23)|(0<<20)|(DMA_LEN<<0);     //使能中断，单个传输，软件触发

            /*启动DMA*/
            dma_regs->dmasktrig = (1<<1)|(1<<0); 

            /*什么时候结束*/
            
            /*休眠*/
            wait_event_interruptible(dma_waitq, ev_dma);
            if(memcmp(src,dst,DMA_LEN)==0)
            {
                printk("MEM_CP_DMA success\n");
            }
            else
            {
                 printk("MEM_CP_DMA fail\n");
            }
            
            break; 
        }
    }
}

static struct file_operations dma_fops = {
        .owner = THIS_MODULE,
        .ioctl = dma_2440_ioctl,
};

static irqreturn_t dma_2440_irq(int irq,void * devid)
{
    ev_dma = 1 ;
    wake_up_interruptible(&dma_waitq);
    return IRQ_HANDLED;
}



static int dma_2440_init(void)
{
    if(request_irq(IRQ_DMA3,dma_2440_irq,0,"dmairq",1))
    {
        printk("dma_irq fail\n");
        
        return -EBUSY;
    }
    /*分配缓冲区*/
    src = dma_alloc_writecombine(NULL , DMA_LEN, &src_phys, GFP_KERNEL);
    if(src == NULL)
    {
        free_irq(IRQ_DMA3,1);
        printk("not alloc buffer for src\n");
        return -ENOMEM;
    }
    dst = dma_alloc_writecombine(NULL , DMA_LEN, &dst_phys, GFP_KERNEL);
    if(dst  == NULL)
    {
        free_irq(IRQ_DMA3,1);
        dma_free_writecombine(NULL, DMA_LEN, src, src_phys);
        printk("not alloc buffer for dst \n");
        return -ENOMEM;
    }
    major = register_chrdev(0,"dma", &dma_fops);
    /*为了自动创建设备节点*/
    cls = class_create( THIS_MODULE, "dma");
    class_device_create(cls,NULL,MKDEV(major,0),NULL,"dma");

    dma_regs = ioremap(DMA3_BASS_ADDR, sizeof(struct s3c_dma_regs));

    
    return 0;
}
static void dma_2440_exit(void)
{
    iounmap(dma_regs);
    dma_free_writecombine(NULL, DMA_LEN, src, src_phys);
    dma_free_writecombine(NULL, DMA_LEN, dst, dst_phys);
    unregister_chrdev(major,"dma");
    class_device_destroy(cls, MKDEV(major,0));
    class_destroy(cls);
    free_irq(IRQ_DMA3,1);
}


module_init(dma_2440_init);
module_exit(dma_2440_exit);

MODULE_LICENSE("GPL");











