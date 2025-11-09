#ifndef _ST7735_H_
#define _ST7735_H_

#include "hal_data.h"

#define ROW  160
#define COL  128

#define TP_TEST
#define EXTEND_TEST

#define CHAR_FONT_W8_H16

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


#ifdef  CHAR_FONT_W8_H16	
	#define  FONT_W  8
	#define  FONT_H  16
#endif
#ifdef  CHAR_FONT_W16_H21
	#define  FONT_W  16
	#define  FONT_H  21
#endif
//------------------------------------------------------
#define PIC_WIDTH    160
#define PIC_HEIGHT   160

#define GRAY75 0x39E7 
#define GRAY50 0x7BEF	
#define GRAY25 0xADB5	

#define X_min 0x0043
#define X_max 0x07AE
#define Y_min 0x00A1
#define Y_max 0x0759

//---------------------------------------------------------------------
void WriteComm(unsigned int i);
void WriteData(unsigned int i);
void WriteDispData(unsigned char DataH,unsigned char DataL);
//void LCD_Init(void);
void BlockWrite(unsigned int Xstart,unsigned int Xend,unsigned int Ystart,unsigned int Yend) ;
void DispBand(void);
void DispColor(unsigned int color);
void DispFrame(void);
void DispPic(unsigned int const *picture);
void DispPicFromSD(unsigned char PicNum);

#define ST7735_SPI_PORT hspi2
//#define USE_SPI_DMA

#define ST7735_1_8_DEFAULT_ORIENTATION	// AliExpress/eBay 1.8" display, default orientation

#define ST7735_MADCTL_MY  0x80
#define ST7735_MADCTL_MX  0x40
#define ST7735_MADCTL_MV  0x20
#define ST7735_MADCTL_RGB 0x00
#define ST7735_MADCTL_BGR 0x08

// AliExpress/eBay 1.8" display, default orientation
#ifdef ST7735_1_8_DEFAULT_ORIENTATION
#define ST7735_IS_160X128		1
#define ST7735_WIDTH  			128
#define ST7735_HEIGHT 			160
#define ST7735_XSTART 			2	//0
#define ST7735_YSTART 			1	//0
#define ST7735_DATA_ROTATION 	(ST7735_MADCTL_MX | ST7735_MADCTL_MY)
#endif //ST7735_1_8_DEFAULT_ORIENTATION

// WaveShare ST7735S-based 1.8" display, default orientation
#ifdef ST7735S_1_8_DEFAULT_ORIENTATION
#define ST7735_IS_160X128 		1
#define ST7735_WIDTH  			128
#define ST7735_HEIGHT 			160
#define ST7735_XSTART 			2
#define ST7735_YSTART 			1
#define	ST7735_DATA_ROTATION	(ST7735_MADCTL_MX | ST7735_MADCTL_MY | ST7735_MADCTL_RGB)
#endif //ST7735S_1_8_DEFAULT_ORIENTATION

// 1.44" display, default orientation
#ifdef ST7735_1_44_DEFAULT_ORIENTATION
#define ST7735_IS_128X128 		1
#define ST7735_WIDTH  			128
#define ST7735_HEIGHT 			128
#define ST7735_XSTART 			2
#define ST7735_YSTART 			3
//#define ST7735_VALUE_ROTATION	0
#define ST7735_DATA_ROTATION 	(ST7735_MADCTL_MX | ST7735_MADCTL_MY | ST7735_MADCTL_BGR)
#endif //ST7735_1_44_DEFAULT_ORIENTATION

// mini 160x80 display (it's unlikely you want the default orientation)
#ifdef ST7735_MINI_DEFAULT_ORIENTATION
#define ST7735_IS_160X80 		1
#define ST7735_XSTART 			26
#define ST7735_YSTART 			1
#define ST7735_WIDTH  			80
#define ST7735_HEIGHT 			160
//#define ST7735_VALUE_ROTATION	0
#define ST7735_DATA_ROTATION 	(ST7735_MADCTL_MX | ST7735_MADCTL_MY | ST7735_MADCTL_BGR)
#endif //ST7735_MINI_DEFAULT_ORIENTATION
/****************************/

//指令
#define ST7735_NOP     0x00
#define ST7735_SWRESET 0x01
#define ST7735_RDDID   0x04
#define ST7735_RDDST   0x09

#define ST7735_SLPIN   0x10
#define ST7735_SLPOUT  0x11
#define ST7735_PTLON   0x12
#define ST7735_NORON   0x13

#define ST7735_INVOFF  0x20
#define ST7735_INVON   0x21
#define ST7735_DISPOFF 0x28
#define ST7735_DISPON  0x29

#define ST7735_CASET   0x2A
#define ST7735_RASET   0x2B
#define ST7735_RAMWR   0x2C

#define ST7735_RAMRD   0x2E

#define ST7735_PTLAR   0x30
#define ST7735_COLMOD  0x3A
#define ST7735_MADCTL  0x36

#define ST7735_FRMCTR1 0xB1
#define ST7735_FRMCTR2 0xB2
#define ST7735_FRMCTR3 0xB3
#define ST7735_INVCTR  0xB4
#define ST7735_DISSET5 0xB6

#define ST7735_PWCTR1  0xC0
#define ST7735_PWCTR2  0xC1
#define ST7735_PWCTR3  0xC2
#define ST7735_PWCTR4  0xC3
#define ST7735_PWCTR5  0xC4
#define ST7735_VMCTR1  0xC5

#define ST7735_RDID1   0xDA
#define ST7735_RDID2   0xDB
#define ST7735_RDID3   0xDC
#define ST7735_RDID4   0xDD

