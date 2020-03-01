#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <poll.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

static unsigned char num[3]={1,1,1};

static int  openled (unsigned char val)
{
	int fd;
	char *filename;
	unsigned char POS;
	
	if(val!=0)
	{
		switch (val)
		{
			case (0x01):
				POS=0;
				num[POS]=num[POS]^1;
				printf("num[%d]=%d\n",POS,num[POS]);
				filename="/dev/led1";
			break;
			case (0x02):
				POS=1;
				num[POS]=num[POS]^1;
				printf("num[%d]=%d\n",POS,num[POS]);
				filename="/dev/led2";
			break;
			case (0x03):
				POS=2;
				num[POS]=num[POS]^1;
				printf("num[%d]=%d\n",POS,num[POS]);
				filename="/dev/led3";
			break;
			default :
			break;
		}
		fd = open(filename, O_RDWR);
	    if (fd < 0)
	    {
	        printf("error, can't open %s\n", filename);
	        return -1;
	    }
		write(fd, &num[POS], 1);
		
	
	}
}

int main(int argc, char **argv)
{
	unsigned char keyval;
	int ret;
	int fd1;
	fd1 = open("/dev/block", O_RDWR );
	if (fd1 < 0)
	{
		printf("can't open the block!\n");
		return -1;
	}
	while (1)
	{
		ret = read(fd1, &keyval, 1);
		printf("keyvalue: 0x%x, ret = %d\n", keyval, ret);

		openled(keyval);
		
		//sleep(1);
	}
	
	return 0;
}
 

