#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>



#define TSL2561_DATA0_LOW     0x8C  
#define TSL2561_DATA0_HIGH    0x8D  
#define TSL2561_DATA1_LOW     0x8E  
#define TSL2561_DATA1_HIGH    0x8F 

#define TSL2561_CMD           0x80 
#define TSL2561_ON            0x03
#define TSL2561_TIMING        0x81 
#define INTERRUPT             0xC6 

int fd;

int main(int argc ,char** argv)
{
    unsigned char buf[4];
    unsigned char addr[4]={TSL2561_DATA0_LOW, TSL2561_DATA0_HIGH,TSL2561_DATA1_LOW,TSL2561_DATA1_HIGH };
    unsigned int  DataHigh0=0, DataHigh1=0,ch0=0,ch1=0;
    unsigned char DataLow0=0,DataLow1=0,lxp=0;
    float lx=0;

    unsigned char i=0 ;
     
    fd=open("/dev/tsl2561",O_RDWR);  ///dev/at24c02
    if(fd<0)
    {
        printf("can't open /dev/tsl2561");
        return -1;
    }
    buf[0]=TSL2561_CMD;
    buf[1]=TSL2561_ON;
    if(write(fd,buf,2)!=2)
    {
        printf("[%s][%d]",__FUNCTION__,__LINE__);
    }
    buf[0]=TSL2561_TIMING;
    buf[1]=0x02;
    if(write(fd,buf,2)!=2)
    {
        printf("[%s][%d]",__FUNCTION__,__LINE__);
    }
    buf[0]=INTERRUPT;
    buf[1]=0x30;
    if(write(fd,buf,2)!=2)
    {
        printf("[%s][%d]",__FUNCTION__,__LINE__);
    }

    
    while(1)
    {   
   
        for(i=0;i<4;i++)
        {
            buf[i]=addr[i];
            read(fd,&buf[i],1);
            printf("%d\n",buf[i]);
        }
        DataLow0 =buf[0];
        DataHigh0=buf[1];
        ch0=(DataHigh0*256+DataLow0);

        DataLow1 =buf[2];
        DataHigh1=buf[3];
        ch1=(DataHigh1*256+DataLow1);
        lxp=(ch1*100/ch0);
        if((lxp>0)&&(lxp<=50))
        {
                lx=0.0304*ch0-0.062*ch0*((ch1/ch0)/4);
        }
        else if((50<lxp)&&(lxp<=61)) 
        {  
                lx=0.0224*ch0-0.031*ch1;  
        }   
        else if((61<lxp)&&(lxp<=80))
        {   
                lx=0.0128*ch0-0.0153*ch1; 

        }   
        else if((80<lxp)&&(lxp<=130)) 
        {   
                lx=0.00146*ch0-0.00112*ch1; 
        } 
        else if(130<lxp)
        {  
                lx=0;  
        } 
        lx=lx*4*3.14;

        printf("lx=%0.2f",lx);
        
        sleep(10);
        printf("\n\n\n");
    }
    close(fd);
    
    return 0 ;
}









