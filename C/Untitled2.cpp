#include <stdio.h> 
#include <string.h>
void strq(char * str,int len)
{
	int i;
	char *p;
	char *q;
	char c=NULL;
	p=str;
	q=p+len-1;
	for (i=0;i<len/2;i++)
	{
		c=*p;
		*p=*q;
		*q=c;
		p++;
		q--;
	} 
	
}
int srtp(char *str ,char ch,int n)
{
	char *p;
	int num=0;
	p=str;
	p=p+n;
	while(*p!='\0')
	{
		if(*p == ch)
		{
			break;
		}
		p++;
		num++;
	}
	return num;
}

float A[10]={0.14,1.25,2.2,21.3,5.02,0.36,1.56,2.0,3.6,1.2};

void fun3(float * a,int len )
{
	int i ,j;
	float c=0.0;
	//printf("%d\n",len);
	float *p=a;
	for (i=0;i<len;i++)
	{
		for(j=0;j<len;j++)
		{
			if(*p<*(p+1))
			{
				c=*p;
				*p=*(p+1);
				*(p+1)=c;
			}
			p++;
		}
		p=a;
	}
		
	for(i=0;i<len;i++)
	{
		printf("%0.2f  ",*(a+i));
	}
	
}



int main ()
{
	char str1[]="hello world I am coming";
    //strq(str1,strlen(str1));
	//printf("%s",str1);
    //printf("%d",srtp(str1,'l',0));
    int i;
    char *p=str1;
    while(*p!='\0')
    {
    	if(*p==' ')
    	{
    		i++;
		}
		p++;
	}
    i++;
//	fun3(A,sizeof(A)/sizeof(float));
	printf("%d",i);
	return 0;	
}



