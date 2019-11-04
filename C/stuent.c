#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <conio.h>
#include <windows.h>

typedef struct student {
	
	char name[10];
    char sex[5];
    
    int num;
    float sum;
    float math;
    float eng;
    float politi;
    float professional;

    struct student * next;
} stu,*pstu;

FILE *fp;
char buff[100];
int cnt=0;
pstu head=NULL;

pstu Split_str(const char * buff,pstu q)
{
	char * res;
	res = strtok((char*)buff, " ");
	if(res)
    {
        q->num = atoi(res);
    }
	res = strtok(NULL, " ");
    if(res)
    {
        strcpy( q->name,res);
    }
	res = strtok(NULL, " ");
    if(res)
    {
        strcpy( q->sex,res);
    }
	res = strtok(NULL, " ");
	if(res)
    {
        q->math = atof(res);
    }
	res = strtok(NULL, " ");
	if(res)
    {
        q->eng = atof(res);
    }
	res = strtok(NULL, " ");
	if(res)
    {
        q->politi = atof(res);
    }
	res = strtok(NULL, " ");
	if(res)
    {
        q->professional = atof(res);
    }
	res = strtok(NULL, " ");
	if(res)
    {
        q->sum = atof(res);
    }
	return q;	
}

void read_data(void)
{
    pstu p=NULL,q=NULL;
    head=(pstu)malloc(sizeof(stu));
	if(head==NULL) 
	{
        printf("分配内存失败!");
        exit(-1);
    }
    head->next = NULL;
    p=head;
    if( (fp=fopen(".\\stu.txt","a+") )== NULL ) 
	{
        printf("Fail to open file!\n");
        exit(-1);
    }
    while (fgets(buff,sizeof(stu)+1,fp)!=NULL)
	{
        q=(pstu)malloc(sizeof(stu));
        if(q==NULL) 
		{
            printf("分配内存失败!");
            exit(-1);
        } 

        q=Split_str(buff,q);
		p->next=q;
		p=q;
		p->next=NULL; 
		cnt++;	
    }
    
    fclose(fp);
}

void display(pstu stu)
{
	pstu p=NULL;
	p=head->next; 
	float math_sum=0;
	float eng_sum=0;
	float politi_sum=0;
	float professional_sum=0;
	float sum_sum=0;
	printf("+---------------------------------------------------------------+\n");
	printf("| 学号  | 姓名\t|  性别\t| 数学\t| 英语\t| 政治\t|专业课\t| 总分\t|\n"); 
	printf("+-------+-------+-------+-------+-------+-------+-------+-------+\n");
	while(p!=NULL)
	{ 
		printf("|  %2d\t| %s\t|  %s\t| %.1f\t| %.1f\t| %.1f\t| %.1f\t| %.1f\t|\n",p->num,p->name,p->sex,p->math,p->eng,p->politi,p->professional,p->sum);
		math_sum+=p->math;
		eng_sum+=p->eng;
		politi_sum+=p->politi;
		professional_sum+=p->professional;
		sum_sum+=p->sum;
		p=p->next;
	}
	printf("+---------------------------------------------------------------+\n");
	printf("|平均分 |  \t|  \t| %.1f\t| %.1f\t| %.1f\t| %.1f\t| %.1f\t|\n",math_sum/cnt,eng_sum/cnt,politi_sum/cnt,professional_sum/cnt,sum_sum/cnt);
	printf("+-------+-------+-------+-------+-------+-------+-------+-------+\n");
}

void write_data(void)
{
	pstu p=head->next;
	char tmp[200];
	if( (fp=fopen(".\\stu.txt","w") )== NULL ) 
	{
        printf("Fail to open file!\n");
        exit(-1);
    }
	while(p!=NULL)
	{
		sprintf(tmp,"%d %s %s %.1f %.1f %.1f %.1f %.1f\n",p->num,p->name,p->sex,p->math,p->eng,p->politi,p->professional,p->sum);
		fputs(tmp,fp);
		p=p->next;
	} 
} 

void free_stu(pstu p)
{
	pstu q;
	q=p->next;
	while(q!=NULL)
	{
		free(p);
		p=q;
		q=p->next;
	}
	
} 

