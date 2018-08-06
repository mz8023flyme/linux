#include <stdio.h>
#include <stdlib.h>


void main ()
{
 	 int  num; 
     unsigned char i,j;
	 scanf("%d",&num);
     
     i=num/2+1;
     for(j=0;j<i;j++)
     {
     	printf("%d %d\n",j,num-j);
     	
	 }
     
	
}



 
