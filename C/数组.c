#include <stdio.h> 
#include <stdlib.h>
#include <malloc.h>

typedef struct pArr
{
	int len;
	int cnt;
	int * pBase;
}Arr,*pArr;

void insect(pArr arrve,int num,int val);
int dele(pArr arrve,int num);
void show(pArr arrve);
void init(pArr arrve,int num);
int empty(pArr arrve);
int full(pArr arrve);
void sort(pArr arrve);
int main()
{
	Arr   shu; 
	init(&shu,5);
	insect(&shu,0,2);
	insect(&shu,1,8);
	insect(&shu,2,1);
	insect(&shu,3,7);
	show(&shu);
	sort(&shu);
	show(&shu);
	
	system("pause"); 
	return 0;
} 

void sort(pArr arrve)
{
	int i,j,t=0;
	for(j=0;j<arrve->cnt-1;j++)
		for(i=0;i<arrve->cnt-1;i++)
		{
			if(arrve->pBase[i]>arrve->pBase[i+1])
			{
				t=arrve->pBase[i];
				arrve->pBase[i]=arrve->pBase[i+1];
				arrve->pBase[i+1]=t;
			}
		}
}

int dele(pArr arrve,int num)
{
	int i;
	if(num>arrve->cnt||num<0)
	{
		printf("ɾ������"); 
	}
	else
	{
		for(i=num;i<arrve->cnt;i++)
		{
			arrve->pBase[i-1]=arrve->pBase[i];
		} 
		arrve->cnt--;
	}
}

int full(pArr arrve)
{
	if(arrve->cnt==arrve->len)
	{
		printf("��������\n");
		return -1;
	}
	return 0;
}


int empty(pArr arrve)
{
	if(arrve->cnt==0)
	{
		printf("����Ϊ��\n");
		return -1;
	}
	return 0;
}


void insect(pArr arrve,int num,int val)
{
	int i;
	if(arrve->len<=num||num<0)
	{
		printf("���鲻����\n");
		exit(-1);
	}
	else
	{
		for( i=arrve->cnt;i>num ;i-- )
		{
			 (arrve->pBase[i])=(arrve->pBase[i-1]);
		}
		arrve->pBase[num]=val;
		arrve->cnt++;
	}
	
}


void show(pArr  arrve)
{
	int i;
	if(empty(arrve))
	{
		printf("����Ϊ��\n"); 
	}
	else
	{
		for(i=0;i<arrve->cnt;i++)
		{
			printf("%d  ",arrve->pBase[i]);
		}
		
    }
    printf("\n");
}

void init(pArr  arrve,int num)
{
	
	arrve->pBase=(int *)malloc(sizeof(int)*num);
	
	if(NULL == arrve->pBase)
	{
		printf("�ڴ����ʧ��"); 
		exit(-1);
	}
	 else
	 {
	 	arrve->len=num;
	 	arrve->cnt=0;
	 }
	
}

