
#define	GPFCON		(*(volatile unsigned long *)0x56000050)
#define	GPFDAT		(*(volatile unsigned long *)0x56000054)

#define	GPF4_out	(1<<(4*2))   
#define	GPF5_out	(1<<(5*2))
#define	GPF6_out	(1<<(6*2))

void delay(unsigned int t)
{
	int i=0;
	for (t;t>0;t--)
	{
		for(i=0;i<1000;i++)	;
	}

}

int main(void)
{
	unsigned long i = 0;

	GPFCON = GPF4_out|GPF5_out|GPF6_out;		// 将LED1,2,4对应的GPF4/5/6三个引脚设为输出
	
	GPFDAT=0xffffffff;
	while(1)
	{
		delay(100);
		GPFDAT&=~(1<<4);	
			GPFDAT|=1<<6;
		delay(100);
		GPFDAT|=1<<4;
		GPFDAT&=~(1<<5);
		delay(100);
		
		GPFDAT|=1<<5;
		GPFDAT&=~(1<<6);
	}

	return 0;  //测试成功
}
