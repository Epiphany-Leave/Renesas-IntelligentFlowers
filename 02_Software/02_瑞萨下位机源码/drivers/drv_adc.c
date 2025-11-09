#include "drv_adc.h"
#include "hal_data.h"
#include <stdio.h>

static void ADCWaitConvCplt(void);
static int ADCDrvInit(struct ADCDev *ptDev);
static int ADCDrvRead(struct ADCDev *ptDev, unsigned short *value, unsigned short num);

static struct ADCDev gAdcDev = {
    .name = "ADC0",
    .channel = 5,
    .Init = ADCDrvInit,
    .Read = ADCDrvRead
};

struct ADCDev *ADCGetDevice(void)
{
    return &gAdcDev;
}

static int ADCDrvInit(struct ADCDev *ptDev)
{
    if(NULL == ptDev)   return -1;
    if(5 == ptDev->channel)
    {
        fsp_err_t err = g_adc5.p_api->open(g_adc5.p_ctrl, g_adc5.p_cfg);
        assert(FSP_SUCCESS == err);
        
        err = g_adc5.p_api->scanCfg(g_adc5.p_ctrl, g_adc5.p_channel_cfg);
        assert(FSP_SUCCESS == err);
    }
    
    return 0;
}

static int ADCDrvRead(struct ADCDev *ptDev, unsigned short *value, unsigned short num)
{
    if(NULL == ptDev || NULL == value || num <= 0)   return -1;
    if(5 == ptDev->channel)
    {
        for(uint16_t i=0; i<num; i++)
        {
            fsp_err_t err = g_adc5.p_api->scanStart(g_adc5.p_ctrl);
            assert(FSP_SUCCESS == err);
            ADCWaitConvCplt();
            err = g_adc5.p_api->read(g_adc5.p_ctrl, ADC_CHANNEL_5, &value[i]);
            assert(FSP_SUCCESS == err);
        }
    }
    
    return 0;
}

static void ADCWaitConvCplt(void)
{
    adc_status_t status;
    status.state = ADC_STATE_SCAN_IN_PROGRESS;
    
    while (ADC_STATE_SCAN_IN_PROGRESS == status.state)
    {
        g_adc5.p_api->scanStatusGet(g_adc5.p_ctrl, &status);
    }
}

//中位均值滤波
float GetADC_Average(struct ADCDev *ptDev, unsigned short count)
{
	uint32_t sum_val = 0;
	uint16_t buf[count];
	
	ptDev->Read(ptDev, buf, count);
	
	for(uint8_t i=0;i<(count-1);i++)
	{
		for(uint8_t j=i+1;j<count;j++)
		{
			if(buf[i] > buf[j])
			{
				uint16_t temp = buf[i];
				buf[i] = buf[j];
				buf[j] = temp;
			}
		}
	}
	
	for(uint8_t i=2;i<(count-2);i++)
	{
		sum_val += buf[i];
	}
	return (float)(sum_val/(count-4));
}
