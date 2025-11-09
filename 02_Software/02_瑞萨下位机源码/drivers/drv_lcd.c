#include "drv_lcd.h"
#include "drv_disp.h"
#include <stdio.h>

static void LCDDrvWaitTX(void);
//static void LCDDrvWriteCS(CS eState);
//static void LCDDrvWriteDCX(DCX eState);
//static void LCDDrvWriteReset(Reset eState);
//static void LCDDrvWriteBlack(Black eState);
static void LCDDrvHWReset(void);
static void LCDDrvWriteReg(uint8_t reg);
static void LCDDrvWriteDat(uint8_t dat);
static void LCDDrvWriteBuf(uint8_t* buf, uint32_t size);

static void LCDDrvInit(struct DisplayDevice* ptDev);
static void LCDDrvSetDisplayOn(struct DisplayDevice* ptDev);
static void LCDDrvSetDisplayOff(struct DisplayDevice* ptDev);
static void LCDDrvSetDisplayWindow(struct DisplayDevice* ptDev, \
                                   unsigned short wXs, unsigned short wYs, \
                                   unsigned short wXe, unsigned short wYe);
static void LCDDrvFlush(struct DisplayDevice *ptDev);
static int  LCDDrvSetPixel(struct DisplayDevice *ptDev, \
                           unsigned short wX, unsigned short wY, \
                           unsigned short wColor);

static volatile bool gFlagWaitTX = 0;
static unsigned short gLcdFbuf[128*160];

static DisplayDevice gLcdDevice = {
        .name = "LCD",
        .FBBase = gLcdFbuf,
        .wXres = 128,
        .wYres = 160,
        .wBpp = 16,
        .dwSize = 128*160*16/8,
        .Init = LCDDrvInit,
        .DisplayON = LCDDrvSetDisplayOn,
        .DisplayOFF = LCDDrvSetDisplayOff,
        .SetDisplayWindow = LCDDrvSetDisplayWindow,
        .Flush = LCDDrvFlush,
        .SetPixel = LCDDrvSetPixel
};

struct DisplayDevice *LCDGetDevice(void)
{
    return &gLcdDevice;
}

////P114
//static void LCDDrvWriteCS(CS eState)
//{
//    g_ioport.p_api->pinWrite(g_ioport.p_ctrl,
//                             BSP_IO_PORT_01_PIN_14,
//                             (bsp_io_level_t)eState);
//}

////P609
//static void LCDDrvWriteDCX(DCX eState)
//{
//    g_ioport.p_api->pinWrite(g_ioport.p_ctrl,
//                             BSP_IO_PORT_06_PIN_09,
//                             (bsp_io_level_t)eState);
//}
////P612
//static void LCDDrvWriteReset(Reset eState)
//{
//    g_ioport.p_api->pinWrite(g_ioport.p_ctrl,
//                             BSP_IO_PORT_06_PIN_12,
//                             (bsp_io_level_t)eState);
//}
////P610
//static void LCDDrvWriteBlack(Black eState)
//{
//    g_ioport.p_api->pinWrite(g_ioport.p_ctrl,
//                             BSP_IO_PORT_06_PIN_10,
//                             (bsp_io_level_t)eState);
//}

void sci_spi8_callback(spi_callback_args_t *arg)
{
    /* 判断是否是发送完成触发的中断 */
    /* 如果是的话就将发送完成标志位置1 */
    if(SPI_EVENT_TRANSFER_COMPLETE == arg->event)
        gFlagWaitTX = true;
}

static void LCDDrvWaitTX(void)
{
    while(!gFlagWaitTX);
    gFlagWaitTX = false;
}

static void LCDDrvHWReset(void)
{
//    LCDDrvWriteReset(isReset);
	LCD_RES_H();
    R_BSP_SoftwareDelay(100, BSP_DELAY_UNITS_MILLISECONDS);
//    LCDDrvWriteReset(notReset);
	LCD_RES_L();
    R_BSP_SoftwareDelay(100, BSP_DELAY_UNITS_MILLISECONDS);
}

static void LCDDrvWriteReg(uint8_t reg)
{
//    LCDDrvWriteDCX(isCommand);
	LCD_DC_C();
    g_spi8.p_api->write(&g_spi8_ctrl, (uint8_t*)&reg, 1, SPI_BIT_WIDTH_8_BITS);
    LCDDrvWaitTX();
}

static void LCDDrvWriteDat(uint8_t dat)
{
//    LCDDrvWriteDCX(isData);
	LCD_DC_D();
    g_spi8.p_api->write(&g_spi8_ctrl, (uint8_t*)&dat, 1, SPI_BIT_WIDTH_8_BITS);
    LCDDrvWaitTX();
}

