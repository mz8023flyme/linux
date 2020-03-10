#include <stdio.h>
#include <string.h>
#include <math.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <ft2build.h>
#include <sys/ioctl.h>

#include FT_FREETYPE_H

int fd_fb;
struct fb_var_screeninfo var;	/* Current var */
struct fb_fix_screeninfo fix;	/* Current fix */
int screen_size;
unsigned char *fbmem;
unsigned int line_width;
unsigned int pixel_width;

//在LCD上显示
/* color : 0x00RRGGBB */
void lcd_put_pixel(int x, int y, unsigned int color)
{
	unsigned char *pen_8 = fbmem+y*line_width+x*pixel_width;
	
	//找到对应的像素在内存中的位置，后面向其(*pen_8)写颜色值。
	//通过pen_8来操作fbmem,实现LCD显示
	
	unsigned short *pen_16;	
	unsigned int *pen_32;	
	unsigned int red, green, blue;	

	pen_16 = (unsigned short *)pen_8;
	pen_32 = (unsigned int *)pen_8;

	switch (var.bits_per_pixel)//不同的像素位数，颜色值格式不同。根据位数设置颜色值格式，并写值。
	{//像素的各位就代表着颜色分量值。
		case 8:
		{
			*pen_8 = color;//8位像素的话，颜色值直接赋给这个内存中即可。
			break;
		}
		case 16:
		{
			/* color : 0x00RRGGBB ，unsigned int 32位color的格式*/
			/* 565 */
			red   = (color >> 16) & 0xff;//右移16位后剩下高16位，再&0xff，又剩下低8位。即取出32位color的16-23位
			green = (color >> 8) & 0xff;//取出32color的8-15位
			blue  = (color >> 0) & 0xff;//取出32color的0-7位
			color = ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3);//取出对应的组成565
			*pen_16 = color;
			break;
		}
		case 32:
		{
			*pen_32 = color;//32位像素也直接写即可。
			break;
		}
		default:
		{
			printf("can't surport %dbpp\n", var.bits_per_pixel);
			break;
		}
	}
}

//freetype声明的一个函数指针draw_bitmap,需要我们自己实现这个函数，根据bitmap把字形描绘出来
//draw_bitmap()函数：把字体在屏上显示，具体屏具体实现：

void draw_bitmap( FT_Bitmap*  bitmap,     //draw_bimap()是把一个字符(一个字符对应的方框所有像素所对应的颜色值,
				     //颜色值是8位，16位等，即unsigned char,char等)写到LCD的framebuffer。
             FT_Int      x,
             FT_Int      y)
{
  	FT_Int  i, j, p, q;
  	FT_Int  x_max = x + bitmap->width;
  	FT_Int  y_max = y + bitmap->rows;

  	for ( i = x, p = 0; i < x_max; i++, p++ )
  	{
    		for ( j = y, q = 0; j < y_max; j++, q++ )
    		{
      			if ( i<0 || j<0 || i>=var.xres || j>=var.yres )
        			continue;

      			lcd_put_pixel(i, j, bitmap->buffer[q * bitmap->width + p]);
				
      			//显示顺序是一列一列的显示，把i=x这一列，不同的y的这一列先显示。
      			//即顺序：(i,j,q*bitmap->width +p);  (i,j+1, (q+1)*bitmap->width +p ) //(x,y,color)
      			//bitmap->buffer[]是颜色值，lcd_put_pixel()是操作lcd的应用层缓冲区framebuffer.
 	  		//来直接显示颜色。
 	  	}
	}
}

