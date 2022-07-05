#ifndef __MAIN_h__
#define __MAIN_h__

typedef  unsigned char   uint8;
typedef  signed char     int8;
typedef  unsigned int    uint16;
typedef  signed int	     int16;   
typedef  unsigned long	 uint32; 
typedef  signed long	 int32; 

extern xdata unsigned char buffer[512];
//------------------------------------------------------
#define ROW 400//320//240//160//240//128//400//480//854//  //显示的行数	  
#define COL 240//240//320//128//240//128//240//320//480//  //显示的列数	 

//定义LCM接口：16位，低8位，高8位，3线SPI，4线SPI,3线2LANE SPI
#define INT_16BIT   
//#define INT_LOW8BIT    
//#define INT_HIGH8BIT  
//#define SPI_3W  
//#define SPI_4W_1L   
//#define SPI_3W_2L 

// 定义	  DRIVER IC
#define ILI9327
//#define ILI9326
//#define ILI9325
//#define NV3023
//#define ILI9163V
//#define ILI9225G
//#define ILI9341V
//#define ILI9340X
//#define ILI9342C
//#define GC9106
//#define ST7735S
//#define NV3022B
//#define ST7775R
//#define GC9203
//#define GC9312
//#define GC9305
//#define GC9306
//#define GC9307
//#define ST7789V
//#define NV3029G
//#define NV3030A2
//#define NV3038B
//#define ST7793
//#define ST7796
//#define ILI9488
//#define ILI9487
//#define R61509V
//#define JD5858A
//#define JD9851
//#define NV3021BW
//#define NV3029C
//#define HX8352A




#define TP_TEST		    //测试功能的选择，选择常量被定义，相应测试功能开启； TP_TEST:TP划线测试功能
//#define READ_REG		    //读ID CODE功能
//#define SLEEP_TEST	    //进入、退出sleep功能
//#define DEBUG			    //调试功能，把要调试的指令和参数写进数组内，每按键执行一组指令和参数

#define EVAL_PIC		    //使用MCU片上64k Flash()内的图片，160*160填充显示区域
#define EXTEND_TEST	    //扩展测试功能，如，灰度，色彩渐变等画面
//------------------------------------------------------
#define CHAR_FONT_W8_H16	//选择显示字符的字体，CHAR_FONT_W8_H16：8*16；CHAR_FONT_W16_H21：16*21
//#define CHAR_FONT_W16_H21

#define CONN_USB		    //在按键等待时连接USB进行数据通信、交换（以此实现USB调试功能）	
#define USE_SD_INIT			//SD卡初始化
#define SD_DEMO_PIC			//显示SD卡内的图片以用于评估和展示
//#define AUTO_TEST			//若定义AUTO-TEST，等待按键时不必按按键，延时一段后自动进行下一步的测试	
//------------------------------------------------------
#define DBH  P2				//接口定义部分
#define DBL  P4

//SPI
//浦洋液晶 SDA=RD SCL=WR CS=DB8 
sbit	SPI_DI=P1^6;  //RD
sbit	SPI_CLK=P1^7; //WR
//sbit	CS0 = P2^0;	  //  SPI CS接DB8

//智越科技 SDA=DB8 SCL=DB9 CS=CS 
//sbit	SPI_DI=P2^0;  							
//sbit	SPI_CLK=P2^1;
	
//MCU  
sbit	CS0 = P3^0;   //  MCU CS接CS
sbit	RST = P0^5;
sbit	RS  = P3^1;
sbit	WR0 = P1^7;
sbit	RD0 = P1^6;

sbit	OE = P0^6;//P0^7;//
sbit	DIR = P0^7;//P0^6;//

//TP 
sbit    TP_CS	  = P1^4;
sbit    TP_PENIRQ = P3^5;
sbit    TP_DOUT   = P3^3;
sbit    TP_DIN	  = P3^2;
sbit    TP_DCLK   = P1^5;
//sbit    TP_BUSY	  = P3^4;

//KEY
sbit    KEY_STEP  = P3^6;	//定义“下一步”按键
sbit    KEY_Stop  = P3^7;
//------------------------------------------------------
#ifdef  READ_REG	
	#define STRING_FUNCTION
    #define READ_FUNCTION
#endif

#ifdef  DEBUG	
	#define STRING_FUNCTION
#endif

#ifdef  CONN_USB	
	#define STRING_FUNCTION
    #define READ_FUNCTION
#endif

#ifdef  TP_TEST	
	#define DRAW_FUNCTION
	#define STRING_FUNCTION
#endif

#ifdef AUTO_TEST
	#define AUTO_NEXT
#else
	#define MANUA_NEXT
#endif	

//#define STRING_FUNCTION	  //定义字符串处理和显示的函数
//#define DRAW_FUNCTION		  //定义TP测试的函数
//#define READ_FUNCTION		  //定义读取IC操作的函数
//------------------------------------------------------
#ifdef  CHAR_FONT_W8_H16	
	#define  FONT_W  8
	#define  FONT_H  16
#endif
#ifdef  CHAR_FONT_W16_H21
	#define  FONT_W  16
	#define  FONT_H  21
#endif
//------------------------------------------------------
#define PIC_WIDTH    160	 //预备向LCD显示区域填充的图片的大小
#define PIC_HEIGHT   160

#define RED    0xF800		 //定义颜色常量 
#define GREEN  0x07E0
#define BLUE   0x001F
#define WHITE  0xFFFF
#define BLACK  0x0000
#define GRAY   0xEF5D	     //0x2410
#define GRAY75 0x39E7 
#define GRAY50 0x7BEF	
#define GRAY25 0xADB5	

#if 0
#define X_min 0x0043		 //TP测试范围常量定义
#define X_max 0x07AE
#define Y_min 0x00A1
#define Y_max 0x0759
#endif

#define X_min 0x0043		 //TP测试范围常量定义
#define X_max 0x02e8//0x03e8	  //0x07ae
#define Y_min 0x0060	   //0x00a1
#define Y_max 0x0360	  //0x0759
//------------------------------------------------------
void  Delay(unsigned int dly);

void StopDelay(unsigned int sdly);
void  WaitKey(void);
void  ConnToUSB(void);
void  Suspend_Device(void);
void  Devices_Init(void);
void  MCU_Init(void); 

#endif