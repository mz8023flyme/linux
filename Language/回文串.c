#include <stdio.h>
#include <string.h>

void main()
{
	char str[50];
	int len,i=0,j=0;
	gets_s(str);
	len = strlen(str);
	printf( "�ַ����ĳ���Ϊ %d\n",len );

	for (i = 0; i < len;i++)
	{
		if (str[i] == str[len - i-1])
		{
			j += 1;
		}
	}
	if (j == len)
	{
		printf("�ǻ��Ĵ�");

	}
	else
	{
		printf("���ǻ��Ĵ�");
	}
	getchar();

}