static void LCDDrvWriteBuf(uint8_t* buf, uint32_t size)
{
    LCDDrvWriteReg(0x3C);
    LCD_DC_D();
    
    unsigned char *pbuf = (unsigned int*)buf;
    while(size)
    {
        uint32_t length = 0;
        if(size<65536)
            length = (uint16_t)size;
        else
        {
            length = 65535;
        }
        fsp_err_t err = g_spi8.p_api->write(g_spi8.p_ctrl, pbuf, length, SPI_BIT_WIDTH_8_BITS);
        assert(FSP_SUCCESS==err);
        LCDDrvWaitTX();
        size = size - length;
        pbuf = pbuf + length;
    }
}

void LCDDrvInit(struct DisplayDevice* ptDev)
{
    if(NULL == ptDev->name)    return;
    /* 打开SPI设备完成初始化 */
    fsp_err_t err = g_spi8.p_api->open(&g_spi8_ctrl, &g_spi8_cfg);
    if(FSP_SUCCESS == err)
        printf("Success to open device:\tspi8\r\n");
    else
        printf("Failed to open device:\tspi8\r\n");
    
    /* 初始化屏幕设备 */
    LCDDrvHWReset(); //LCD 复位
	LCD_RES_H();
	R_BSP_SoftwareDelay(100, BSP_DELAY_UNITS_MILLISECONDS);
//    LCDDrvWriteBlack(isLight);//点亮背光
	LCD_CS_H();
	LCD_BL_H();
	R_BSP_SoftwareDelay(100, BSP_DELAY_UNITS_MILLISECONDS);
    
    LCDDrvWriteReg(0x11);
    LCDDrvWriteReg(0x20);
    LCDDrvWriteReg(0x36);
    LCDDrvWriteDat(0x40);
    LCDDrvWriteReg(0x3A);
    LCDDrvWriteDat(0x55);
    LCDDrvWriteReg(0x13);
    LCDDrvWriteReg(0x29);

//	LCDDrvWriteReg(0x11); //Exit Sleep
//	R_BSP_SoftwareDelay(120, BSP_DELAY_UNITS_MILLISECONDS);

//	LCDDrvWriteReg(0xB1);
//	LCDDrvWriteDat(0x05);//0a
//	LCDDrvWriteDat(0x3c);//14
//	LCDDrvWriteDat(0x3c);

//	LCDDrvWriteReg(0xB2);
//	LCDDrvWriteDat(0x05);
//	LCDDrvWriteDat(0x3c);
//	LCDDrvWriteDat(0x3c);

//	LCDDrvWriteReg(0xB3);
//	LCDDrvWriteDat(0x05);
//	LCDDrvWriteDat(0x3c);
//	LCDDrvWriteDat(0x3c);

//	LCDDrvWriteDat(0x05);
//	LCDDrvWriteDat(0x3c);
//	LCDDrvWriteDat(0x3c);

//	LCDDrvWriteReg(0xB4);
//	LCDDrvWriteDat(0x03);

//	LCDDrvWriteReg(0xC0); //Set VRH1[4:0] & VC[2:0] for VCI1 & GVDD
//	LCDDrvWriteDat(0x28);//
//	LCDDrvWriteDat(0x08);
//	LCDDrvWriteDat(0x04);//

//	LCDDrvWriteReg(0xC1); //Set BT[2:0] for AVDD & VCL & VGH & VGL
//	LCDDrvWriteDat(0xC0);

//	LCDDrvWriteReg(0xC2); //Set VMH[6:0] & VML[6:0] for VOMH & VCOML
//	LCDDrvWriteDat(0x0D);//54h
//	LCDDrvWriteDat(0x00);//33h

//	LCDDrvWriteReg(0xC3);
//	LCDDrvWriteDat(0x8D);
//	LCDDrvWriteDat(0x2A);

//	LCDDrvWriteReg(0xC4);
//	LCDDrvWriteDat(0x8D);
//	LCDDrvWriteDat(0xEE);

//	LCDDrvWriteReg(0xC5);
//	LCDDrvWriteDat(0x1A);

//	LCDDrvWriteReg(0x36); 	//MX,MY,RGB MODE
//	LCDDrvWriteDat(0x08);

//	LCDDrvWriteReg(0xe0);
//	LCDDrvWriteDat(0x04);//2c
//	LCDDrvWriteDat(0x22);
//	LCDDrvWriteDat(0x07);
//	LCDDrvWriteDat(0x0A);
//	LCDDrvWriteDat(0x2E);
//	LCDDrvWriteDat(0x30);
//	LCDDrvWriteDat(0x25);
//	LCDDrvWriteDat(0x2A);
//	LCDDrvWriteDat(0x28);
//	LCDDrvWriteDat(0x26);
//	LCDDrvWriteDat(0x2E);
//	LCDDrvWriteDat(0x3A);
//	LCDDrvWriteDat(0x00);
//	LCDDrvWriteDat(0x01);
//	LCDDrvWriteDat(0x03);
//	LCDDrvWriteDat(0x03);

//	LCDDrvWriteReg(0xe1);
//	LCDDrvWriteDat(0x04);
//	LCDDrvWriteDat(0x16);
//	LCDDrvWriteDat(0x06);
//	LCDDrvWriteDat(0x06);
//	LCDDrvWriteDat(0x0D);
//	LCDDrvWriteDat(0x2D);
//	LCDDrvWriteDat(0x26);
//	LCDDrvWriteDat(0x23);
//	LCDDrvWriteDat(0x27);
//	LCDDrvWriteDat(0x27);
//	LCDDrvWriteDat(0x25);
//	LCDDrvWriteDat(0x2D);
//	LCDDrvWriteDat(0x3B);
//	LCDDrvWriteDat(0x00);
//	LCDDrvWriteDat(0x01);
//	LCDDrvWriteDat(0x04);
//	LCDDrvWriteDat(0x13);

//	LCDDrvWriteReg(0x3A); 
//	LCDDrvWriteDat(0x05);
//	
//	LCDDrvWriteReg(0x29); // Display On
//	R_BSP_SoftwareDelay(20, BSP_DELAY_UNITS_MILLISECONDS);
}

