#include <stdio.h>
#include <string.h>

int A[]={2,5,7,8,3,4};
int sum=0;
int MAX;
int Average;
int max(int  len )
{
	if(len==1)
	{
		if(A[0]>MAX)
		{
			MAX=A[0];
		}
		return MAX	;
	}
	else
	{
		if(A[len-1]>MAX)
		{
			MAX=A[len-1];
		}
		max(len-1);
	}
}

int  add(int len )
{
	if(len==1)
	{
		sum=sum+A[0];
		return sum;
	}
	else
	{
		sum=sum+A[len-1];
		add(len-1);
	}
	
}


float average(int len)
{
	return add(len)/len;
}



int main ()
{
//	add(6);
//	printf("%d",sum);

//	max(6);
//	printf("%d",MAX);

	printf("%f",average(6));
	return 0 ;
} 
