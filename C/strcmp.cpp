#include <stdio.h>
#include <string.h>

char s1[]={'h','e','l','l','o'};
char s2[]={'h','e','l','l','o','\0'};
char s3[]="hello";
int main()
{
	if(!strcmp(s1,s2))
		printf("s1==s2");
	else
		printf("s1!=s2");
	
}