void node(pstu p) 
{
	printf("姓名：");
	scanf("%s",&p->name);
sex:printf("性别：");
	scanf("%s",&p->sex);
	if( (strcmp(p->sex,"女")!=0 ) && (strcmp(p->sex,"男")!=0) )
	{ 
		printf("只能输入男或女\n"); 
		goto sex;
	} 
m:	printf("数学成绩：");
    scanf("%f",&p->math);
    if(p->math<0||p->math>100)
    {
    	printf("请输入正确分数（0-100）\n");
		goto m;
	} 
e:	printf("英语成绩：");
    scanf("%f",&p->eng);
    if(p->eng<0||p->eng>100)
    {
    	printf("请输入正确分数（0-100）\n");
		goto e;
	}
po:	printf("政治成绩：");
    scanf("%f",&p->politi);
    if(p->politi<0||p->politi>100)
    {
    	printf("请输入正确分数（0-100）\n");
		goto po;
	}
pr:	printf("专业课成绩：");
    scanf("%f",&p->professional);
    if(p->professional<0||p->professional>100)
    {
    	printf("请输入正确分数（0-100）\n");
		goto pr; 
	}
    p->sum=p->math+p->eng+p->politi+p->professional;
    
}
void add_stu(void)
{
	pstu p=p=head->next,q;
	int num=0,index=0;
	char c;
	
	printf("学号:");
	scanf("%d",&num);
	while(p!=NULL)
	{
		if(p->num==num)
		{
ss:			printf("这个学号信息已存在，是否修改？\n");
			printf("1)是  0)否\n");
			c=getch();
			switch(c)
			{
				case '1':
					node(p);
					return ; 
					break;
				case '0':
					return;
				break;
				default :
					goto ss;
					break;
			}
		}
		p=p->next;
	}	
	q=(pstu)malloc(sizeof(stu));
	if(q==NULL) 
	{
	    printf("分配内存失败!");
	    exit(-1);
	}
	node(q);
	q->num=num;
	q->next=head->next;
	head->next=q;
	cnt++;
}
void modifier(void)
{
	int num=0,index=0; 
	char c;
	pstu p=head->next,q=NULL;
	printf("学号:");
	scanf("%d",&num);
	while(p!=NULL)
	{
		if(p->num==num)
		{
			node(p);
			return ;
		}
		p=p->next;
	}
sw:	printf("没有这个学号信息，是否改为添加？\n");
    printf("1)是  0)否\n");
	c=getch();
	switch(c)
	{
		case '1':
			q=(pstu)malloc(sizeof(stu));
	        if(q==NULL) 
			{
	            printf("分配内存失败!");
	            exit(-1);
	        }
	        node(q);
	        q->num=num;
	        q->next=head->next;
	        head->next=q;
	        cnt++;
	        return;
		break;	
		case '0':
			return;
		break;
		default:
			goto sw;
		break;
	}
}
void del_stu(void)
{
	int num=0;
	printf("请输入需要删除的学号:");
	scanf("%d",&num);
	pstu p=head,q=NULL;
	head->num=-1;
	while(p!=NULL)
	{
		if(p->num==num)
		{
			q->next=p->next;
			free(p);
			cnt--;
			printf("删除成功！\n");
			Sleep(1000);
			return;
		}
		q=p;
		p=p->next;
	}
	printf("没有这个学号，无需删除！\n");
	getch();
}

