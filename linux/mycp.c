/*************************************************************************
    > File Name: mycp.c
  > Author: life
  > Created Time: 2018年07月02日 星期一 17时35分51秒
 ************************************************************************/

#include<stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>





int main (int argc ,char *argv[])
{
    int fd1,fd2;
    char buf[8192];
    int len =0;
    if(argc<3)
    {
        printf("./app sec  dev");
    }
    else
    {
         fd1=open(argv[1],O_RDONLY);
         if(fd1<0)
         {
            printf("open error");
            exit(1);
         }
         fd2=open(argv[2],O_CREAT|O_WRONLY,0644);
         if(fd2<0)
         {
            printf("open error");
            exit(1);
         }
         len=read(fd1,buf,sizeof(buf));
         write(fd2,buf,len);
    }


    return 0;
}
