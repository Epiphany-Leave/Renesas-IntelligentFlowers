#include "dev_MH.h"
#include "devices.h"
#include "drv_adc.h"
#include <libs.h>
#include <errno.h>
#include "hal_data.h"
#include <stdio.h>

static int MHDrvInit(struct ADCDev *ptDev);
static int MHDrvRead(struct ADCDev *ptDev, unsigned short *value, unsigned short num);

static struct ADCDev gMHDev = {
    .name = "ADC0",
    .channel = 10,
    .Init = MHDrvInit,
    .Read = MHDrvRead
};

struct ADCDev *MHGetDevice(void)
{
    return &gMHDev;
}

static int MHDrvInit(struct ADCDev *ptDev)
{
    if(NULL == ptDev)   return -1;
    if(10 == ptDev->channel)
    {
        fsp_err_t err = g_adc5.p_api->open(g_adc5.p_ctrl, g_adc5.p_cfg);
        assert(FSP_SUCCESS == err);
        
        err = g_adc5.p_api->scanCfg(g_adc5.p_ctrl, g_adc5.p_channel_cfg);
        assert(FSP_SUCCESS == err);
    }
	
	g_ioport.p_api->pinCfg(g_ioport.p_ctrl, BSP_IO_PORT_04_PIN_15 , (uint32_t)IOPORT_CFG_PORT_DIRECTION_INPUT);
    
    return 0;
}

static int MHDrvRead(struct ADCDev *ptDev, unsigned short *value, unsigned short num)
{
    if(NULL == ptDev || NULL == value || num <= 0)   return -1;
    if(10 == ptDev->channel)
    {
        for(uint16_t i=0; i<num; i++)
        {
            fsp_err_t err = g_adc5.p_api->scanStart(g_adc5.p_ctrl);
            assert(FSP_SUCCESS == err);
            err = g_adc5.p_api->read(g_adc5.p_ctrl, ADC_CHANNEL_10, &value[i]);
            assert(FSP_SUCCESS == err);
        }
    }
    
    return 0;
}



static int MH_PinRead(void)
{
	bsp_io_level_t level = 0;
	g_ioport.p_api->pinRead(g_ioport.p_ctrl, BSP_IO_PORT_04_PIN_15, &level);

	if(level)
	{
		R_BSP_SoftwareDelay(100,BSP_DELAY_UNITS_MILLISECONDS);
		return 1;
	}
	return 0;
}


float MHData(struct ADCDev *ptDev)
{
	if(!MH_PinRead())
	{
		printf("having Water\r\n");
		g_ioport.p_api->pinWrite(&g_ioport_ctrl, BSP_IO_PORT_02_PIN_11 , BSP_IO_LEVEL_LOW);
	}
	else
	{
		printf("No Water\r\n");
		g_ioport.p_api->pinWrite(&g_ioport_ctrl, BSP_IO_PORT_02_PIN_11 , BSP_IO_LEVEL_HIGH);
	}
	float raindata = (float)(GetADC_Average(ptDev, 20)*(3.3/4096));	//无水>1，有水<1
	raindata = (float)((1.5-raindata)/1.5*100);
	printf("MH: %.2f\r\n", raindata);
	
	mdelay(200);
	
	return raindata;
}