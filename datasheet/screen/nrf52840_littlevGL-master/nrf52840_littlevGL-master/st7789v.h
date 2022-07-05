
#ifndef _ST7789V_H_
#define _ST7789V_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nrf.h"
#include "nrfx_spim.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "lvgl.h"

#define SPI_INSTANCE  3     /**< SPI instance index. */


#define CONFIG_LCD_TYPE_ST7789V

// #define SPI_LINE_3//使用的是几线spi，也就是说RS线是否合并到spi传输�

/********************* 采购店铺 ********************/
//0:    1.54�电子技术商�https://item.taobao.com/item.htm?spm=a1z09.2.0.0.54982e8dG0xp9s&id=557941066304&_u=v9qfoh84e10
//1:    1.54�阿美��//2:    1.30�中景�https://item.taobao.com/item.htm?spm=a1z1r.7974869.0.0.269c3ad4lZRlEd&id=565591692266
#define LCD_SHOP_ST7789 1


#define TFT_HOR_RES (240)
#define TFT_VER_RES (240)

//-----------------------------SPI ��������--------------------------------------//

#define PIN_NUM_MISO (28)
#define PIN_NUM_MOSI (4)
#define PIN_NUM_CLK  (3)
#define PIN_NUM_CS   (29)

#define PIN_NUM_DC   (30)	//低电平：命令。高电平：内存数据或者参�
#define PIN_NUM_RST  (25)
#define PIN_NUM_BCKL (31)


#define LCD_CS_H	 
#define LCD_CS_L	 

#define LCD_RST_H    
#define LCD_RST_L	 

#define LCD_RS_H	 
#define LCD_RS_L	 


//To speed up transfers, every SPI transfer sends a bunch of lines. This define specifies how many. More means more memory use,
//but less overhead for setting up / finishing transfers. Make sure 240 is dividable by this.
#define PARALLEL_LINES 16




/*
 The LCD needs a bunch of command/argument values to be initialized. They are stored in this struct.
*/
typedef struct {
    uint8_t cmd;
    uint16_t data[16];
    uint8_t databytes; //No of data in data; bit 7 = delay after set; 0xFF = end of cmds.
} lcd_init_cmd_t;






#define RED  	0xf800
#define GREEN	0x07e0
#define BLUE 	0x001f
#define WHITE	0xffff
#define BLACK	0x0000
#define YELLOW  0xFFE0
#define GRAY0   0xEF7D   	//��ɫ0 3165 00110 001011 00101
#define GRAY1   0x8410      	//��ɫ1      00000 000000 00000
#define GRAY2   0x4208      	//��ɫ2  1111111111011111










void st7789_init(void);

//void LCD_GPIO_Init(void);
void Lcd_WriteIndex(uint8_t Index);
void Lcd_WriteData(uint8_t Data);
void Lcd_WriteReg(uint8_t Index,uint8_t Data);
uint16_t Lcd_ReadReg(uint8_t LCD_Reg);
void Lcd_Reset(void);

void Lcd_Clear(uint16_t Color);
void Lcd_SetXY(uint16_t x,uint16_t y);
void Gui_DrawPoint(uint16_t x,uint16_t y,uint16_t Data);
unsigned int Lcd_ReadPoint(uint16_t x,uint16_t y);
void Lcd_SetRegion(uint16_t x_start,uint16_t y_start,uint16_t x_end,uint16_t y_end);
void Lcd_WriteData_16Bit(uint16_t Data);
void Lcd_DrawImage(uint16_t x,uint16_t y, uint16_t width, uint16_t height,const uint8_t * data);



void st7789_flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t * color_map);
void st7789_map(int32_t x1, int32_t y1, int32_t x2, int32_t y2,  const lv_color_t * color_p);
void st7789_fill(int32_t x1, int32_t y1, int32_t x2, int32_t y2,  lv_color_t color);
#endif


