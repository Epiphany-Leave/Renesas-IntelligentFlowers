#include "dev_MQ.h"
#include "devices.h"
#include "drv_adc.h"
#include <libs.h>
#include <errno.h>
#include "hal_data.h"
#include <stdio.h>

static int MQDrvInit(struct ADCDev *ptDev);
static int MQDrvRead(struct ADCDev *ptDev, unsigned short *value, unsigned short num);

static struct ADCDev gMQDev = {
    .name = "ADC0",
    .channel = 12,
    .Init = MQDrvInit,
    .Read = MQDrvRead
};

struct ADCDev *MQGetDevice(void)
{
    return &gMQDev;
}

static int MQDrvInit(struct ADCDev *ptDev)
{
    if(NULL == ptDev)   return -1;
    if(12 == ptDev->channel)
    {
        fsp_err_t err = g_adc5.p_api->open(g_adc5.p_ctrl, g_adc5.p_cfg);
        assert(FSP_SUCCESS == err);
        
        err = g_adc5.p_api->scanCfg(g_adc5.p_ctrl, g_adc5.p_channel_cfg);
        assert(FSP_SUCCESS == err);
    }
    
    return 0;
}

static int MQDrvRead(struct ADCDev *ptDev, unsigned short *value, unsigned short num)
{
    if(NULL == ptDev || NULL == value || num <= 0)   return -1;
    if(12 == ptDev->channel)
    {
        for(uint16_t i=0; i<num; i++)
        {
            fsp_err_t err = g_adc5.p_api->scanStart(g_adc5.p_ctrl);
            assert(FSP_SUCCESS == err);
            err = g_adc5.p_api->read(g_adc5.p_ctrl, ADC_CHANNEL_12, &value[i]);
            assert(FSP_SUCCESS == err);
        }
    }
    
    return 0;
}


float MQData(struct ADCDev *ptDev)
{
	float MQvalue = (float)(GetADC_Average(ptDev, 20)*(3.3/4096));	//
	printf("MQ: %.2f\r\n", MQvalue);

	mdelay(200);
	
	return MQvalue;
}