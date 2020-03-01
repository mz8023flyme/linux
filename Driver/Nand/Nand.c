
/*
 * drives\mtd\nand\s3c2410.c
 * drives\mtd\nand\at91_nand.c
 */
#include <linux/module.h>
#include <linux/types.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/ioport.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/clk.h>
    
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/nand_ecc.h>
#include <linux/mtd/partitions.h>
    
#include <asm/io.h>
    
#include <asm/arch/regs-nand.h>
#include <asm/arch/nand.h>

#define TACLS  0 
#define TWRPH0 1
#define TWRPH1 0


struct s3c_nand_regs{
     unsigned long    nfcont    ; 
     unsigned long    nfcmd     ; 
     unsigned long    nfaddr    ; 
     unsigned long    nfdata    ; 
     unsigned long    nfeccd0   ; 
     unsigned long    nfeccd1   ; 
     unsigned long    nfeccd    ; 
     unsigned long    nfstat    ; 
     unsigned long    nfestat0  ; 
     unsigned long    nfestat1  ; 
     unsigned long    nfmecc0   ; 
     unsigned long    nfmecc1   ; 
     unsigned long    nfsecc    ; 
     unsigned long    nfsblk    ; 
     unsigned long    nfeblk    ; 

};
static struct nand_chip * bsp_nand;
static struct mtd_info * bsp_mtd;
static struct s3c_nand_regs * s3c_nand_regs;



static struct mtd_partition s3c_nand_parts[]={
    [0]={
        .name = "bootoader",
        .size = 0x00040000,
        .offset = 0,
    },
    [1]={
        .name = "params",
        .size = 0X00020000,
        .offset= MTDPART_OFS_APPEND,
    },
    [0]={
        .name = "kernel",
        .size = 0x00200000,
        .offset=MTDPART_OFS_APPEND,
    },
    [0]={
        .name = "root",
        .size = MTDPART_SIZ_FULL,
        .offset= MTDPART_OFS_APPEND,
    }
};

 
static void s3c2440_select_chip(struct mtd_info * mtd,int chipnr)
{
    if(chipnr == -1)
    {
        /*取消选中 NFCOTN[1]设为1*/
        s3c_nand_regs->nfcont |=  (1<<1);
        
    }
    else
    {
        /*选中NFCOTN[1]设为0*/
        s3c_nand_regs->nfcont &= ~(1<<1);
    }
}

static void s3c_cmd_ctrl(struct mtd_info *mtd, int data, unsigned int ctrl)
{

	if (ctrl & NAND_CLE)
	{
        /*发命令NFCMD =DATA*/
        s3c_nand_regs->nfcmd = data ;
	}
	else
	{
        /*发命令NFCMD =DATA*/
        s3c_nand_regs->nfaddr = data ;
	}
}

static int s3c_dev_ready(struct mtd_info *mtd)
{
    return (s3c_nand_regs->nfstat & (1<<0));
}

static int  nand_init()
{
    struct clk * clk;
    /*1.分配一个nand_chip结构体*/
    bsp_nand = kzalloc(sizeof(struct nand_chip), GFP_KERNEL);
    s3c_nand_regs = ioremap(0x4E000000,sizeof(struct s3c_nand_regs));
    /*2.设置*/
    bsp_nand->select_chip = s3c2440_select_chip;
    bsp_nand->cmd_ctrl = s3c_cmd_ctrl;
    bsp_nand->IO_ADDR_R =  &s3c_nand_regs->nfdata;
    bsp_nand->IO_ADDR_W =  &s3c_nand_regs->nfdata;
    bsp_nand->dev_ready = s3c_dev_ready;
    bsp_nand->ecc.mode = NAND_ECC_SOFT;
    /*3.硬件相关的操作*/
    clk_get(NULL, "nand");
    clk_enable(clk);
    s3c_nand_regs->nfcont = (TACLS<<12)|(TWRPH0<<8)|(TWRPH1<<4);
    s3c_nand_regs->nfcont = (1<<1)|(1<<0);
     
    
    /*4.使用： nand_scan*/
    bsp_mtd = kzalloc(sizeof(struct mtd_info), GFP_KERNEL);
    
    bsp_mtd->owner = THIS_MODULE;
    bsp_mtd->priv = bsp_nand;

    
    nand_scan(bsp_mtd,1);
    /*5.add_mtd_partitions*/
    add_mtd_partitions(bsp_mtd,s3c_nand_parts,4);
    //add_mtd_device
    
}

static void nand_exit()
{
    //del_mtd_partitions(mtd);
    kfree(bsp_mtd);
    iounmap(s3c_nand_regs);
    kfree(bsp_nand);
    del_mtd_partitions(bsp_mtd);
    
}



module_init(nand_init);
module_exit(nand_exit);
MODULE_LICENSE("GPL");
 
   





























