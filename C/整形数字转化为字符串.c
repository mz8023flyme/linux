#include <stdio.h>
#include <stdlib.h>

void main ()
{
	int i,digit,data,data1,j=1;
	char a[10],b[10];
	printf("请输入要转化的数字");
	 
	scanf("%d",&data);
	printf("data=%d\n",data); 
	while(data)
	{
		a[i++]=data%10+'0';
		data=data/10;
		//printf("%c",a[i-1]);
	}
	for (j=0;j<i;j++)
	{
		b[j]=a[i-j-1];
	}
	b[j]='\0';
	printf("%s",b);
	
	/*
	data1=data; 
	do
	{
		digit++;
		data1/=10;
	}while(data1);
	printf("\n%d\n",digit-1);
	data1=data; 
	for(i=0;i<digit-2;i++)
	{
	     j*=10;
	}
	for(i=0;i<digit-1;i++)
	{
		a[i]=(data1/j)+'0';
		printf("%c",a[i]);
		data1%=j;
		j/=10;
	}
	*/
	//system("pause");
	
}





