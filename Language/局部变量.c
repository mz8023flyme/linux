#include <stdio.h>
#include <stdlib.h>
int add (int n)
{
	static i=0;
	return (i+n);	
}


void mian ()
{
     printf ("��һ�� %d\n",add(1));
     printf ("�ڶ��� %d\n",add(1));
     printf ("������ %d\n",add(1));
     getchar();
}



 
