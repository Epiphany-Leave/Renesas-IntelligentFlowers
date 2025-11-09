#include "dev_PH.h"
#include "devices.h"
#include "drv_adc.h"
#include <libs.h>
#include <errno.h>
#include "hal_data.h"
#include <stdio.h>

static int PHDrvInit(struct ADCDev *ptDev);
static int PHDrvRead(struct ADCDev *ptDev, unsigned short *value, unsigned short num);

static struct ADCDev gPHDev = {
    .name = "ADC0",
    .channel = 4,
    .Init = PHDrvInit,
    .Read = PHDrvRead
};

struct ADCDev *PHGetDevice(void)
{
    return &gPHDev;
}

static int PHDrvInit(struct ADCDev *ptDev)
{
    if(NULL == ptDev)   return -1;
    if(4 == ptDev->channel)
    {
        fsp_err_t err = g_adc5.p_api->open(g_adc5.p_ctrl, g_adc5.p_cfg);
        assert(FSP_SUCCESS == err);
        
        err = g_adc5.p_api->scanCfg(g_adc5.p_ctrl, g_adc5.p_channel_cfg);
        assert(FSP_SUCCESS == err);
    }
    
    return 0;
}

static int PHDrvRead(struct ADCDev *ptDev, unsigned short *value, unsigned short num)
{
    if(NULL == ptDev || NULL == value || num <= 0)   return -1;
    if(4 == ptDev->channel)
    {
        for(uint16_t i=0; i<num; i++)
        {
            fsp_err_t err = g_adc5.p_api->scanStart(g_adc5.p_ctrl);
            assert(FSP_SUCCESS == err);
            err = g_adc5.p_api->read(g_adc5.p_ctrl, ADC_CHANNEL_4, &value[i]);
            assert(FSP_SUCCESS == err);
        }
    }
    
    return 0;
}


float PHData(struct ADCDev *ptDev)
{
	float phValue = (float)(GetADC_Average(ptDev, 20)*5.0/1024/6);
	phValue = (float)(3.5*phValue + 7);
	printf("PH: %.2f\r\n", phValue);
	
	mdelay(200);
	return phValue;
}