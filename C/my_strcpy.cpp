#include <stdio.h>



char * my_strcpy(char * source ,char * dest)
{
	char *p1;
	char *p2;
	p1=source;
	p2=dest;
	while(*source!='\0')
	{
	    *p2=*p1;
	    p1++;
	    p2++;
	}
} 

int main ()
{
	char str1[]="hello world";
	char str2[100] = {};
	my_strcpy(str1,str2);
	printf("%s\n",str2);
	printf("%d",sizeof(unsigned char));
 } 
 
