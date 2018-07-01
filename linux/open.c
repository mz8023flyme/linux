/*************************************************************************
    > File Name: open.c
  > Author: life
  > Created Time: 2018年06月30日 星期六 21时46分21秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>


int main (int argc ,char * argv[])
{

    int fd;
    if (argc<2)
    {
        printf("./app filename");
        exit(1);
    }
    

    fd = open(argv[1],O_RDWR|O_CREAT|O_APPEND,0644);
    if (fd<0)
    {   
        printf("error\n");
        printf("fd = %d\n",fd);
    }
    else
    {
        printf("fd = %d\n",fd);
        write(fd,argv[2],strlen(argv[2]));
    }
    close(fd);
}
