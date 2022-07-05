#include "c8051F340.h"
#include <intrins.h>
#include <absacc.h>
 
#include<main.h>
#include<mmc_sd\mmc_sd.h>
#include<lcd\lcd.h>
#include <tp\tp.h>

#ifdef TP_TEST
#if 0
unsigned int TP_GetPosition(unsigned char cmd);


//增加TP校准程序
long int  xdata Tp_a,Tp_b,Tp_c,Tp_d,Tp_e,Tp_f;
void Tp_Adjust(void)
{
	unsigned long int X[3],Y[3];

	DispStr("+",6,2,RED,WHITE);
	TP_PENIRQ = 1;
	while (TP_PENIRQ) 
	{
		while(!TP_PENIRQ)
		{	 
			X[0]=TP_GetPosition(0xd0);	
			Y[0]=TP_GetPosition(0x90);
			Delay(15);
		}
		Delay(50);
	} 
	DispStr("+",116,152,RED,WHITE);
	TP_PENIRQ = 1; 
	Delay(5000);
	while (TP_PENIRQ)
	{	
		while(!TP_PENIRQ)
		{	 
			X[1]=TP_GetPosition(0xd0);	
			Y[1]=TP_GetPosition(0x90);
			Delay(15);
		}
		Delay(50);
	}

	DispStr("+",206,302,RED,WHITE);
	TP_PENIRQ = 1;
	Delay(5000);
	while (TP_PENIRQ)
	{
		while(!TP_PENIRQ)
		{	 
			X[2]=TP_GetPosition(0xd0);	
			Y[2]=TP_GetPosition(0x90);
			Delay(15);
		}
	}
	Delay(50);

	Tp_a=200*(Y[1]-Y[2])-110*(Y[0]-Y[2]);
	Tp_b=(X[0]-X[2])*110-200*(X[1]-X[2]);
	Tp_c=Y[0]*(120*X[2]-10*X[1])+Y[1]*(10*X[0]-210*X[2])+Y[2]*(210*X[1]-120*X[0]);
	Tp_d=300*(Y[1]-Y[2])-150*(Y[0]-Y[2]);
	Tp_e=150*(X[0]-X[2])-300*(X[1]-X[2]);
	Tp_f=Y[0]*(160*X[2]-10*X[1])+Y[1]*(10*X[0]-310*X[2])+Y[2]*(310*X[1]-160*X[1]);

}


////add by rao 2016.6.21
#endif
void TP_Write(unsigned char cmd)
{
	unsigned char xdata i;

	TP_CS=0;
	TP_DCLK=0;

	for(i=0;i<8;i++)
	{
		if((cmd&0x80)==0)
		TP_DIN=0;
		else
		TP_DIN=1;
		TP_DCLK=1;
		cmd=cmd<<1;
		TP_DCLK=0;//be low ,before read
	}
}

unsigned int TP_Read(void)
{
	unsigned char xdata i;
	long int xdata 	val=0;

	TP_CS=0;
	TP_DOUT = 1;

	for(i=0;i<12;i++)
	{
		TP_DCLK=0;
		if(TP_DOUT)
		{
			val=val|(1<<(11-i));
		}
		TP_DCLK=1;
	}

	TP_CS=1;
	return val;
}

unsigned int TP_GetPosition(unsigned char cmd)
{
	unsigned int xdata temp;
	unsigned char xdata i=0,j=0,k=0;
	unsigned int  xdata TP[11];

	TP_PENIRQ = 1;

	while((i<11)&&(!TP_PENIRQ))
	{
		TP_CS=0;
		TP_Write(cmd+1);
		TP[i]=TP_Read();
		i++;
		TP_CS=1;
	}
	if(i!=0)
	{
		for(j=0;j<i;j++)
			for(k=0;k<i-j;k++)
				if (*(TP+k)>*(TP+k+1))
					{
					temp=*(TP+k);
					*(TP+k)=*(TP+k+1);
					*(TP+k+1)=temp;
				}
		return (TP[i/2]);
	}
	else
		return 0;
}

void TP_DrawLine(void)
{
	unsigned long int  xdata gx=0,gy=0;       
	unsigned long int  xdata tx=0,ty=0;           
	unsigned long int  xdata gxtemp=0,gytemp=0;           

	DispColor(BLACK);	//TP测试黑色画面
//	DrawGird(GRAY25);	//TP测试网格画面
	DispStr("TP TEST",0,0,BLACK,WHITE);	 //TP测试画面显示 TP TEST 字符


	TP_PENIRQ = 1;
	while (TP_PENIRQ) ;

	while(1)
	{

		while(KEY_STEP)
		{	
			Delay(50);//!
			while(!TP_PENIRQ)
			{	 
				#if 1
				tx=TP_GetPosition(0xd0);
				gx=(tx-X_min)*(unsigned long int)(COL)/((X_max-X_min))*0.77;
		
				ty=TP_GetPosition(0x90);
				gy=(ty-Y_min)*(unsigned long int)(ROW)/((Y_max-Y_min))*0.95;

				Delay(10);//
			//nv3029g从IC侧刷屏	  240*320
			//	gx=240-gx;	
			//	gy=320-gy;

			//nv3029g从IC侧刷屏	  320*480
//				gx=320-gx;	
//				gy=480-gy;

			////	touch panel must be interrupted
				if((!TP_PENIRQ)&&(gx>=0&&gx<COL)&&(gy>=0&&gy<ROW))
				{					   					
//					DispInt(gx,50,10,BLUE,WHITE);	//校正用
//					DispInt(gy,50,50,BLUE,WHITE);
					PutPixel(gx,gy,RED); 
				}
				else 
				{
//					DispColor(0xf800);
//					DispInt(gx>>16,10,10,RED,WHITE);	 DispInt(gx,50,10,RED,WHITE);
//					DispInt(gy>>16,10,50,RED,WHITE);	 DispInt(gy,50,50,RED,WHITE);
				}
				#endif

			}

			
		}
		
		DispColor(BLACK);
//		DrawGird(GRAY25);
		DispStr("TP TEST",0,0,BLACK,WHITE);

	}

}
#endif