#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <linux/input.h>

int key_fd;
int led_fd;
#define  key_input  "/dev/event1"
#define  led1        "/dev/led1"
#define  led2        "/dev/led2"
#define  led3        "/dev/led3"
#define  leds        "/dev/leds"

int main (int argc,char ** argv )
{
    char i ;
    struct input_event  dev_key;
    key_fd =  open (key_input,O_RDONLY);
    unsigned char num[3]={1,1,1};
    unsigned char val;
    char * filename=NULL;

    if(key_fd<=0)
    {
        printf("can't open key");    
    }
     while(1)
     {
        if(read(key_fd,&dev_key,sizeof(dev_key))== sizeof(dev_key))
        {
            if(dev_key.type == EV_KEY)
            {
                printf("type: 0x%x code: 0x%x value: 0x%x\n", 
                dev_key.type,
                dev_key.code,  
                dev_key.value);
                switch(dev_key.code)
                {
                     case KEY_L :
                        /*
                        val=0;
                        num[val] = num[val] ^ 1;
                        filename = led1;
                        printf("[%s][%d]",__FUNCTION__,__LINE__);
                        */
                         
                         led_fd = open(led1,O_RDWR);
                        if(led_fd<=0)
                        {
                            printf("can't open led");
                            return -1;
                        }
                        val=1;
                        write(led_fd &val, 1);
                    break;
                     case KEY_S :
                        /*
                         val=1;
                         num[val] = num[val] ^ 1;
                         filename = led2;
                         printf("[%s][%d]",__FUNCTION__,__LINE__);
                         */
                          
                         led_fd = open(led1,O_RDWR);
                        if(led_fd<=0)
                        {
                            printf("can't open led");
                            return -1;
                        }
                        val=0;
                        write(led_fd &val, 1);
                    break;
                     case KEY_ENTER :
                        /*
                         val=2;
                         num[val] = num[val] ^ 1;
                         filename = led3;
                         printf("[%s][%d]",__FUNCTION__,__LINE__);
                        */
                         
                         led_fd = open(leds,O_RDWR);
                        if(led_fd<=0)
                        {
                            printf("can't open led");
                            return -1;
                        }
                        val=1;
                        write(led_fd &val, 1);
                    break;
                     case KEY_BACK :
                        /*
                        printf("[%s][%d]",__FUNCTION__,__LINE__);
                        led_fd = open(leds,O_RDWR);
                        if(led_fd<=0)
                        {
                            printf("can't open leds");
                            return -1;
                        }
                        val=1;
                        write(led_fd  ,&val, 1);
                        */
                          
                         led_fd = open(leds,O_RDWR);
                        if(led_fd<=0)
                        {
                            printf("can't open led");
                            return -1;
                        }
                        val=0;
                        write(led_fd &val, 1);
                    break;
                    default :
                    break;
               }
                
                
                
                
            }
        }
     }
    
        
    return 0;
}



















