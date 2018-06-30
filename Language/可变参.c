/*************************************************************************
	> File Name: 可变参.c
	> Author: 
	> Mail: 
	> Created Time: 2017年11月04日 星期六 22时08分41秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>
#include<string.h>
void add (double num,...);
void sys(int num,...);
int main ()
{
        add(3,6.0,3.0,9.0) ;
        add(4,1.0,6.2,8.5,5.6);
        sys(2,"smplayer","calc");
}

void sys(int num,...)
{
        int i=0;
        va_list argp;
        va_start(argp,num);
        for(i;i<num;i++)
        {
                char str[50];
                strcpy(str,va_arg(argp,char *)) ;
                printf("%s\n",str);
                system("str");
        }
        va_end(argp);
}

void add (double num,...)
{
        int i=0;
        double sum=0;
        va_list argp;//创建一个char型的指针
        va_start(argp,num);//读取有多少个指针
        for(i=0;i<num;i++)
        {
             //   printf("%f ",va_arg(argp,double));
                sum+=va_arg(argp,double);
        }
        va_end(argp);
        printf("sum=%0.2f ",sum);
        printf("jun=%0.2f ",sum/num);
        printf("\n");
}
