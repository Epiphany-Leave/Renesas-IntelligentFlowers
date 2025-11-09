#include "app.h"
#include <devices.h>
#include <drivers.h>
#include <errno.h>
#include <libs.h>
#include "hal_data.h"
#include <stdio.h>


void BH1750Test(void)
{
	UartDevicesRegister();
    TimerDevicesRegister();
    IODevicesRegister();
	
//	BH1750_Init();
	BH1750_IIC_Init();
	
	while(1)
	{
		float Light = LIght_Intensity();
		printf("Light: %.2f\r\n", Light);
	
		mdelay(2000);
	}
}

float BH1750Data(void)
{
	float Light = LIght_Intensity();
	printf("Light: %.2f\r\n", Light);
	
	mdelay(200);
//	R_BSP_SoftwareDelay(1000, BSP_DELAY_UNITS_MILLISECONDS);
	
	return Light;
}