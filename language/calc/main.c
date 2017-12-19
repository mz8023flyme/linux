/*************************************************************************
	> File Name: main.c
	> Author: 
	> Mail: 
	> Created Time: 2017年10月10日 星期二 21时14分13秒
 ************************************************************************/

#include <stdio.h>
#include "cmd.h"
int main()
{
        int a,b ,d;
        char c;
        c=getchar();
        scanf("%d",&a);
        scanf("%d",&b); 
        switch(c)
        {
                case('+'):
                       d= add(a,b);
                break;
                case('-'):
                       d= sub(a,b);
                break;
                case('*'):
                        d=mul(a,b);
                break;
                case('/'):
                       d= dive(a,b);
                break;

        }
        printf("%d\n",d);
        getchar();
        return 0;

}
