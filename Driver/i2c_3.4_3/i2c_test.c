#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "i2c-dev.h"
/*
 * i2c_test </dev/i2c-0> <dev-addr> r addr
 * i2c_test </dev/i2c-0> <dev-addr> w addr val
 */

void print_usage(char * file)
{
    printf("%s r addr \n",file);
    printf("%s w addr data \n",file);
}

int main (int argc ,char **argv)
{
    int fd;
    unsigned char addr ,data;
    int dev_addr;
    if( (argc!=5)&&(argc !=6) )
    {   
        print_usage(argv[0]);
        return -1;
    }

    fd = open(argv[1],O_RDWR); 
    if(fd<0)
    {
        printf("not open /dev/i2c-0\n");
        return -1;
    }

    dev_addr = strtoul(argv[2],NULL,0);
    if(ioctl(fd,I2C_SLAVE,dev_addr)<0)
    {   
        printf("set addr error");
        return -1;     
   }
    
    if(strcmp(argv[3],"r")==0)
    {
        addr = strtoul(argv[4],NULL,0);
        data=i2c_smbus_read_word_data(fd,addr);
        printf("data:%c,%d 0x%2x\n",data,data,data);
    }
    else if((strcmp(argv[3],"w")==0)&&(argc==6))
    {
        addr = strtoul(argv[4],NULL,0);
        data = strtoul(argv[5],NULL,0);
        i2c_smbus_write_byte_data(fd, addr, data);
    }
    else
    {
        print_usage(argv[0]);
        return -1;

    }
    return 0 ;
}
























