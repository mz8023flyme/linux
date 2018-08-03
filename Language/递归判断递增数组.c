#include <stdio.h>
#include <stdlib.h>

void increase(int a[],int n)
{
	if (n==1)
	{
		printf("ÊÇ\n");
		return; 
	}
	else
	{
		if(a[n-1]<a[n-2])
		{
			printf("²»ÊÇ\n");
			return;
		}
		else
		{
			increase(a,n-1);
		}
	}
	
} 


void main()
{
	int a[4]={0,6,8,9};
	
	increase(a,4); 
}
/*
char fun( int a[], int n )
{
if( n==1 )
return 1;
if( n==2 )
return a[n-1] >= a[n-2];
return fun( a,n-1) && ( a[n-1] >= a[n-2] );
}
void main()
{
    int a[4]={0,1,2,3};
	printf("%d",fun(a[4],4));
}

*/
