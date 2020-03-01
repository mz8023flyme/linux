#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/fb.h>
#include <linux/init.h>
#include <linux/dma-mapping.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <linux/wait.h>
#include <linux/platform_device.h>
#include <linux/clk.h>

#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/div64.h>

#include <asm/mach/map.h>
#include <asm/arch/regs-lcd.h>
#include <asm/arch/regs-gpio.h>
#include <asm/arch/fb.h>

struct lcd_regs{
	unsigned long lcdcon1;
	unsigned long lcdcon2;
	unsigned long lcdcon3;
	unsigned long lcdcon4;
	unsigned long lcdcon5;
	unsigned long lcdsaddr1;
	unsigned long lcdsaddr2;
	unsigned long lcdsaddr3;
	unsigned long redlut;
	unsigned long greenlut;
	unsigned long bluelut;
	unsigned long reserved[9];
	unsigned long dithmod;
	unsigned long tpal;
	unsigned long lcdintpnd;
	unsigned long lcdsrcpnd;
	unsigned long lcdintpmsk;
	unsigned long lpcsel;
	
};

static int lcd_lcdfb_setcolreg(unsigned int regno, unsigned int red, unsigned int green, unsigned int blue, unsigned int transp, struct fb_info * info)
{
	unsigned int val;
	if(regno > 16)
		return 1;

	/*RGB*/
	val = chan_to_field(red, &info->var.red) ;
	val = chan_to_field(green, &info->var.red) ;
	val = chan_to_field(blue, &info->var.red) ;
	((u32 *)(info->pseudo_palette))[regno] = val;
	return 0;

	
}



static struct fb_info *lcd;
static struct fb_ops lcd_fb_ops={
	.owner        =  THIS_MODULE,
	.fb_setcolreg = lcd_lcdfb_setcolreg,
	.fb_fillrect  =  cfb_fillrect,
	.fb_copyarea  =  cfb_copyarea,
	.fb_imageblit =  cfb_imageblit,	
};


static volatile  unsigned  long *gpbcon;
static volatile  unsigned  long *gpbdat;
static volatile  unsigned  long *gpccon;
static volatile  unsigned  long *gpdcon;
static volatile  unsigned  long *gpgcon;

static volatile struct lcd_regs* lcd_regs;
static u32 pseudo_palette [16];




static int lcd_init()
{
	/*1、分配一个fb_info*/

	lcd=framebuffer_alloc(0,NULL);

	/*2、设置*/
	/*2.1设置固定的参数*/
	strcpy(&lcd->fix,"mylcd");
	lcd->fix.smem_len = 204*320*16;		//显存的长度
	lcd->fix.type     = FB_TYPE_PACKED_PIXELS;
	lcd->fix.visual   = FB_VISUAL_DIRECTCOLOR;
	lcd->fix.line_length= 240*16;
		 
	/*2.2设置可变的参数*/
	lcd->var.xres    		= 240;
	lcd->var.yres 			= 320;
	lcd->var.xres_virtual	= 240;
	lcd->var.yres_virtual	= 320;
	lcd->var.bits_per_pixel	= 16;	

	/*RGB*/
	lcd->var.red.offset = 11;
	lcd->var.red.length = 5;
	
	lcd->var.green.offset = 5;
	lcd->var.green.length = 6;

	lcd->var.blue.offset = 0;
	lcd->var.blue.length = 5;


	lcd->var.activate = FB_ACTIVATE_NOW;
	/*2.3设置操作*/
	lcd->fbops  = &lcd_fb_ops;

	/*2.4其他的设置*/
//	lcd->screen_base  = pseudo_palette ;  /显存的虚拟地址*/
	lcd->pseudo_palette =pseudo_palette ;
	lcd->screen_size  = 240*320*2;

	
	/*3、 硬件相关的设置*/
	/*3.1 配置GPIO哟用于LCD  */
	gpbcon = ioremap(0x56000010, 8);
	gpbdat = gpbcon+1;
	gpccon = ioremap(0x56000020, 4);
	gpdcon = ioremap(0x56000020, 4);
	gpgcon = ioremap(0x56000060, 4);

	*gpccon = 0xaaaaaaaa;  /* 屏幕需要的引脚  */ 
	*gpdcon = 0xaaaaaaaa;
	
	*gpbcon &=~(3);  /*GPIOB设置输出引脚*/
	*gpbcon |=1;
	*gpbdat &=~1;   /*输出低电平*/

	*gpgcon |=(3<<8);
	
	/*3.2 根据LCD手册设置LCD控制器*/

	lcd_regs = ioremap(0x4D000000, sizeof(struct lcd_regs));

	/*  */
	lcd_regs->lcdcon1   =   (4<<8)|(3<<5)|(0x0c<<1);
	/*垂直方向上的时间参数*/
	lcd_regs->lcdcon2   =   (3<<24)|(319<<14)|(1<<6)|(0<<0) ;
	/*垂直方向上的时间参数*/
	lcd_regs->lcdcon3   =  (16<<19)|(239<<8)|(10<<0);
	/*水平方向上的同步信号*/
	lcd_regs->lcdcon4   =  4;

	lcd_regs->lcdcon5   =  (1<<11)|(0<<10)|(1<<9)|(1<<8)|(1<<0);
	
	/*3.3 分配显存*/
	lcd->screen_base =dma_alloc_writecombine(NULL, lcd->fix.smem_len, &lcd->fix.smem_start, GFP_KERNEL);
	lcd_regs->lcdsaddr1 = (lcd->fix.smem_start>>1) & ~(3<30)  ; 
	lcd_regs->lcdsaddr2 = ((lcd->fix.smem_start + lcd->fix.smem_len) >> 1) &  0x1fffff;
	lcd_regs->lcdsaddr3 = (240*16/16);//一行的长度单位是半字

	lcd_regs->lcdcon1  |=(1<<0);
	lcd_regs->lcdcon5  |=(1<<3);  	
	*gpbdat |=1;   /*使能背光灯*/
	


	
	/*4、注册*/

	register_framebuffer(lcd);
	
	return 0;
}



static void lcd_exit()
{
	
	unregister_framebuffer(lcd);
	lcd_regs->lcdcon1 &=~(1<<0);/*关闭LCD*/
	*gpbdat &=~1;		/*关掉背光*/
	dma_free_writecombine(NULL,lcd->fix.smem_len,lcd->screen_base,lcd->fix.smem_start);
	iounmap(lcd_regs);
  	iounmap(gpbcon);
 	iounmap(gpccon);
 	iounmap(gpdcon);
 	iounmap(gpgcon);
	framebuffer_release(lcd);

	
}

module_init(lcd_init);
module_exit(lcd_exit);
MODULE_LICENSE("GPL");



























