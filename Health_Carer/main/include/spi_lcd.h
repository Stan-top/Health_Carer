#ifndef __SPI_LCD_H__
#define __SPI_LCD_H__

#include <string.h>
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "freertos/task.h"
#include "simple_gui.h"

/**
 * @brief LCD驱动IC型号定义。
 *        只能保留一个，其余的都注释，做备选
 */
#define CONFIG_LCD_TYPE_ST7789V 1 //LCD驱动固定，型号为ST7789V，CLK最大频率为80MHz

/**
 * @brief LCD屏幕像素分辨率定义。（需要用户手动配置）
 *        只能保留一个，其余的都注释，做备选
 */

#define LCD_PIXEL_SIZE_240_280 1 

/**
 * @brief LCD屏幕的安装方向定义（需要用户手动配置）
 *        驱动会根据用户配置自动更改LCD-IC的扫描方向，来适配屏幕的安装方向
 *       （注意：easyIO仅对STxxxx-IC支持横屏适配，且需要用户自行到  simple_gui.c 的 LCD_SetCursor\LCD_SetWindows 修改偏移基地址）
 */

#define LCD_DIR horizontal //vertical、horizontal。竖屏 / 横屏
#define LCD_INVERT invert_en //invert_dis、invert_en。正着放 / 倒立
#define LCD_MIRROR mirror_dis //mirror_dis、mirror_en。不开启左右镜像显示 / 开启镜像显示（可用于镜面反射及棱镜的镜像显示，可使画面左右翻转。参照分光棱镜）

/**
 * @brief LCD的GPIO定义。（CS尽量使用IOMUX默认的SPI，DC/RST/BLK随意）（CLK、MOSI、MISO在 spi_config.h 中定义）
 */
// ESP32默认的SPI3引脚。使用默认引脚可以使用IOMUX将GPIO翻转速率提高到80MHz，GPIO矩阵不建议超过26MHz

#define LCD_SPI3_DEF_PIN_NUM_CS0 5 // SPI3默认的CS0引脚

#define PIN_NUM_DC   22   // D/C线，0-命令，1-数据
#define PIN_NUM_RST  25   // RST复位引脚。-1为空置不使用。但LCD的RST引脚应上拉接高电平。
#define PIN_NUM_BCKL 21   //背光BLK引脚。不需要接


/* 定义每行的最大像素个数 */

#if LCD_PIXEL_SIZE_80_160
    #define LINE_PIXEL_MAX_SIZE 160
#elif LCD_PIXEL_SIZE_128_128
    #define LINE_PIXEL_MAX_SIZE 128
#elif LCD_PIXEL_SIZE_135_240
    #define LINE_PIXEL_MAX_SIZE 240
#elif LCD_PIXEL_SIZE_240_280
    #define LINE_PIXEL_MAX_SIZE 280
#elif LCD_PIXEL_SIZE_240_320
    #define LINE_PIXEL_MAX_SIZE 320
#elif LCD_PIXEL_SIZE_320_480
    #define LINE_PIXEL_MAX_SIZE 480
#endif


#define LCD_DEF_DMA_CHAN 2 //LCD默认使用的SPI-DMA通道
// 为了加快传输速度，每个SPI传输都会发送一堆线。 此定义指定了几行线。 更多意味着更多的内存使用，但设置/完成转帐的开销较小。 确保240可被此整除。
#define PARALLEL_LINES 16
#define LCD_DMA_MAX_SIZE (PARALLEL_LINES * LINE_PIXEL_MAX_SIZE *2 + 8)

// LCD与SPI关联的句柄，通过此来调用SPI总线上的LCD设备
extern spi_device_handle_t LCD_SPI;


/*
 LCD初始化需要的一堆命令/参数值。 它们存储在此结构中。
*/
typedef struct {
    uint8_t cmd;
    uint8_t data[16];
    uint8_t databytes; //No of data in data; bit 7 = delay after set; 0xFF = end of cmds.
} lcd_init_cmd_t;

/*
 LCD的驱动IC型号定义。
*/
typedef enum {
    LCD_TYPE_ST_7735 ,
    LCD_TYPE_ST_7735S,
    LCD_TYPE_ST_7789V = 1,
    LCD_TYPE_ILI_9341,
    LCD_TYPE_ILI_9488,
    LCD_TYPE_ILI_9481,
    LCD_TYPE_HX_8357C,
    LCD_TYPE_MAX,
} type_lcd_t;

/*
 下面是关于各型号LCD驱动IC的参数配置。
 每一行分别为 命令、值、值的字节数（0x80代表写入后延时、0xff表示终止传输）
 将数据放入DRAM。 默认情况下，常量数据被放置到DROM中，而DMA无法访问该数据。
*/

