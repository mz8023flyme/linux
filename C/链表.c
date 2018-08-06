#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

typedef struct NODE
{
	int data;
	struct NODE * Pnext;
}node,*pnode;

void insrct(pnode phead,int num,int val);
void dele(pnode phead,int num);
pnode init();
int find(pnode phead); 
void list(pnode phead);
void sort(pnode phead) ;
int main ()
{
	pnode phead=NULL;
	phead=init();
	list(phead);
	//insrct(phead,2,22);
	//list(phead);
	//dele(phead,3);
	sort(phead);
	list(phead);
	//find(phead);
	//printf("%d",phead->data);
}
void sort(pnode phead) 
{
	int t;
	pnode p,q;
	p=phead->Pnext;
	q=p;
	while(q->Pnext!=NULL)
	{
		p=phead->Pnext;
		while(p->Pnext!=NULL)
		{
			
			if(p->data > p->Pnext->data)
			{
				t=p->data;
				p->data=p->Pnext->data;
				p->Pnext->data=t;
			}
			p=p->Pnext;
		}
		q=q->Pnext;
	}
} 

int find(pnode phead)
{
	int val,i=0,j=0;
	
	pnode q,p;
	p=phead;
	printf("输入要找的数");
	scanf("%d",&val); 
	while(p->Pnext!=NULL)
	{
		p=p->Pnext;
		i++;
		if(p->data==val)
		{
			printf("在%d节点处\n",i);
			j++;	
		} 
	}	
	if(!j)
	{
		printf("没有要找的数");
	}
}

void dele(pnode phead,int num)
{
	int i;
	pnode q,p;
	p=phead;
	for(i=0;i<num-1;i++)
	{
		p=p->Pnext;
		if(p==NULL)
		{
			printf("链表不够长");
			exit(-1);
		}
	}
	q=p->Pnext;
	p->Pnext=q->Pnext;
	free(q);
}

void insrct(pnode phead,int num,int val)
{
	int i;
	pnode q,p;
	p=phead;
	q=(pnode)malloc(sizeof(node));
	if(q==NULL)
	{
		printf("内存失败!");
		exit(-1);
	}
	for(i=0;i<num;i++)
	{
		p=p->Pnext;
		if(p==NULL)
		{
			printf("链表不够长，插入的地方出错");
			exit(-1);
		}
	}
	q->data=val;
	q->Pnext=p->Pnext;
	p->Pnext=q;
}

void list(pnode phead)
{
	pnode p;
	p=phead;
	while(p->Pnext!=NULL)
	{
		printf("%d  ",p->Pnext->data);
		p=p->Pnext;
	}
	printf("\n");
}

pnode init()
{
	int num,i,val;
	pnode phead,ptail;
	phead=(pnode)malloc(sizeof(node));
	if(phead==NULL)
	{
		printf("内存失败!");
		exit(-1);
	}
	ptail=phead;
	ptail->Pnext=NULL;

	printf("输入节点数 ");
	scanf("%d",&num);
	
	for(i=0;i<num;i++)
	{
		pnode  pnew=(pnode)malloc(sizeof(node));
		if(pnew==NULL)
		{
			printf("内存失败!");
			exit(-1);
		}
		printf("输入数据 ");
		scanf("%d",&val);
		pnew->data=val;			//定义一个尾节点tail 
		ptail->Pnext=pnew;
		pnew->Pnext=NULL;
		ptail=pnew;
	}
	return phead;
}


