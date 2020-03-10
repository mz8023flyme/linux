#include <stdio.h>
#include <string.h>
#include <math.h>
#include <wchar.h>

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>

#include <ft2build.h>
#include FT_FREETYPE_H


/* origin is the upper left corner */
//unsigned char image[HEIGHT][WIDTH];

wchar_t *ch_char = L"中国abc";

int fd_fb;
struct fb_var_screeninfo var;	/* Current var */
struct fb_fix_screeninfo fix;	/* Current fix */
int screen_size;
unsigned char *fbmem;
unsigned int line_width;
unsigned int pixel_width;

void lcd_put_pixel(int x, int y, unsigned int color)
{
	unsigned char *pen_8 = fbmem+y*line_width+x*pixel_width;
	unsigned short *pen_16;	
	unsigned int *pen_32;	

	unsigned int red, green, blue;	

	pen_16 = (unsigned short *)pen_8;  
	pen_32 = (unsigned int *)pen_8;

	switch (var.bits_per_pixel)
	{
		case 8:
		{
			*pen_8 = color;
			break;
		}
		case 16:
		{
			/* 565 */
			red   = (color >> 16) & 0xff;
			green = (color >> 8) & 0xff;
			blue  = (color >> 0) & 0xff;
			color = ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3);
			*pen_16 = color;
			break;
		}
		case 32:
		{
			*pen_32 = color;
			break;
		}
		default:
		{
			printf("can't surport %dbpp\n", var.bits_per_pixel);
			break;
		}
	}
}



/* Replace this function with something useful. */

void draw_bitmap( FT_Bitmap*  bitmap,
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
      if ( i < 0      || j < 0       ||
           i >= var.xres || j >= var.yres )
        continue;

     // image[j][i] |= bitmap->buffer[q * bitmap->width + p];
     lcd_put_pixel(i,j,bitmap->buffer[q * bitmap->width + p]);
    }
  }
}

int main( int argc,char**  argv )
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
	fbmem = (unsigned char *)mmap(NULL , screen_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_fb, 0);
	if (fbmem == (unsigned char *)-1)
	{
		printf("can't mmap\n");
		return -1;
	}
	memset(fbmem, 0, screen_size);
	filename      = argv[1];                           /* first argument     */
	// text          = argv[2];                           /* second argument    */
	num_chars     = sizeof( ch_char )/4;
	angle         = ( 0.0 / 360 ) * 3.14159 * 2;      /* use 0 degrees  旋转     */
	target_height = var.yres;
	
	error = FT_Init_FreeType( &library );              /* initialize library */
	/* error handling omitted */
	
	error = FT_New_Face( library, filename, 0, &face );/* create face object */
	/* error handling omitted */
	
	/* use 50pt at 100dpi */
	//error = FT_Set_Char_Size( face, 40 * 64, 0,
	//                          40, 0 );                /* set character size */
		FT_Set_Pixel_Sizes(face, 64, 0);
	/* error handling omitted */

	slot = face->glyph;
	
	/* set up matrix */
	matrix.xx = (FT_Fixed)( cos( angle ) * 0x10000L );
	matrix.xy = (FT_Fixed)(-sin( angle ) * 0x10000L );
	matrix.yx = (FT_Fixed)( sin( angle ) * 0x10000L );
	matrix.yy = (FT_Fixed)( cos( angle ) * 0x10000L );
	
	/* the pen position in 26.6 cartesian space coordinates; */
	/* start at (40,0) relative to the upper left corner  */
	pen.x = (var.xres/2) * 64;
	pen.y = ( var.yres/2 ) * 64;
	
	for ( n = 0; n < num_chars; n++ )
	{
	/* set transformation */
	FT_Set_Transform( face, &matrix, &pen );
	
	/* load glyph image into the slot (erase previous one) */
	error = FT_Load_Char( face, ch_char[n], FT_LOAD_RENDER );
	if ( error )
		continue;                 /* ignore errors */
	
	/* now, draw to our target surface (convert position) */
	draw_bitmap( &slot->bitmap,
				slot->bitmap_left,
				var.yres/2 - slot->bitmap_top );
	
	/* increment pen position */
	pen.x += slot->advance.x;
	pen.y += slot->advance.y;
	}

	FT_Done_Face    ( face );
	FT_Done_FreeType( library );
	
	return 0;
}

/* EOF */
