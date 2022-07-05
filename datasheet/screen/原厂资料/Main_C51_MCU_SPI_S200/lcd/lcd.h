#include "c8051F340.h"
#include <main.h>

#ifndef __LCD_h__
#define __LCD_h__

#ifdef DEBUG
#define END 0xffff
#endif

extern unsigned int code pic_eval[];
extern unsigned char code ascii[]; 
extern unsigned int code debug_code[50][15];

//---------------------------------------------------------------------
void WriteComm(unsigned int i);// reentrant;
void WriteData(unsigned int i);// reentrant;
void WriteDispData(unsigned char DataH,unsigned char DataL);  
void LCD_Init(void);
void BlockWrite(unsigned int Xstart,unsigned int Xend,unsigned int Ystart,unsigned int Yend) reentrant;
void DispBand(void);
void DispColor(unsigned int color);
void DispFrame(void);
void DispPic(unsigned int code *picture);
void DispPicFromSD(unsigned char PicNum);

void DispScaleHor1(void);
void DispScaleVer(void);
void DispScaleVer_Red(void);
void DispScaleVer_Green(void);
void DispScaleVer_Blue(void);
void DispScaleVer_Gray(void);
void DispGrayHor16(void);
void DispGrayHor32(void);
void DispScaleHor2(void);
void DispSnow(void);
void DispBlock(void);

void WriteOneDot(unsigned int color);
unsigned char ToOrd(unsigned char ch); 
void DispOneChar(unsigned char ord,unsigned int Xstart,unsigned int Ystart,unsigned int TextColor,unsigned int BackColor);	 // ord:0~95
void DispStr(unsigned char *str,unsigned int Xstart,unsigned int Ystart,unsigned int TextColor,unsigned int BackColor);
void DispInt(unsigned int i,unsigned int Xstart,unsigned int Ystart,unsigned int TextColor,unsigned int BackColor);

unsigned int ReadData(void);
void DispRegValue(unsigned int RegIndex,unsigned char ParNum);

void Debug(void);

void PutPixel(unsigned int x,unsigned int y,unsigned int color);
void DrawLine(unsigned int Xstart,unsigned int Xend,unsigned int Ystart,unsigned int Yend,unsigned int color);
void DrawGird(unsigned int color);


#endif
