/*************************************************************************
	> File Name: 二维数组.c
	> Author: 
	> Mail: 
	> Created Time: 2017年11月05日 星期日 16时00分35秒
 ************************************************************************/

#include<stdio.h>

void main ()
{
        int a[3][5];
        for(int i=0;i<15;i++)
        {
                a[i/5][i%5]=i;
        }
        for(int i=0;i<3;i++)
        {
                for(int j=0;j<5;j++)
                {
                        printf("%d ",a[i][j]);
                }
                printf("\n");
        }
}


void main1 ()
{
        int a[3][5];
        for(int i=0;i<3;i++)
        {
                for (int j=0;j<5;j++)
                {
                        a[i][j]=i*5+j;
                        printf("%-2d ",a[i][j]);
                }
                printf("\n");
        }
}