void LCDDrvSetDisplayOn(struct DisplayDevice* ptDev)
{
    if(NULL == ptDev->name)    return;
    LCDDrvWriteReg(0x29);
}
void LCDDrvSetDisplayOff(struct DisplayDevice* ptDev)
{
    if(NULL == ptDev->name)    return;
    LCDDrvWriteReg(0x28);
}

void LCDDrvSetDisplayWindow(struct DisplayDevice* ptDev, \
                           unsigned short hwXs, unsigned short hwYs, \
                           unsigned short hwXe, unsigned short hwYe)
{
    if(NULL == ptDev->name)    return;
    /* 设置列地址 */
    LCDDrvWriteReg(0x2A);
    LCDDrvWriteDat((uint8_t)(hwXs>>8));       // 起始地址先高后低
    LCDDrvWriteDat((uint8_t)(0x00FF&hwXs));
    LCDDrvWriteDat((uint8_t)(hwXe>>8));        // 结束地址先高后低
    LCDDrvWriteDat((uint8_t)(0x00FF&hwXe));

    /* 设置行地址 */
    LCDDrvWriteReg(0x2B);
    LCDDrvWriteDat((uint8_t)(hwYs>>8));
    LCDDrvWriteDat((uint8_t)(0x00FF&hwYs));
    LCDDrvWriteDat((uint8_t)(hwYe>>8));
    LCDDrvWriteDat((uint8_t)(0x00FF&hwYe));
}

/* 把FBBase的数据刷到LCD的显存里 */
void LCDDrvFlush(struct DisplayDevice *ptDev)
{
    if(NULL == ptDev->name)    return;
    LCDDrvWriteBuf((uint8_t*)ptDev->FBBase, (uint32_t)ptDev->dwSize);
}

int LCDDrvSetPixel(struct DisplayDevice *ptDev, \
                   unsigned short wX, unsigned short wY, \
                   unsigned short wColor)
{
    if(NULL == ptDev->name)    return -1;
    if (wX >= ptDev->wXres || wY >= ptDev->wYres)
        return -1;

    unsigned short *buf = (unsigned short*)ptDev->FBBase;

    buf[wY * ptDev->wXres + wX] = (unsigned short)wColor;

    return 0;
}


