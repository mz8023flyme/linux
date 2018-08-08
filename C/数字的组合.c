#include <stdio.h>
#include <stdlib.h>
/*
combination(int num,int n)
{
	if(n==0)
	{
		return ;	
    } 
    combination(num-1,n-1);
    
    
    
    
}

void main ()
{
	
	combination(5,3);
	
	system("pause");
}

*/


int pop(int *);
int push(int );
void combination(int ,int );
int stack[3]={0};
top=-1;
int main()
{
int n,m;
printf("Input two numbers:\n");
while( (2!=scanf("%d%*c%d",&n,&m)) )
{
fflush(stdin);
printf("Input error! Again:\n");
}
combination(n,m);
printf("\n");
}
void combination(int m,int n)
{
int temp=m;
push(temp);
while(1)
{
if(1==temp)
{
if(pop(&temp)&&stack[0]==n) //当栈底元素弹出&&为可能取的最小值，循环退出
break;
}
else if( push(--temp))
{
printf("%d%d%d ",stack[0],stack[1],stack[2]);//§&auml;¨ì¤@?
pop(&temp);
} } }
int push(int i)
{
stack[++top]=i;
if(top<2)
return 0;
else
return 1;}
int pop(int *i)
{
*i=stack[top--];
if(top>=0)
return 0;
else
return 1;
}






