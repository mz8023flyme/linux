/*
 * drivers/block/xd.c
 * drivers/block/z2ram.c
 */
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/interrupt.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/timer.h>
#include <linux/genhd.h>
#include <linux/hdreg.h>
#include <linux/ioport.h>
#include <linux/init.h>
#include <linux/wait.h>
#include <linux/blkdev.h>
#include <linux/blkpg.h>
#include <linux/delay.h>
#include <linux/io.h>

#include <asm/system.h>
#include <asm/uaccess.h>
#include <asm/dma.h>


static struct gendisk * ramblock_disk;
static request_queue_t * ramblock_queue;
static DEFINE_SPINLOCK(ramblock_lock);
static int maior; 
static unsigned char * ramblock_buf;

#define REMBLOCK_SIZE  (1024*1024)

static int ramblock_fetgeo(struct block_device *bdev, struct hd_geometry *geo)
{

	geo->heads = 2;
	
	geo->cylinders = 32;
    geo->sectors = REMBLOCK_SIZE/512/2/32;
	return 0;
}



static struct block_device_operations ramblock_fops = {
	.owner	= THIS_MODULE,
    .getgeo = ramblock_fetgeo,
};


static void ramblock_requesr (request_queue_t * q)
{
   
    static int r_cnt = 0 ;    
    static int w_cnt = 0 ;

    struct request *req;
 //   printk("ramblock_requesr %d\n",cnt++);
    while ((req = elv_next_request(q)) != NULL) 
    {
        /*数据传输三要素 源 目的 长度*/
        /*源/目的*/
        unsigned long offset = req->sector<<9;
        /*目的/源*/
        //ramblock_buf
        
        /*长度*/
        unsigned long len =  req->current_nr_sectors<<9;

        if(rq_data_dir(req)== READ)
        {
            printk("ramblock_requesr read%d\n",r_cnt++);
            memcpy(req->buffer,ramblock_buf+offset,len);
        }
        else
        {
            printk("ramblock_requesr write%d\n",w_cnt++);
            memcpy(ramblock_buf+offset,req->buffer,len);
        }

        
        end_request(req, 1);
    }
}

static int  ramblock_init(void)
{
    /*1、分配 gendisk:alloc_disk */
    ramblock_disk = alloc_disk(5);
    /*2、设置*/
    /*2.1、分配/设置队列 提供读写能力*/
    ramblock_queue = blk_init_queue(ramblock_requesr , & ramblock_lock);
    ramblock_disk->queue = ramblock_queue;
    /*2.2、设置其他属性：比如容量*/
    maior = register_blkdev(0, "ramblock");
    
    ramblock_disk->major = maior;
	ramblock_disk->first_minor = 0;
	sprintf(ramblock_disk->disk_name,"ramblock");
 	ramblock_disk->fops = &ramblock_fops;
    set_capacity(ramblock_disk, REMBLOCK_SIZE/512);//以扇区为单位

    /*3.硬件相关的操作*/
    ramblock_buf = kzalloc(REMBLOCK_SIZE, GFP_KERNEL);
    /* 4.注册*/
    add_disk(ramblock_disk);
    return 0; 
}

static void  ramblock_exit(void)
{
    unregister_blkdev(maior,"ramblock");
    del_gendisk(ramblock_disk);
    put_disk(ramblock_disk);
    blk_cleanup_queue(ramblock_queue);
    kfree(ramblock_buf);


    
}



module_init(ramblock_init);
module_exit(ramblock_exit);
MODULE_LICENSE("GPL");






























































