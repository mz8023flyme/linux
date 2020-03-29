#include "config.h"
#include "disp_manager.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <string.h>


static int g_iFBfd;
static struct fb_var_screeninfo g_tVar;	/* Current var */
static struct fb_fix_screeninfo g_tFix;	/* Current fix */
static int g_FBScreenSize;
static unsigned char *g_FBMem;
static unsigned int g_LineWidth;
static unsigned int g_iPixelWidth;


static int FBDevicesInit(void);
static int FBShowPixel(int iPenX,int iPenY,unsigned int dwColor);
static int FBCleanScreen(unsigned int dwBackColor);


/* 构造分配设置注册一个结构体         */

static T_DispOpr g_tFBDispOpr ={
	.name 		 = 	"fb",
	.DeviceInit = 	FBDevicesInit,
	.ShowPixel	 = 	FBShowPixel,
	.CleanScreen = 	FBCleanScreen,
};


static int FBDevicesInit(void)
{
	g_iFBfd = open(FB_DEVICE_NAME, O_RDWR);
	if (g_iFBfd < 0)
	{
		DBG_PRINT("can't open /dev/fb0\n");
		return -1;
	}
	if (ioctl(g_iFBfd, FBIOGET_VSCREENINFO, &g_tVar))
	{
		DBG_PRINT("can't get var\n");
		return -1;
	}

	if (ioctl(g_iFBfd, FBIOGET_FSCREENINFO, &g_tFix))
	{
		DBG_PRINT("can't get fix\n");
		return -1;
	}
	
	g_tFBDispOpr.iXres = g_tVar.xres;
	g_tFBDispOpr.iYres = g_tVar.yres;
	g_tFBDispOpr.iBpp  = g_tVar.bits_per_pixel;

	g_FBScreenSize = g_tVar.xres * g_tVar.yres * g_tVar.bits_per_pixel / 8;
	g_LineWidth  = g_tVar.xres * g_tVar.bits_per_pixel / 8;
	g_iPixelWidth = g_tVar.bits_per_pixel / 8;
	
	g_FBMem = (unsigned char *)mmap(NULL , g_FBScreenSize, PROT_READ | PROT_WRITE, MAP_SHARED, g_iFBfd, 0);
	if (g_FBMem == (unsigned char *)-1)
	{
		DBG_PRINT("can't mmap\n");
		return -1;
	}
	return 0;
}

static int FBShowPixel(int iPenX,int iPenY,unsigned int dwColor)
{
	unsigned char *pucPen_8 = g_FBMem+iPenY*g_LineWidth+iPenX*g_iPixelWidth;
	unsigned short *pwPen_16;	
	unsigned int *pdwPen_32;	

	unsigned int red, green, blue;	

	pwPen_16 = (unsigned short *)pucPen_8;  
	pdwPen_32 = (unsigned int *)pucPen_8;

	switch (g_tVar.bits_per_pixel)
	{
		case 8:
		{
			*pucPen_8 = dwColor;
			break;
		}
		case 16:
		{
			/* 565 */
			red   = (dwColor >> 16) & 0xff;
			green = (dwColor >> 8) & 0xff;
			blue  = (dwColor >> 0) & 0xff;
			dwColor = ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3);
			*pwPen_16 = dwColor;
			break;
		}
		case 32:
		{
			*pdwPen_32 = dwColor;
			break;
		}
		default:
		{
			DBG_PRINT("can't surport %dbpp\n", g_tVar.bits_per_pixel);
			break;
		}
	}
	return 0;
}

static int FBCleanScreen(unsigned int dwBackColor)
{
	unsigned char *pucPen_8 = g_FBMem;
	unsigned short *pwPen_16;	
	unsigned int *pdwPen_32;	

	unsigned int red, green, blue;	
	int i ;
	pwPen_16 = (unsigned short *)pucPen_8;  
	pdwPen_32 = (unsigned int *)pucPen_8;

	switch (g_tFBDispOpr.iBpp)
	{
		case 8:
		{
			memset(g_FBMem, dwBackColor, g_FBScreenSize);
			break;
		}
		case 16:
		{
			/* 565 */
			red   = (dwBackColor >> 16) & 0xff;
			green = (dwBackColor >> 8) & 0xff;
			blue  = (dwBackColor >> 0) & 0xff;
			dwBackColor = ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3);

			for(i = 0;i < g_FBScreenSize;i++)
			{
				*pwPen_16 = dwBackColor;
				pwPen_16++;
				i += 2;
			}
			break;
		}
		case 32:
		{
			for(i = 0;i < g_FBScreenSize;i++)
			{
				*pdwPen_32 = dwBackColor;
				pdwPen_32++;
				i += 4;
			}
			break;
		}
		default:
		{
			DBG_PRINT("can't surport %dbpp\n", g_tVar.bits_per_pixel);
			break;
		}
	}
	return 0;
}


int FBInit (void)
{
	return RegisterDispOpr(&g_tFBDispOpr);
}





