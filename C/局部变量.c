#include <stdio.h>
#include <stdlib.h>
int add (int n)
{
	static i=0;
	return (i+n);	
}


void mian ()
{
     printf ("第一次 %d\n",add(1));
     printf ("第二次 %d\n",add(1));
     printf ("第三次 %d\n",add(1));
     getchar();
}



 
