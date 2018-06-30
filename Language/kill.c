#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
int main()
{
	//	system("\"D:\\软件安装\\酷狗\\KGMusic\\KuGou.exe\"");
	

	system("taskkill /f /im KuGou.exe");  //  /f强制  /im  进程名
	MessageBoxA(0,"酷狗出现了故障","友情提示",1);
}