#define ST7735_PWCTR6  0xFC

#define ST7735_GMCTRP1 0xE0
#define ST7735_GMCTRN1 0xE1

// Color definitions
#define	ST7735_BLACK   0x0000
#define	ST7735_RED    0x001F
#define	ST7735_BLUE     0xF800
#define	ST7735_GREEN   0x07E0
#define ST7735_CYAN    0x07FF
#define ST7735_MAGENTA 0xF81F
#define ST7735_YELLOW  0xFFE0
#define ST7735_WHITE   0xFFFF
#define ST7735_LGRAY   0XC618 //浅灰色(PANNEL),窗体背景色
#define ST7735_LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)

//OLED Color definitions
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 
#define BRRED 			 0XFC07 
#define GRAY  			 0X8430 
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色

//#define bool uint8_t
	
typedef struct {
    const uint8_t width;
    uint8_t height;
    const uint16_t *data;
} FontDef;


extern FontDef Font_7x10;
extern FontDef Font_11x18;
extern FontDef Font_16x26;	



//--------------SPI---------------------
#define	OLED_MOSI_SET()  	R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_10_PIN_00, BSP_IO_LEVEL_HIGH)//MOSI--PA00
#define	OLED_MOSI_CLR()  	R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_10_PIN_00, BSP_IO_LEVEL_LOW)//MOSI
  
#define	OLED_CLK_SET()  	R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_10_PIN_01, BSP_IO_LEVEL_HIGH)//CLK--PA01
#define	OLED_CLK_CLR()  	R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_10_PIN_01, BSP_IO_LEVEL_LOW)//CLK
//-----------------OLED----------------
#define OLED_CS_Set()  		R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_14, BSP_IO_LEVEL_HIGH)//CS--P114
#define OLED_CS_Clr()  		R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_14, BSP_IO_LEVEL_LOW)//CS

#define OLED_DC_Set()  		R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_06_PIN_09, BSP_IO_LEVEL_HIGH)//DC--P609
#define OLED_DC_Clr()  		R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_06_PIN_09, BSP_IO_LEVEL_LOW)//DC

#define OLED_RST_Set()  	R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_06_PIN_12, BSP_IO_LEVEL_HIGH)//RST--P612
#define OLED_RST_Clr()  	R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_06_PIN_12, BSP_IO_LEVEL_LOW)//RST

#define OLED_BL_Set()  		R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_06_PIN_10, BSP_IO_LEVEL_HIGH)//BL--P610
#define OLED_BL_Clr()  		R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_06_PIN_10, BSP_IO_LEVEL_LOW)//BL


#define TFT_BL_H() 		R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_06_PIN_10, BSP_IO_LEVEL_HIGH)	//BL
#define TFT_BL_L() 		R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_06_PIN_10, BSP_IO_LEVEL_LOW)	//BL

#define TFT_CS_H() 		R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_14, BSP_IO_LEVEL_HIGH)	//CS
#define TFT_CS_L() 		R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_14, BSP_IO_LEVEL_LOW)//CS

#define TFT_DC_D() 		R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_06_PIN_09, BSP_IO_LEVEL_HIGH)	//DC
#define TFT_DC_C() 		R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_06_PIN_09, BSP_IO_LEVEL_LOW)//DC

#define TFT_RES_H() 	R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_06_PIN_12, BSP_IO_LEVEL_HIGH)	//RST
#define TFT_RES_L() 	R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_06_PIN_12, BSP_IO_LEVEL_LOW)//RST

#define TFT_SCL_H() 	R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_10_PIN_01, BSP_IO_LEVEL_HIGH)	//CLK
#define TFT_SCL_L() 	R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_10_PIN_01, BSP_IO_LEVEL_LOW)//CLK

#define TFT_MOSI_H() 	R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_10_PIN_00, BSP_IO_LEVEL_HIGH)	//MOSI
#define TFT_MOSI_L() 	R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_10_PIN_00, BSP_IO_LEVEL_LOW)//MOSI

void ST7735_GPIO_Init(void);
void ST7735_Spi2_Init(void);

void ST7735_Backlight_On(void);
void ST7735_Backlight_Off(void);
void XYZST7735_Init(void);
void ST7735_DrawPixel(uint16_t x, uint16_t y, uint16_t color);
void ST7735_DrawString(uint16_t x, uint16_t y, const char* str, FontDef font, uint16_t color, uint16_t bgcolor);
void LCD_ShowIntNum(uint16_t x, uint16_t y,uint16_t number,FontDef font, uint16_t color, uint16_t bgcolor);
void ST7735_FillRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void ST7735_FillScreen(uint16_t color);
void ST7735_DrawImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t* data);
void ST7735_InvertColors(bool invert);
void ST7735_DrawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
void ST7735_DrawCircleHelper( int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color);
void ST7735_FillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
void ST7735_FillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color);
void ST7735_DrawEllipse(int16_t x0, int16_t y0, int16_t rx, int16_t ry, uint16_t color);
void ST7735_FillEllipse(int16_t x0, int16_t y0, int16_t rx, int16_t ry, uint16_t color);
void ST7735_DrawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void ST7735_DrawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color);
void ST7735_FillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color);
void ST7735_DrawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
void ST7735_FillTriangle( int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
void ST7735_DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void ST7735_DrawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
void ST7735_DrawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
void ST7735_SetRotation(uint8_t m);
uint8_t ST7735_GetRotation(void);
int16_t ST7735_GetHeight(void);
int16_t ST7735_GetWidth(void);

void demoTFT(void);

void XTFT_Init_GPIO(void);
void LCD_Init(void);
	
#endif
