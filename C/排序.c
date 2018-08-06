#include <stdio.h>
#include <stdlib.h>

int a[7]={6,4,1,7,9,5,8};
void sort (int *a ,int low ,int high);
int find(int *a,int low ,int high);

int main()
{
	int i;
	sort(a,0,6);
	for(i=0;i<7;i++)
	{
		printf("%d ",a[i]);
	}
	
	return 0;
}

int find(int *a,int low ,int high)
{
	int val=a[low];
	while(low<high)
	{
		while(low<high && a[high]>=val)
		high--;
		a[low]=a[high];
		
		while(low<high && a[low]<=val)
		low++;
		
		a[high]=a[low];
	}
	a[low]=val;
	return high;
}

void sort (int *a ,int low ,int high)
{
	int pos;
	if(low<high)
	{
		pos=find(a,low,high);
		sort(a,low,pos-1);
		sort(a,pos+1,high);
	}
	
}
