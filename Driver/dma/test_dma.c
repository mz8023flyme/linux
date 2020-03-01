#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>

#define MEM_CP_NO_DMA  0
#define MEM_CP_DMA     1

void print_usage(char * file)
{
    printf("%s <nodma | dma> \n",file);
}

int main (int argc,char **argv )
{
    int fd;
    if(argc!=2)
    {
        print_usage(argv[0]);
        return -1;    
    }
    fd = open("/dev/dma",O_RDWR);
    if(fd<0)
    {
        printf("not open dma\n");
        return -1 ;
    }
    if(strcmp(argv[1],"nodma")==0)
    {
        while(1)
        {
            ioctl(fd,MEM_CP_NO_DMA);
        }
    }
    else if (strcmp(argv[1],"dma")==0)
    {   
        while(1)
        {
            ioctl(fd,MEM_CP_DMA);
        }
    }
    else
    {
        print_usage(argv[0]);
        return 0 ;
    }
}