#ifdef CONFIG_LCD_TYPE_ST7789V
//st7789V
DRAM_ATTR static const lcd_init_cmd_t st_7789V_init_cmds[]={
    /* Memory Data Access Control, MX=MV=1, MY=ML=MH=0, RGB=0 */
    {0x36, {(1<<4)|(1<<5)|(1<<6)}, 1},
    /* Interface Pixel Format, 16bits/pixel for RGB/MCU interface */
    {0x3A, {0x05}, 1},
    /* Porch Setting */
    {0xB2, {0x0c, 0x0c, 0x00, 0x33, 0x33}, 5},
    /* Gate Control, Vgh=13.65V, Vgl=-10.43V */
    {0xB7, {0x35}, 1},
    /* VCOM Setting, VCOM=1.175V */
    {0xBB, {0x19}, 1},
    /* LCM Control, XOR: BGR, MX, MH */
    {0xC0, {0x2C}, 1},
    /* VDV and VRH Command Enable, enable=1 */
    //{0xC2, {0x01, 0xff}, 2},
    {0xC2, {0x01}, 1},
    /* VRH Set, Vap=4.4+... */
    {0xC3, {0x12}, 1},
    /* VDV Set, VDV=0 */
    {0xC4, {0x20}, 1},
    /* Frame Rate Control, 60Hz, inversion=0 */
    {0xC6, {0x0f}, 1},
    /* Power Control 1, AVDD=6.8V, AVCL=-4.8V, VDDS=2.3V */
    {0xD0, {0xA4, 0xA1}, 1},
    /* Positive Voltage Gamma Control */
    {0xE0, {0xD0, 0x04, 0x0D, 0x11, 0x13, 0x2B, 0x3F, 0x54, 0x4C, 0x18, 0x0D, 0x0B, 0x1F, 0x23}, 14},
    /* Negative Voltage Gamma Control */
    {0xE1, {0xD0, 0x04, 0x0C, 0x11, 0x13, 0x2C, 0x3F, 0x44, 0x51, 0x2F, 0x1F, 0x1F, 0x20, 0x23}, 14},
    /* Display Inversion On */
    {0x21, {0}, 0x80},
    /* Sleep Out */
    {0x11, {0}, 0x80},
    /* Display On */
    {0x29, {0}, 0x80},
    {0, {0}, 0xff}
};
#endif



/**
 * @brief  向LCD发送1个字节的命令（D/C线电平为0）
 *      - 使用spi_device_polling_transmit，它等待直到传输完成。
 *      - 发送时同时设置D/C线为0，传输命令
 *      - 例：lcd_cmd(LCD_SPI, 0x04);
 * 
 * @param  spi LCD与SPI关联的句柄，通过此来调用SPI总线上的LCD设备
 * @param  cmd 向LCD发送的1个字节命令内容
 * 
 * @return
 *     - none
 */
void lcd_cmd(spi_device_handle_t spi,const uint8_t cmd);

/**
 * @brief  向LCD发送长度为len个字节的数据（D/C线电平为1）
 *      - 发送时同时设置D/C线为1，传输数据
 *      - 例：lcd_data(LCD_SPI, dataBuf, 10);
 * 
 * @param  spi LCD与SPI关联的句柄，通过此来调用SPI总线上的LCD设备
 * @param  data 要发送数据的指针
 * @param  len 发送的字节数
 * 
 * @return
 *     - none
 */
void lcd_data(spi_device_handle_t spi,const uint8_t *data,int len);


/**
 * @brief  向LCD发送单点16Bit的像素数据，（根据驱动IC的不同，可能为2或3个字节，需要转换RGB565、RGB666）
 *      - ili9488\ili9481 这类IC，SPI总线仅能使用RGB666-18Bit/像素，分3字节传输。而不能使用16Bit/像素，分2字节传输。（0x3A寄存器）
 *      - 例：lcd_data16(LCD_SPI, RED);
 * 
 * @param  spi LCD与SPI关联的句柄，通过此来调用SPI总线上的LCD设备
 * @param  data 要发送的单点像素数据，uint16_t的RGB565
 * 
 * @return
 *     - none
 */

void lcd_16data(spi_device_handle_t spi,uint16_t data);

/**
 * @brief  以SPI方式驱动LCD初始化函数
 *      - 过程包括：关联 SPI总线及LCD设备、驱动IC的参数配置、点亮背光、设置LCD的安装方向、设置屏幕分辨率、扫描方向、初始化显示区域的大小
 *      - （注意：普通GPIO最大只能30MHz，而IOMUX默认的SPI，CLK最大可以设置到80MHz）
 *      - 例：spi_lcd_init(SPI2_HOST, 60*1000*1000, LCD_SPI2_DEF_PIN_NUM_CS0);
 * 
 * @param  host_id SPI端口号。SPI1_HOST / SPI2_HOST / SPI3_HOST
 * @param  clk_speed LCD设备的SPI速度（注意：普通GPIO最大只能30MHz，而IOMUX默认的SPI，CLK最大可以设置到80MHz）
 * @param  cs_io_num CS端口号，尽量使用IOMUX默认的IO
 * 
 * @return
 *     - none
 */
void spi_lcd_init(spi_host_device_t host_id, uint32_t clk_speed, gpio_num_t cs_io_num);

#endif