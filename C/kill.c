#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
int main()
{
	//	system("\"D:\\�����װ\\�ṷ\\KGMusic\\KuGou.exe\"");
	

	system("taskkill /f /im KuGou.exe");  //  /fǿ��  /im  ������
	MessageBoxA(0,"�ṷ�����˹���","������ʾ",1);
}



