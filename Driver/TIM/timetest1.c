#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

/*
  *  signal
  */
int fd;

void my_signal_fun(int signum)
{
	unsigned char key_val;
	read (fd,&key_val,1);
	printf("key_val:0x%x\n",key_val);
}




int main(int argc, char **argv)
{
    int Oflags;
    signal(SIGIO,my_signal_fun);
	
    fd = open("/dev/time", O_RDONLY);//eint
    if (fd < 0)
    {
        printf("error, can't open \n");
        return 0;
    }

	fcntl(fd, F_SETOWN, getpid());//告诉内核，发给谁
	
	Oflags = fcntl(fd, F_GETFL); 	      //改变fasync标志，最终会调用驱动的
										  //signal_fasync->fasync_helper
    fcntl(fd, F_SETFL, Oflags | FASYNC);  //初始化结构体
										 
										 
	while(1)
	{
		sleep(1000 );
 	}
   
    return 0;
}








































