pstu sorted(pstu sub_root,int chose)
{
	if (sub_root->next)
	{
		pstu second_half = NULL;
		pstu first_half = sub_root;
		pstu temp = sub_root->next->next;
		while (temp)
		{
			first_half = first_half->next;
			temp = temp->next;
			if(temp)
			temp = temp->next;
		}
		second_half = first_half->next;
		first_half->next = NULL;
		pstu lChild = sorted(sub_root,chose);
		pstu rChild = sorted(second_half,chose);
		if(chose)
		{
			if (lChild->num < rChild->num)
			{
				sub_root = temp = lChild;
				lChild = lChild->next;
			}
			else
			{
				sub_root = temp = rChild;
				rChild = rChild->next;
			}
			while (lChild&&rChild)
			{
				if (lChild->num < rChild->num )
				{
					temp->next = lChild;
					temp = temp->next;
					lChild = lChild->next;
				}
				else
				{
					temp->next = rChild;
					temp = temp->next;
					rChild = rChild->next;
				}
			}
		} 
		else
		{
			if (lChild->sum > rChild->sum)
			{
				sub_root = temp = lChild;
				lChild = lChild->next;
			}
			else
			{
				sub_root = temp = rChild;
				rChild = rChild->next;
			}
			while (lChild&&rChild)
			{
				if (lChild->sum > rChild->sum )
				{
					temp->next = lChild;
					temp = temp->next;
					lChild = lChild->next;
				}
				else
				{
					temp->next = rChild;
					temp = temp->next;
					rChild = rChild->next;
				}
			}
		} 
		
		if (lChild)
		temp->next = lChild;
		else
		temp->next = rChild;
	}
	return sub_root;

}

void inquire(void)
{
	char c;
	int num;
	pstu p=head->next,q=NULL;
	
	printf("**********************学生管理系统**********************\n");
	printf("*                                                      *\n");
	printf("*               当前共有 %d 名学生信息                  *\n",cnt);
	printf("*                                                      *\n");
	printf("*                1)按学号升序显示                      *\n");
	printf("*                2)按总分降序显示                      *\n");
	printf("*                3)查询单个学号学生                    *\n");
	printf("*                0)返回主菜单                          *\n");
	printf("*                                                      *\n");
	printf("********************************************************\n");
	c=getch();
	switch(c)
	{
		case '1':
			head->next = sorted(head->next,1);
			system("cls");//清屏
			display(head);
			getch();
			break;
		case '2':
				head->next = sorted(head->next,0);
				system("cls");//清屏
				display(head);
				getch();
			break;
		case '3':
			printf("请输入学号\n");
			scanf("%d",&num);
			while(p!=NULL)
			{
				if(p->num==num)
				{
					system("cls");//清屏
					printf("+---------------------------------------------------------------+\n");
					printf("|学号 |  姓名\t|  性别\t| 数学\t| 英语\t| 政治\t|专业课\t| 总分\t|\n"); 
					printf("+-----+---------+-------+-------+-------+-------+-------+-------+\n");
					printf("| %2d  | %s\t|  %s\t| %.1f\t| %.1f\t| %.1f\t| %.1f\t| %.1f\t|\n",p->num,p->name,p->sex,p->math,p->eng,p->politi,p->professional,p->sum);
					printf("+---------------------------------------------------------------+\n");
					getch();
					return ;
				}
				p=p->next;
			}
			printf("查无此人\n");
			break;
		case '0':
			return ;				
		default :
				system("cls");//清屏
				inquire();
			break;
	}

}

void mian_display(void)
{
	printf("**********************学生管理系统**********************\n");
	printf("*                                                      *\n");
	printf("*             --------------------------               *\n"); 
	printf("*             |   作 者：     life     |               *\n");
	printf("*             --------------------------               *\n"); 
	printf("*                                                      *\n");
	printf("*               当前共有 %d 名学生信息                  *\n",cnt);
	printf("*                                                      *\n");
	printf("*                   1)添加学生信息                     *\n");
	printf("*                   2)删除学生信息                     *\n");
	printf("*                   3)查询学生信息                     *\n");
	printf("*                   4)修改学生信息                     *\n");
	printf("*                   0)退出软件                         *\n");
	printf("*                                                      *\n");
	printf("********************************************************\n");

}
int main()
{
	char c; 
	read_data(); 
	
	while(1)
	{
		
		system("cls");//清屏
		mian_display();
		c=getch();
		switch(c)
		{
			case '1':
				add_stu();
				break;
			case '2':
				del_stu();
				break;
			case '3':
				system("cls");//清屏
				inquire();
				break;
			case '4':
				modifier();
				break;
			case '0':
				write_data();
				free_stu(head);
				exit(0);
				break;
			default:
				break;
		} 
		
	}
}

