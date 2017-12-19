#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

typedef struct BTnode
{
	char data;
	struct BTnode * PL;
	struct BTnode * PR;
}BTNODE,*PBTNODE;

PBTNODE init ();
void xlist(PBTNODE);
void zlist(PBTNODE);
void hlist(PBTNODE);

int main ()
{
	PBTNODE  PA;
	PA=init ();
	xlist(PA);
	printf("\n");
	zlist(PA);
	printf("\n");
	hlist(PA);
	printf("\n");
	system("pause");
	return 0;	
} 


void hlist(PBTNODE PP )
{
	if(NULL!=PP)
	{
		if(NULL!=PP->PL)
		hlist(PP->PL);
		if(NULL!=PP->PR)
		hlist(PP->PR);
		printf("%c ",PP->data);
	}
}


void zlist(PBTNODE PP )
{
	if(NULL!=PP)
	{	
		if(NULL!=PP->PL)
		zlist(PP->PL);
		printf("%c ",PP->data);
		if(NULL!=PP->PR)
		zlist(PP->PR);
	}
}


void xlist(PBTNODE PP )
{
	if(NULL!=PP)
	{
		printf("%c ",PP->data);
		if(NULL!=PP->PL)
		xlist(PP->PL);
		if(NULL!=PP->PR)
		xlist(PP->PR);
	}
}

PBTNODE init ()
{
	PBTNODE PA=(PBTNODE)malloc(sizeof(BTNODE));
	PBTNODE PB=(PBTNODE)malloc(sizeof(BTNODE));
	PBTNODE PC=(PBTNODE)malloc(sizeof(BTNODE));
	PBTNODE PD=(PBTNODE)malloc(sizeof(BTNODE));
	PBTNODE PE=(PBTNODE)malloc(sizeof(BTNODE));
	PBTNODE PF=(PBTNODE)malloc(sizeof(BTNODE));
	PBTNODE PG=(PBTNODE)malloc(sizeof(BTNODE));
	
	PA->data='A';
	PB->data='B';
	PC->data='C';
	PD->data='D';
	PE->data='E';
	PF->data='F';
	PG->data='G';
	
	PA->PL=PB;
	PA->PR=PE;
	PB->PL=PC;
	PB->PR=PD;
	PC->PL=PC->PR=NULL;
	PD->PL=PD->PR=NULL;
	PE->PL=PF;
	PE->PR=PG;
	PF->PL=PF->PR=NULL;
	PG->PL=PG->PR=NULL;
	
	return PA;
}
