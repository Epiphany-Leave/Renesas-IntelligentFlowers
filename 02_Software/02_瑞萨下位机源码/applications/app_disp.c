/*
 * app_disp.c
 *
 *  Created on: 2023年3月31日
 *      Author: slhuan
 */
#include <drv_disp.h>
#include "app.h"
#include <stdio.h>
#include <devices.h>

void DispAppTest(void)
{
	UartDevicesRegister();
	IODevicesRegister();
	
	XTFT_Init_GPIO();//LCD引脚初始化
    LCD_Init();//LCD——SPI初始化
    ST7735_FillScreen(ST7735_BLUE);
	printf("OK!\r\n");
	
	while(1)
	{
		printf("STATE\r\n");
		ST7735_DrawString(0, 0, "RA6M5", Font_11x18, ST7735_GREEN, ST7735_BLACK);
        ST7735_DrawString(0, 16, "PrairieOne", Font_11x18, ST7735_GREEN, ST7735_BLACK);
//		ST7735_DrawImage(32,80,64,74,wuxie);
	}
}

void DispApp_Init(void)
{
	XTFT_Init_GPIO();//LCD引脚初始化
    LCD_Init();//LCD——SPI初始化
    ST7735_FillScreen(ST7735_BLACK);
	printf("TFT Init OK!\r\n");
}
