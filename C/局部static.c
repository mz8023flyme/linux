#include<stdio.h>


int add (int n)
{
	static i=0;
	i+=n;
	return i;	
}


void main ()
{
     printf ("��һ��Ϊ%d\n",add(1));
     printf ("�ڶ���Ϊ%d\n",add(1)); 
     printf ("������Ϊ%d\n",add(1));
}



 
