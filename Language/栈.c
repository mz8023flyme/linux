#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

typedef struct  node
{
	int data;
	struct node * Pnext;
}NODE,*PNODE;

typedef struct stick
{
	PNODE top;
	PNODE bottom;
}STICK,*PSTICK; 

void init(PSTICK ps);
void list(PSTICK ps);
void pop(PSTICK ps,int num);
void push(PSTICK ps,int val);

int main()
{
	STICK S;
	init(&S);
	push(&S,4);
	push(&S,5);
	push(&S,6);
	push(&S,9);
	push(&S,2);
	list(&S);
	pop(&S,9);
	list(&S);
	return 0;
} 

void pop(PSTICK ps,int num)
{
	PNODE p,q;
	p=q=ps->top;
	while(num--)
	{
		p=ps->top->Pnext;
		ps->top=p;
		free(q);
		q=p;
		if(ps->top==ps->bottom)
		{
			printf("Õ»ÒÑÇå¿Õ!");
			exit(-1);
		}
	}
}

void push(PSTICK ps,int val)
{
	PNODE p;
	p=(PNODE)malloc(sizeof(NODE));
	p->data=val;
	p->Pnext=ps->top;
	ps->top=p;
}

void list(PSTICK ps)
{
	PNODE p;
	p=ps->top;
	while(p!=ps->bottom)
	{
		printf("%d ",p->data);
		p=p->Pnext;
	}
	printf("\n");
}

void init(PSTICK ps)
{
	PNODE Pne;
	Pne=(PNODE)malloc(sizeof(NODE));
	Pne->Pnext=NULL; 
	ps->top=Pne;
	ps->bottom=ps->top;
}

