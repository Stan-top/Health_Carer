#include "st7789v.h"



static const nrfx_spim_t spi = NRFX_SPIM_INSTANCE(SPI_INSTANCE);  /**< SPI instance. */
static volatile bool spi_xfer_done;  /**< Flag used to indicate that SPI instance completed the transfer. */
void spim_event_handler(nrfx_spim_evt_t const * p_event,
                       void *                  p_context)
{
    spi_xfer_done = true;
    
 
}


//Place data into DRAM. Constant data gets placed into DROM by default, which is not accessible by DMA.
static  lcd_init_cmd_t st_init_cmds[]={
    /* Sleep Out */
    {0x11, {0}, 0x80},
    /* Memory Data Access Control, MX=MV=1, MY=ML=MH=0, RGB=0 */
    {0x36, {0X00}, 1},
    /* Interface Pixel Format, 16bits/pixel for RGB/MCU interface */
    {0x3A, {0x05}, 1},
    /* Porch Setting */
    {0xB2, {0x0c, 0x0c, 0x00, 0x33, 0x33}, 5},
    /* Gate Control, Vgh=13.65V, Vgl=-10.43V */
    {0xB7, {0x35}, 1},
#if  LCD_SHOP_ST7789==0 
    /* VCOM Setting, VCOM=1.175V */
    {0xBB, {0x1a}, 1}, 
    /* LCM Control, XOR: BGR, MX, MH */
    {0xC0, {0x3C}, 1},
#elif  LCD_SHOP_ST7789==2      
    {0xBB, {0x19}, 1},    
    /* LCM Control, XOR: BGR, MX, MH */
    {0xC0, {0x3C}, 1},
#endif    
    /* VDV and VRH Command Enable, enable=1 */
    {0xC2, {0x01}, 1},
#if LCD_SHOP_ST7789==0 
    /* VRH Set, Vap=4.4+... */
    {0xC3, {0x0b}, 1},
#elif  LCD_SHOP_ST7789==2     
    {0xC3, {0x12}, 1},    
#endif
    /* VDV Set, VDV=0 */
    {0xC4, {0x20}, 1},
    /* Frame Rate Control, 60Hz, inversion=0 */
    {0xC6, {0x0f}, 1},
    /* Power Control 1, AVDD=6.8V, AVCL=-4.8V, VDDS=2.3V */
    {0xD0, {0xA4, 0xA1}, 2},
/*           以下内容很奇怪，注释掉就显示颜色正常，但是店家给的设置里包含这些         */    
// #if LCD_SHOP_ST7789==0 
//  /* Positive Voltage Gamma Control */
//     {0xE0, {0xD0, 0x00, 0x05, 0x0E, 0x15, 0x0D, 0x37, 0x43, 0x47, 0x09, 0x15, 0x12, 0x16, 0x19}, 14},
//     /* Negative Voltage Gamma Control */
//     {0xE1, {0xD0, 0x00, 0x05, 0x0D, 0x0C, 0x06, 0x2D, 0x44, 0x40, 0x0E, 0x1C, 0x18, 0x16, 0x19}, 14},
// #elif  LCD_SHOP_ST7789==2     /* Positive Voltage Gamma Control */
// /* Positive Voltage Gamma Control */   
//     {0xE0, {0xd0, 0x04, 0x0d, 0x11, 0x13, 0x2b, 0x3f, 0x54, 0x4c, 0x18, 0x0d, 0x0b, 0x1f, 0x23}, 14},
//     /* Negative Voltage Gamma Control */
//     {0xE1, {0xd0, 0x04, 0x0c, 0x11, 0x13, 0x2c, 0x3f, 0x44, 0x51, 0x2f, 0x1f, 0x1f, 0x20, 0x23}, 14},
// #endif    
/*                                     end                                   */   
    /* Display On */
    {0x29, {0}, 0x80},
    {0, {0}, 0xff}
};



