#include<iostream>
#include <string.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
using namespace std; 
int sum(int a)
{
	auto int c=0;
	static int b=3;
	c+=1;
	b+=2;
	return(a+b+c);
}
void main()
{
	int I;
	int a=2;
	for(I=0;I<5;I++)
	{
	     printf("%d,", sum(a));
	} 
}
