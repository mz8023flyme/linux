#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


void print_usage(char * file)
{
    printf("%s <dev> \n",file);
    
}
int main (int argc ,int **argv)
{
    int fd ;
    if(argc !=2)
    {
        print_usage(argv[0]);
        return 0 ;
    }

    fd= open( argv[1],O_RDWR);

    if(fd<0)
    {
        printf("do not open \n",argv[1]);
    }
    else
    {
        printf(" open \n",argv[1]);

    }


    return 0 ;
}
