//Send a command to the LCD. Uses spi_device_transmit, which waits until the transfer is complete.
static void lcd_cmd( uint8_t cmd)
{
 
    spi_xfer_done = false;
  
#if defined(SPI_LINE_3)   
	 
#endif
    
#if !defined(SPI_LINE_3)   
   
#endif
  
  nrfx_spim_xfer_desc_t xfer_desc = NRFX_SPIM_XFER_TX(&cmd, 1);
  APP_ERROR_CHECK(nrfx_spim_xfer_dcx(&spi, &xfer_desc, 0,1));
    while(spi_xfer_done==false)
    {}
}

//Send data to the LCD. Uses spi_device_transmit, which waits until the transfer is complete.
static void lcd_data( uint8_t *data, int len)
{
    spi_xfer_done = false;
    if (len==0) return;             //no need to send anything
#if defined(SPI_LINE_3)  
    for(uint16_t i=0;i<len;i++)
    {       
        memset(&t, 0, sizeof(t));       //Zero out the transaction    
        t.addr = 1;
        t.length=1*8;                 //Len is in bytes, transaction length is in bits.
        t.tx_buffer=&data[i];               //Data
        // t.user=(void*)1;                //D/C needs to be set to 1
        ret=spi_device_transmit(spi, &t);  //Transmit!
        assert(ret==ESP_OK);            //Should have had no issues.
      //   vTaskDelay(1 / portTICK_RATE_MS);
    }
  
 #else
      nrfx_spim_xfer_desc_t xfer_desc = NRFX_SPIM_XFER_TX(data, len);
      APP_ERROR_CHECK(nrfx_spim_xfer_dcx(&spi, &xfer_desc, 0,0));   
#endif
 while(spi_xfer_done==false){}
    
}



static uint16_t rgb2bgr(uint16_t rgb)
{
	uint16_t r= rgb>>11;
	uint16_t g=(rgb>>5)&0x3f;
	uint16_t b = rgb&0x1f;
	return (b<<11)|(g<<5)|r;
}
//---------------------------------function----------------------------------------------------//






/****************************************************************************
* ��    �ƣ�void Lcd_Reset(void)
* ��    �ܣ�Һ��Ӳ��λ����
* ��ڲ������ￊ* ���ڲ�������
* ˵    ����Һ����ʼ��ǰ��ִ��һ�θ�λ����
****************************************************************************/
void Lcd_Reset(void)
{
	// LCD_RST_L;
	// Delay_ms(100);
	// LCD_RST_H;
	// Delay_ms(50);
}




/*************************************************
��������LCD_Set_XY
���ܣ�����lcd��ʾ��ʼ��
��ڲ�����xy����
����ֵ����
*************************************************/
void Lcd_SetXY(uint16_t Xpos, uint16_t Ypos)
{	
    uint8_t data=0;
	// #ifndef AMEILIN
	// Ypos+=0x0028;
	// #endif
	lcd_cmd(0x2a);

    data = (Xpos&0xff00)>>8;
	lcd_data(&data,1); 

    data = Xpos&0x00ff;
    lcd_data(&data,1); 

	lcd_cmd(0x2b);

    data = (Ypos&0xff00)>>8;
	lcd_data(&data,1); 

    data = Ypos&0x00ff;
    lcd_data(&data,1); 

	lcd_cmd(0x2c);	
} 
/*************************************************
��������LCD_Set_Region
���ܣ�����lcd��ʾ�����ڴ�����д�������Զ�����
��ڲ�����xy�����յ�
����ֵ����
*************************************************/
//������ʾ����
void Lcd_SetRegion(uint16_t xStar, uint16_t yStar,uint16_t xEnd,uint16_t yEnd)
{
#if (LCD_SHOP_ST7789==0)  
	yStar+=0x0028;
	yEnd+=0x0028;
#endif
    uint8_t dataxrange[4]={(xStar&0xff00)>>8,   xStar&0x00ff,   (xEnd&0xff00)>>8,   xEnd&0x00ff};
    uint8_t datayrange[4]={(yStar&0xff00)>>8,   yStar&0x00ff,   (yEnd&0xff00)>>8,   yEnd&0x00ff};
    lcd_cmd(0x2a);
	lcd_data(dataxrange,4); 
	lcd_cmd(0x2b);
	lcd_data(datayrange,4); 
	lcd_cmd(0x2c);

    // uint8_t data[4]={(xStar&0xff00)>>8,xStar&0x00ff,(xEnd&0xff00)>>8,xEnd&0x00ff};
    // lcd_data(spi,0x2a,data,4);

    // uint8_t datay[4]={(yStar&0xff00)>>8,yStar&0x00ff,(yEnd&0xff00)>>8,yEnd&0x00ff};
    // lcd_data(spi,0x2b,data,4);
     


}


