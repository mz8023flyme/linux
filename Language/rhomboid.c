#include <stdio.h>

int main ()
{	
	char i,j,k,space=0;
	for (i=0;i<4;i++)
	{
		j=space;
		for(;j>0;j--)
		{
			printf(" ");
		}
		for(k=0;k<4;k++)
		{
			printf("*");
		}
		printf("\n");
		space++;
	}
}
