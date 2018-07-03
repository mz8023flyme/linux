/*************************************************************************
    > File Name: ioctl.c
  > Author: life
  > Created Time: 2018年07月03日 星期二 15时57分24秒
 ************************************************************************/

#include<stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>    
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <linux/fb.h>

int main (void )
{
    int fd ;
    
    struct fb_var_screeninfo fb_info;

    fd = open ("/dev/fb0",O_RDONLY);
    if (fd < 0 )
    {
        perror("open /dev/fb0");
        exit(1);
    }
    
    ioctl(fd,FBIOGET_VSCREENINFO,&fb_info);

    printf("winth=%d\nC=%d\n",fb_info.xres,fb_info.yres);

    close(fd);
    return 0;


}