/*************************************************
��������LCD_DrawPoint
���ܣ���һ����
��ڲ�����xy�������ɫ���ￊ����ֵ����
*************************************************/
void Gui_DrawPoint(uint16_t x,uint16_t y,uint16_t Data)
{
#if (LCD_SHOP_ST7789==1)
      Data=~Data;
#endif
   
    uint8_t data[2]={Data>>8,Data&0xff};
	Lcd_SetXY(x,y);		 
    lcd_data(data,2); 
}    

/*************************************************
��������Lcd_Clear
���ܣ�ȫ����������
��ڲ����������ɫCOLOR
����ֵ����
*************************************************/
void Lcd_Clear(uint16_t Color)               
{	
	// Color= rgb2bgr(Color);
#if (LCD_SHOP_ST7789==1)
     Color= ~Color;
#endif
//	Lcd_WriteIndex(0x2a);
//	Lcd_WriteData(0x00); 
//    Lcd_WriteData(0x00); 
//	Lcd_WriteData(0x00); 
//    Lcd_WriteData(0xef); 
//	Lcd_WriteIndex(0x2b);
//	Lcd_WriteData(0x00); 
//    Lcd_WriteData(0x28); 
//	Lcd_WriteData(0x01); 
//    Lcd_WriteData(0x18); 
//	Lcd_WriteIndex(0x2c);
printf("clear\r\n");  
	uint8_t col[2]={Color>>8,Color&0xff};
	Lcd_SetRegion(0,0,TFT_HOR_RES-1,TFT_VER_RES-1);
    uint8_t data_tmp[TFT_HOR_RES*2]={};
    for(uint32_t i=0;i<TFT_HOR_RES*1;i++)
    {
        data_tmp[2*i] = col[0];
        data_tmp[2*i+1] = col[1];

    }

      
   for(uint32_t i=0;i<TFT_VER_RES;i++)
    {   
        lcd_data(&data_tmp[0],TFT_HOR_RES*2);

	}
}

 

void Lcd_DrawImage(uint16_t x,uint16_t y, uint16_t width, uint16_t height,const uint8_t * data)
{
	
	

}

uint16_t colorinvert(uint16_t color)
{
    uint8_t color_tmp[16];
    for(uint8_t i=0;i<8;i++)
    {
        color_tmp[i] =(color >>i) &0x01;
        color_tmp[i+8] =(color >>i) &0x01;
    }
    color=0;
    for(uint8_t i=0;i<8;i++)
    {
        color =(color_tmp[i]<<(8-i))|(color_tmp[i+8]<<(15-i))|color;
         
    }
    return color;

}



