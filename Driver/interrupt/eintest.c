#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

/*
  *  key <dev> <on|off>
  */

int main(int argc, char **argv)
{
    int fd;
    unsigned char key_value;
	
    fd = open("/dev/eint", O_RDONLY);//eint
    if (fd < 0)
    {
        printf("error, can't open \n");
        return 0;
    }
	
	while(1)
	{
		read(fd,&key_value,1);
		printf("key_value=0x%x\n",key_value);
 	}
   
    return 0;
}








































































