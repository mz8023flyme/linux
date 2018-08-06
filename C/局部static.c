#include<stdio.h>


int add (int n)
{
	static i=0;
	i+=n;
	return i;	
}


void main ()
{
     printf ("第一次为%d\n",add(1));
     printf ("第二次为%d\n",add(1)); 
     printf ("第三次为%d\n",add(1));
}



 