int lcd_init(void)
{
	fd_fb = open("/dev/fb0", O_RDWR);
	if (fd_fb < 0)
	{
		printf("can't open /dev/fb0\n");
		return -1;
	}

	if (ioctl(fd_fb, FBIOGET_VSCREENINFO, &var))
	{
		printf("can't get var\n");
		return -1;
	}

	if (ioctl(fd_fb, FBIOGET_FSCREENINFO, &fix))
	{
		printf("can't get fix\n");
		return -1;
	}

	line_width  = var.xres * var.bits_per_pixel / 8;
	pixel_width = var.bits_per_pixel / 8;
	screen_size = var.xres * var.yres * var.bits_per_pixel / 8;
	printf("line_width%d pixel_width%d  screen_size%d\n",line_width,pixel_width,screen_size);
	fbmem = (unsigned char *)mmap(NULL , screen_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_fb, 0);
	if (fbmem == (unsigned char *)-1)
	{
		printf("can't mmap\n");
		return -1;
	}
}

int main( int argc,char** argv )
{
	FT_Library    library;
 	FT_Face       face;
 	FT_GlyphSlot  slot;
	FT_Matrix     matrix;                 /* transformation matrix */
  	FT_Vector     pen;                    /* untransformed origin  */
  	FT_Error      error;

  	char*         filename;
  	char*         text;
  	double        angle;
  	int           target_height;
  	int           n, num_chars;

  	if ( argc != 3 )
  	{
    		fprintf ( stderr, "usage: %s font sample-text\n", argv[0] );
    		exit( 1 );
  	}

 	lcd_init();

  	/* 清屏: 全部设为黑色 */
	memset(fbmem, 0, screen_size);

  	filename      = argv[1];                          /* first argument     */
  	text          = argv[2];                          /* second argument    */
  	num_chars     = strlen( text );
  	angle         = ( 0.0 / 360 ) * 3.14159 * 2;      /* use 25 degrees     */
  	//旋转的角度
 	//这个相当于像素数
  	target_height = var.yres;
  	error = FT_Init_FreeType( &library );              /* initialize library */
  	error = FT_New_Face( library, argv[1], 0, &face ); /* create face object */
	
	FT_Set_Pixel_Sizes(face, 24, 0);

  	slot = face->glyph;	//先使slot指向face->glyph,下面也是通过glyph来设置slot

  	/* set up matrix */
  	matrix.xx = (FT_Fixed)( cos( angle ) * 0x10000L );
  	matrix.xy = (FT_Fixed)(-sin( angle ) * 0x10000L );
  	matrix.yx = (FT_Fixed)( sin( angle ) * 0x10000L );
  	matrix.yy = (FT_Fixed)( cos( angle ) * 0x10000L );

  	/* the pen position in 26.6 cartesian space coordinates; */
  	/* start at (0,40) relative to the upper left corner  */
  	pen.x = 0 * 64;
  	pen.y = ( target_height - 40 ) * 64;  //(0,40是LCD坐标，转为笛卡尔)，target_height是以dip为单位的分辨率。

  	//这个pen.x = 0 * 64是char_width值，是FT_Set_Char_Size()函数参数中的50*64一样，
  	//0,50都是LCD坐标，总共80*80个坐标(分辨率)。
  	//char_width与坐标值的关系：char_width=坐标值*64，char_width/64=n point,坐标值就代表第 n 
  	//个point,第n个分辨率单元。1分辨率就是1point

  	for ( n = 0; n < num_chars; n++ )	
  	{
   		 /* set transformation */

    		FT_Set_Transform( face, &matrix, &pen );//这里使用到了pen

    		/* load glyph image into the slot (erase previous one) */

    		error = FT_Load_Char( face, text[n], FT_LOAD_RENDER );//就会设置face->glyph,转换的位图又保存在哪里?
    		if ( error )					//位图点阵应该是保存在face->glyph.bitmap中。 
      			continue;                 /* ignore errors */

    		/* now, draw to our target surface (convert position) */

    		draw_bitmap( &slot->bitmap,
                	 slot->bitmap_left,
                 	 target_height - slot->bitmap_top );
    		/* increment pen position */
    		pen.x += slot->advance.x;
    		pen.y += slot->advance.y;
  	}
    FT_Done_Face    ( face );
  	FT_Done_FreeType( library );

  	return 0;
}
