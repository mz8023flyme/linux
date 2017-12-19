#include <stdio.h>
#include <string.h>

void main()
{
	char str[50];
	int len,i=0,j=0;
	gets_s(str);
	len = strlen(str);
	printf( "字符串的长度为 %d\n",len );

	for (i = 0; i < len;i++)
	{
		if (str[i] == str[len - i-1])
		{
			j += 1;
		}
	}
	if (j == len)
	{
		printf("是回文串");

	}
	else
	{
		printf("不是回文串");
	}
	getchar();

}





