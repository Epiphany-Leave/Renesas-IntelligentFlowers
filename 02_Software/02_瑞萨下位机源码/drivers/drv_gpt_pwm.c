/*
 * drv_gpt_pwm.c
 *
 *  Created on: 2023年4月23日
 *      Author: slhuan
 */

#include "drv_gpt_pwm.h"
#include <stdio.h>
#include "app.h"

//extern Data_Value Data;

static timer_info_t gTimerInfo;
static uint32_t gPeriod;
static uint32_t gStep;
static volatile uint32_t gPulse = 0;
static volatile bool gDir = false;

static void PWM_SetDuty(uint8_t duty);

void GptPWMDrvInit(void)
{
    fsp_err_t err = g_timer5.p_api->open(g_timer5.p_ctrl, g_timer5.p_cfg);
    if(FSP_SUCCESS != err)
        printf("Function:%s\tLine:%d\r\n", __FUNCTION__, __LINE__);

    err = g_timer5.p_api->infoGet(g_timer5.p_ctrl, &gTimerInfo);
    if(FSP_SUCCESS != err)
        printf("Function:%s\tLine:%d\r\n", __FUNCTION__, __LINE__);
//    gPeriod = gTimerInfo.period_counts;
//    gStep = gPeriod/1500;
    
    err = g_timer5.p_api->start(g_timer5.p_ctrl);
    if(FSP_SUCCESS != err)
	{
        printf("Function:%s\tLine:%d\r\n", __FUNCTION__, __LINE__);
	}
	PWM_SetDuty(0);
	
//	err = g_timer8.p_api->open(g_timer8.p_ctrl, g_timer8.p_cfg);
//    if(FSP_SUCCESS != err)
//        printf("Function:%s\tLine:%d\r\n", __FUNCTION__, __LINE__);

//    err = g_timer8.p_api->infoGet(g_timer8.p_ctrl, &gTimerInfo);
//    if(FSP_SUCCESS != err)
//        printf("Function:%s\tLine:%d\r\n", __FUNCTION__, __LINE__);
//    gPeriod = gTimerInfo.period_counts;
//    gStep = gPeriod/1500;
//    
//    err = g_timer8.p_api->start(g_timer8.p_ctrl);
//    if(FSP_SUCCESS != err)
//        printf("Function:%s\tLine:%d\r\n", __FUNCTION__, __LINE__);
}

static void PWM_SetDuty(uint8_t duty)
{
	timer_info_t info;
	uint32_t current_period_counts;
	uint32_t duty_cycle_counts;
	
	if(duty > 100)
		duty = 100;
	
	R_GPT_InfoGet(&g_timer5_ctrl, &info);
	
	current_period_counts = info.period_counts;
	
	duty_cycle_counts = (uint32_t)(((uint64_t)current_period_counts * duty) / 100);
	
	R_GPT_DutyCycleSet(&g_timer5_ctrl, duty_cycle_counts, GPT_IO_PIN_GTIOCA);
	R_GPT_DutyCycleSet(&g_timer5_ctrl, duty_cycle_counts, GPT_IO_PIN_GTIOCB);
}

void PWM_LED(Data_Value Data)
{
	uint8_t gpulse = 0;
	
	if(Data.Light <= 100)
		gpulse = (uint8_t)Data.Light;
	else if(Data.Light >= 100)
		gpulse = 0;	
	
    PWM_SetDuty(gpulse);
}

/* time5_Callback function */
//void gpt_timer5_callback(timer_callback_args_t *p_args)
//{
////    if(p_args->event == TIMER_EVENT_CYCLE_END)
////    {
////        fsp_err_t err = g_timer5.p_api->dutyCycleSet(g_timer5.p_ctrl, 100, GPT_IO_PIN_GTIOCA);
////        if(FSP_SUCCESS != err)
////            printf("Function:%s\tLine:%d\r\n", __FUNCTION__, __LINE__);
////    }
//	
//	if(p_args->event == TIMER_EVENT_CYCLE_END)
//    {
//		
//        fsp_err_t err = g_timer5.p_api->dutyCycleSet(g_timer5.p_ctrl, gPulse, GPT_IO_PIN_GTIOCA);
//        if(FSP_SUCCESS != err)
//            printf("Function:%s\tLine:%d\r\n", __FUNCTION__, __LINE__);
//		err = g_timer5.p_api->dutyCycleSet(g_timer5.p_ctrl, gPulse, GPT_IO_PIN_GTIOCB);
//        if(FSP_SUCCESS != err)
//            printf("Function:%s\tLine:%d\r\n", __FUNCTION__, __LINE__);
//    }
//}

/* time8_Callback function */
void gpt_timer8_callback(timer_callback_args_t *p_args)
{
    if(p_args->event == TIMER_EVENT_CYCLE_END)
    {
        if(gDir)
            gPulse -= gStep;
        else
            gPulse += gStep;
        if(gPulse <= 0)
            gDir = false;
        else if(gPulse >= gPeriod)
            gDir = true;
        fsp_err_t err = g_timer8.p_api->dutyCycleSet(g_timer8.p_ctrl, gPulse, GPT_IO_PIN_GTIOCA | GPT_IO_PIN_GTIOCB);
        if(FSP_SUCCESS != err)
            printf("Function:%s\tLine:%d\r\n", __FUNCTION__, __LINE__);
    }
}


