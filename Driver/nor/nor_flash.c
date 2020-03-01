/*
 * 参考 drive\mtd\maps\physamp.c  
 */

#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/map.h>
#include <linux/mtd/partitions.h>
#include <linux/mtd/physmap.h>
#include <asm/io.h>

static struct map_info * nor_map;
static struct mtd_info * nor_mtd;

static struct mtd_partition s3c_nor_parts[]={
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


static int nor_flash_init(void)
{
    /*1.分配一个map_info结构体*/
    nor_map = kzalloc(sizeof(struct map_info), GFP_KERNEL);
    
    /*2，设置：物理基地址（phy）大小（size），位宽（bankwidth），虚拟基地址（virtual）*/
    nor_map->name = "s3c_nor";
    nor_map->phys = 0;
    nor_map->size =  0x1000000;
    nor_map->bankwidth = 2;
    //nor_map->set_vpp = physmap_data->set_vpp;
    nor_map->virt = ioremap(nor_map->phys, nor_map->size);

    simple_map_init(nor_map);

    /*3.使用：调用 NOR FLASH协议层提供的函数来识别*/
    printk("cfi_prob\n");
    nor_mtd = do_map_probe("cfi_prob", nor_map);
    if(!nor_mtd)
    {
        printk("jedec_probe\n");
        nor_mtd = do_map_probe("jedec_probe", nor_map);

    }
    if(!nor_mtd)
    {
        kfree(nor_map);
        iounmap(nor_map->virt);
        return -EIO;
    }
    
    /*4。 add_mtd_partitions*/
   add_mtd_partitions(nor_mtd,s3c_nor_parts,4);


    
    return 0;    
}
static void nor_flash_exit(void)
{
    del_mtd_partitions(nor_mtd );
    kfree(nor_map);
    iounmap(nor_map->virt);

}
module_init(nor_flash_init);
module_exit(nor_flash_exit);
MODULE_LICENSE("GPL");
 






