void st7789_init( )
{
    int cmd=0;	




    nrfx_spim_config_t spi_config = NRFX_SPIM_DEFAULT_CONFIG;
    spi_config.frequency      = NRF_SPIM_FREQ_32M;
    spi_config.ss_pin         = PIN_NUM_CS;
    spi_config.miso_pin       = NRFX_SPIM_PIN_NOT_USED;
    spi_config.mosi_pin       = PIN_NUM_MOSI;
    spi_config.sck_pin        = PIN_NUM_CLK;
    spi_config.dcx_pin        = PIN_NUM_DC;
    spi_config.use_hw_ss      = true;
    spi_config.ss_active_high = false; 
    APP_ERROR_CHECK(nrfx_spim_init(&spi, &spi_config, spim_event_handler, NULL));



    //Initialize non-SPI GPIOs
    #if defined(SPI_LINE_3)

    #else     
//    gpio_set_direction(PIN_NUM_DC, GPIO_MODE_OUTPUT);
    #endif
    nrf_gpio_cfg_output(PIN_NUM_RST);
    nrf_gpio_cfg_output(PIN_NUM_BCKL);
    nrf_gpio_pin_set(PIN_NUM_BCKL);
    nrf_delay_ms(1000); 
    //Reset the display
    nrf_gpio_pin_set(PIN_NUM_RST);
    nrf_delay_ms(1000); 
    nrf_gpio_pin_clear(PIN_NUM_RST);
    nrf_delay_ms(300); 
    nrf_gpio_pin_set(PIN_NUM_RST);
    nrf_delay_ms(300); 
     
    
    //Send all the commands
    while (st_init_cmds[cmd].databytes!=0xff) {
        lcd_cmd(   st_init_cmds[cmd].cmd);
        printf("LCD cmd.\n");
        lcd_data(st_init_cmds[cmd].data, st_init_cmds[cmd].databytes&0x1F);
        if (st_init_cmds[cmd].databytes&0x80) {
            nrf_delay_ms(100);
        }
        cmd++;
    }

    ///Enable backlight
    nrf_gpio_pin_clear(PIN_NUM_BCKL);
    // printf("LCD clear.\n");
    
Lcd_Clear(BLUE);  nrf_delay_ms(100);

Lcd_Clear(BLACK);  nrf_delay_ms(100); 
 for(uint8_t i=0;i<200;i++)
 {
     Gui_DrawPoint(i,i,GREEN);

 }
   
   nrf_delay_ms(100);
    
}

void st7789_flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t * color_map)
{
#if defined SPI_LINE_3    
    Lcd_SetRegion(x1,y1,x2,y2);
    uint32_t size = (x2 - x1 + 1) * (y2 - y1 + 1);
    /*Byte swapping is required*/
    uint32_t i;
    uint8_t * color_u8 = (uint8_t *) color_map;
    uint8_t color_tmp;
    for(i = 0; i < size * 2; i += 2) {
        color_tmp = color_u8[i + 1];
        color_u8[i + 1] = color_u8[i];//0xab;//
        color_u8[i] = color_tmp;//0x65;//
    }
    // printf("\r\nx1,y1=(%d,%d) x2,y2=(%d,%d)\r\n",x1,y1,x2,y2);
    // printf("  (%x,%x)  (%x,%x) ",color_u8[0],color_u8[1],color_u8[2],color_u8[3]);
    lcd_data(spi,(uint8_t *)color_map,size*2);
    lv_flush_ready();

#else
    #if (LCD_SHOP_ST7789==0)  
	y1+=0x0028;
	y2+=0x0028;
    #endif



    /*Byte swapping is required*/
    uint32_t size = (x2 - x1 + 1) * (y2 - y1 + 1);
    uint8_t *color_u8 = (uint8_t *)color_map;
	uint8_t color_tmp;
 
    
	for(uint32_t i = 0; i < size * 2; i += 2) {
        #if (LCD_SHOP_ST7789==1)
          color_tmp = color_u8[i + 0];
        color_u8[i + 0] = ~color_u8[i + 1];//0xaa;//
        color_u8[i + 1] = ~color_tmp;//0xaa;//
       #else
        color_tmp = color_u8[i + 0];
        color_u8[i + 0] = color_u8[i + 1];//0xaa;//
        color_u8[i + 1] = color_tmp;//0xaa;//
         #endif
	}
 
   Lcd_SetRegion(x1,y1,x2,y2); 
   lcd_data((uint8_t *)color_map,size*2);

    lv_flush_ready();
#endif
}



