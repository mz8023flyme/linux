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
    unsigned char key_value[3];
	
    fd = open("/dev/button", O_RDONLY);//keys_drv
    if (fd < 0)
    {
        printf("error, can't open \n");
        return 0;
    }
	
	while(1)
	{
		read(fd,key_value,sizeof(key_value));
		if(!key_value[0])
		{
			printf("key1 down\n");
		}
		if(!key_value[1])
		{
			printf("key2 down\n");
		}
		if(!key_value[2])
		{
			printf("key3 down\n");
		}
 	}
   
    return 0;
}































