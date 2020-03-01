#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/cdev.h>

/*1.确定主设备号*/
static int major;

/*2.构造 file_opertaions 结构体*/

static int char_dev_open (struct inode *inode, struct file *file)
{
    printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
    return 0 ;
}
static int char_dev_open2 (struct inode *inode, struct file *file)
{
    printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
    return 0 ;
}

static struct cdev char_cdev ;
static struct cdev char_cdev2 ;

static struct class * cls;
  
static struct file_operations chardev= {
    .owner = THIS_MODULE,
    .open = char_dev_open,
};
static struct file_operations chardev2= {
    .owner = THIS_MODULE,
    .open = char_dev_open2,
};


static int char_dev_init()
{
    dev_t devid;
    /*告诉内核*/
    if(major)
    {
        devid = MKDEV(major,0);
        register_chrdev_region(devid,2,"char_dev");
    }
    else
    {
        alloc_chrdev_region(&devid,0, 2,"char_dev");
        major = MAJOR(devid);
    }
    cdev_init(&char_cdev,&chardev);
    cdev_add(&char_cdev,devid, 2 );
    
    devid = MKDEV(major,2);
    register_chrdev_region(devid,1,"char_dev2");
    cdev_init(&char_cdev2,&chardev2);
    cdev_add(&char_cdev2,devid, 1 );


    
    cls = class_create(THIS_MODULE, "char_dev");
    class_device_create(cls, NULL,MKDEV(major,0), NULL, "char_dev0");
    class_device_create(cls, NULL,MKDEV(major,1), NULL, "char_dev1");
    class_device_create(cls, NULL,MKDEV(major,2), NULL, "char_dev2");

    class_device_create(cls, NULL,MKDEV(major,3), NULL, "char_dev3");


    return 0 ;
}
static void char_dev_exit()
{
    class_device_destroy(cls, MKDEV(major,0));  
    class_device_destroy(cls, MKDEV(major,1)); 
    class_device_destroy(cls, MKDEV(major,2)); 
    class_device_destroy(cls, MKDEV(major,3)); 
    class_destroy(cls);
    cdev_del(&char_cdev);
    unregister_chrdev_region(MKDEV(major, 0), 2);

    cdev_del(&char_cdev2);
    unregister_chrdev_region(MKDEV(major, 2), 1);
}

module_init(char_dev_init);
module_exit(char_dev_exit);
MODULE_LICENSE("GPL");




