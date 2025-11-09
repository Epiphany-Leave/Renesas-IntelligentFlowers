#include "app.h"
#include "drv_adc.h"
#include <devices.h>
#include <errno.h>
#include <libs.h>
#include "hal_data.h"
#include <stdio.h>



void ADCAppTest(void)
{   
//	bsp_io_level_t level = BSP_IO_LEVEL_LOW;
	
	UartDevicesRegister();
    TimerDevicesRegister();
    IODevicesRegister();	
	
	ADCDevTypeDef *ptMHDev = MHGetDevice();
    if(NULL == ptMHDev)
    {
        printf("Error. Not found ADC device!\r\n");
        return;
    }
    printf("Success to find ADC device: %s-%d\r\n", ptMHDev->name, ptMHDev->channel);
    ptMHDev->Init(ptMHDev);
	
	ADCDevTypeDef *ptMQDev = MQGetDevice();
    if(NULL == ptMQDev)
    {
        printf("Error. Not found ADC device!\r\n");
        return;
    }
    printf("Success to find ADC device: %s-%d\r\n", ptMQDev->name, ptMQDev->channel);
    ptMQDev->Init(ptMQDev);
	
	ADCDevTypeDef *ptPHDev = PHGetDevice();
    if(NULL == ptPHDev)
    {
        printf("Error. Not found ADC device!\r\n");
        return;
    }
    printf("Success to find ADC device: %s-%d\r\n", ptPHDev->name, ptPHDev->channel);
    ptPHDev->Init(ptPHDev);
	
    while(1)
    {
		MHData(ptMHDev);
		
		MQData(ptMQDev);
		
		PHData(ptPHDev);
		
		mdelay(200);
		
//		g_ioport.p_api->pinWrite(g_ioport.p_ctrl, BSP_IO_PORT_04_PIN_00, level);
//		g_ioport.p_api->pinWrite(g_ioport.p_ctrl, BSP_IO_PORT_02_PIN_11, level);
//		R_BSP_SoftwareDelay(5000, BSP_DELAY_UNITS_MILLISECONDS);
//		
//		level = !level;
    }
}