//void LCDDrvInit(struct DisplayDevice* ptDev)
//{
//    if(NULL == ptDev->name)    return;
//    /* 打开SPI设备完成初始化 */
//    fsp_err_t err = g_spi8.p_api->open(&g_spi8_ctrl, &g_spi8_cfg);
//    if(FSP_SUCCESS == err)
//        printf("Success to open device:\tspi8\r\n");
//    else
//        printf("Failed to open device:\tspi8\r\n");
//    
//    /* 初始化屏幕设备 */
////    LCDDrvHWReset(); //LCD 复位
////    LCDDrvWriteCS(isSelect);
//	g_ioport.p_api->pinWrite(g_ioport.p_ctrl,
//                             BSP_IO_PORT_06_PIN_12,
//                             BSP_IO_LEVEL_LOW);
//	g_ioport.p_api->pinWrite(g_ioport.p_ctrl,
//                             BSP_IO_PORT_06_PIN_09,
//                             BSP_IO_LEVEL_LOW);
//	R_BSP_SoftwareDelay(100, BSP_DELAY_UNITS_MILLISECONDS);
//	g_ioport.p_api->pinWrite(g_ioport.p_ctrl,
//                             BSP_IO_PORT_06_PIN_12,
//                             BSP_IO_LEVEL_HIGH);
//	R_BSP_SoftwareDelay(100, BSP_DELAY_UNITS_MILLISECONDS);
//    LCDDrvWriteBlack(isLight);//点亮背光
//	R_BSP_SoftwareDelay(100, BSP_DELAY_UNITS_MILLISECONDS);
//    
//    LCDDrvWriteReg(0x11);
//	R_BSP_SoftwareDelay(120, BSP_DELAY_UNITS_MILLISECONDS);
//	
//    LCDDrvWriteReg(0xB1);
//	LCDDrvWriteDat(0x05);
//	LCDDrvWriteDat(0x3C); 
//	LCDDrvWriteDat(0x3C); 
//	LCDDrvWriteReg(0xB2); 
//	LCDDrvWriteDat(0x05);
//	LCDDrvWriteDat(0x3C); 
//	LCDDrvWriteDat(0x3C); 
//	LCDDrvWriteReg(0xB3); 
//	LCDDrvWriteDat(0x05); 
//	LCDDrvWriteDat(0x3C); 
//	LCDDrvWriteDat(0x3C); 
//	LCDDrvWriteDat(0x05); 
//	LCDDrvWriteDat(0x3C); 
//	LCDDrvWriteDat(0x3C);
//	
//	LCDDrvWriteReg(0xB4); //Dot inversion 
//	LCDDrvWriteDat(0x03);
//	//------------------- ST7735S Power Sequence ---------------------//
//	LCDDrvWriteReg(0xC0); 
//	LCDDrvWriteDat(0x28); 
//	LCDDrvWriteDat(0x08); 
//	LCDDrvWriteDat(0x04); 
//	LCDDrvWriteReg(0xC1); 
//	LCDDrvWriteDat(0XC0); 
//	LCDDrvWriteReg(0xC2); 
//	LCDDrvWriteDat(0x0D); 
//	LCDDrvWriteDat(0x00); 
//	LCDDrvWriteReg(0xC3); 
//	LCDDrvWriteDat(0x8D); 
//	LCDDrvWriteDat(0x2A); 
//	LCDDrvWriteReg(0xC4); 
//	LCDDrvWriteDat(0x8D); 
//	LCDDrvWriteDat(0xEE);
//	//------------------- End ST7735S Power Sequence ---------------------//
//	LCDDrvWriteReg(0xC5); //VCOM 
//	LCDDrvWriteDat(0x1A); 
//	LCDDrvWriteReg(0x36); //MX, MY, RGB mode 
//	if(USE_HORIZONTAL==0)LCDDrvWriteDat(0x00);
//	else if(USE_HORIZONTAL==1)LCDDrvWriteDat(0xC0);
//	else if(USE_HORIZONTAL==2)LCDDrvWriteDat(0x70);
//	else LCDDrvWriteDat(0xA0); 
//	//------------------- ST7735S Gamma Sequence ---------------------//
//	LCDDrvWriteReg(0xE0); 
//	LCDDrvWriteDat(0x04); 
//	LCDDrvWriteDat(0x22); 
//	LCDDrvWriteDat(0x07); 
//	LCDDrvWriteDat(0x0A); 
//	LCDDrvWriteDat(0x2E); 
//	LCDDrvWriteDat(0x30); 
//	LCDDrvWriteDat(0x25); 
//	LCDDrvWriteDat(0x2A); 
//	LCDDrvWriteDat(0x28); 
//	LCDDrvWriteDat(0x26); 
//	LCDDrvWriteDat(0x2E); 
//	LCDDrvWriteDat(0x3A); 
//	LCDDrvWriteDat(0x00); 
//	LCDDrvWriteDat(0x01); 
//	LCDDrvWriteDat(0x03); 
//	LCDDrvWriteDat(0x13); 
//	LCDDrvWriteReg(0xE1); 
//	LCDDrvWriteDat(0x04); 
//	LCDDrvWriteDat(0x16); 
//	LCDDrvWriteDat(0x06); 
//	LCDDrvWriteDat(0x0D); 
//	LCDDrvWriteDat(0x2D); 
//	LCDDrvWriteDat(0x26); 
//	LCDDrvWriteDat(0x23); 
//	LCDDrvWriteDat(0x27); 
//	LCDDrvWriteDat(0x27); 
//	LCDDrvWriteDat(0x25); 
//	LCDDrvWriteDat(0x2D); 
//	LCDDrvWriteDat(0x3B); 
//	LCDDrvWriteDat(0x00); 
//	LCDDrvWriteDat(0x01); 
//	LCDDrvWriteDat(0x04); 
//	LCDDrvWriteDat(0x13); 
//	//------------------- End ST7735S Gamma Sequence ---------------------//
//	LCDDrvWriteReg(0x3A); //65k mode 
//	LCDDrvWriteDat(0x05); 
//	LCDDrvWriteReg(0x29); //Display on
//}
