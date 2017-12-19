/*************************************************************************
	> File Name: 随机数.c
	> Author: 
	> Mail: 
	> Created Time: 2017年11月05日 星期日 11时05分51秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int main ()
{
        int sum=0;
        float total=0.0;
        time_t ts; //设置时间变量"
        srand ((unsigned int )time(&ts));
        int a[10];
        for(int i=0;i<10;i++)
        {
                a[i]=rand()%100;   //取随机数再１００范围内
                printf("%d  ",a[i]);
                sum+=a[i];
        }
        printf("\n");
        total=sum/10;
        printf("sum=%d  tatal=%f",sum,total);
        printf("\n");
}

