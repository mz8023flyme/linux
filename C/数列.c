#include <stdio.h>

int main ()
{
	int i;
	int a=1;
	int b=1;
	int c=0;
	printf("%d\n%d\n",a,b);
	for(i=0;i<20;i++)
	{
		c=a+b;
		printf("%d\n",c);
		a=b;
		b=c;
	}
	
} 


