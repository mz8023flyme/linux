#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

typedef struct node 
{
	int data;
	struct node *pnext;
}NODE,*PNODE;

typedef struct link
{
	PNODE head;
	PNODE end;
}LINK,*PLINK;

void init(PLINK); 
void list(PLINK);
void in_link(PLINK,int);
void out_link(PLINK);

int main()
{
	LINK link;
	init(&link);
	in_link(&link,2);
	in_link(&link,25);
	in_link(&link,6);
	in_link(&link,1);
	in_link(&link,36);
	in_link(&link,54);
	list(&link);
	out_link(&link);
	list(&link);
	out_link(&link);
	list(&link);
	out_link(&link);
	list(&link);
	in_link(&link,7);
	list(&link);
	return 0;	
}

void out_link(PLINK pp)
{
	PNODE	pa;
	pa=pp->head;
	pp->head=pp->head->pnext;
	free(pa);
}

void in_link(PLINK pp,int val)
{
	PNODE pa=(PNODE)malloc(sizeof(NODE));
	pa->pnext=NULL;
	pp->end->data=val;
	pp->end->pnext=pa;
	pp->end=pa;	
} 

void list(PLINK pp)
{
	PNODE pa;
	pa=pp->head;
	if(pp->end==pp->head)
	{
		printf("empty!");
	}
	while(pp->end!=pa)
	{
		printf("%d ",pa->data);
		pa=pa->pnext;
	}
	printf("\n");
}

void init(PLINK pp)
{
	PNODE	pa=(PNODE)malloc(sizeof(NODE));
	pa->pnext=NULL;
	pp->end=pp->head=pa;
}

