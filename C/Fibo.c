/*************************************************************************
	> File Name: Fibo.c
	> Author: 
	> Mail: 
	> Created Time: 2017年11月05日 星期日 11时16分53秒
 ************************************************************************/

#include<stdio.h>

int main()
{
	int i; 
        int a[10];
        a[0]=1;
        a[1]=1;
        for(i=2;i<10;i++)
        {
                a[i]=a[i-1]+a[i-2];
        }
        for (i=0;i<10;i++)
        {
                printf("%d ",a[i]);
        }
        return 0;
}
