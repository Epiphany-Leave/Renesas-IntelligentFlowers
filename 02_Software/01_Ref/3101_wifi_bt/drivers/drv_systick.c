/*
 * drv_systick.c
 *
 *  Created on: 2023年3月28日
 *      Author: slhuan
 */

#include "drv_systick.h"
#include <devices.h>
#include <errno.h>
#include <libs.h>
#include "hal_data.h"

static int SystickInit(struct TimerDev *ptdev);
static int HAL_Delay(struct TimerDev *ptdev, uint32_t timeout);
static uint32_t HAL_GetTick(void);

static __IO uint32_t dwTick;
static struct TimerDev gSystickDevice = {
    .name = "Systick",
    .channel = 0xFF,
    .status = 0,
    .Init = SystickInit,
    .Start = NULL,
    .Stop = NULL,
    .Read = NULL,
    .Timeout = HAL_Delay,
    .next = NULL
};

void SystickTimerDevicesCreate(void)
{
    TimerDeviceInsert(&gSystickDevice);
    gSystickDevice.Init(&gSystickDevice);
}

static int SystickInit(struct TimerDev *ptdev)
{
    if(NULL==ptdev) return -EINVAL;
	/* 获取处理器时钟uwSysclk */
    uint32_t uwSysclk = R_BSP_SourceClockHzGet(FSP_PRIV_CLOCK_PLL);
    /* 技术周期为uwSysclk/1000 */
    if(SysTick_Config(uwSysclk/1000) != 0)
    {
        return -EIO;
    }
    ptdev->status = 1;
    /* Return function status */
    return ESUCCESS;
}

void SysTick_Handler(void);
void SysTick_Handler(void)
{
    dwTick += 1;
}

static int HAL_Delay(struct TimerDev *ptdev, uint32_t timeout)
{
    if(NULL == ptdev)       return -EINVAL;
    if(0 == ptdev->status)  return -EIO;
    uint32_t dwStart = dwTick;
    uint32_t dwWait = timeout;

    /* Add a freq to guarantee minimum wait */
    if (dwWait < HAL_MAX_DELAY)
    {
        dwWait += (uint32_t)(1);
    }

    while((dwTick - dwStart) < dwWait)
    {
    }
    return ESUCCESS;
}

static uint32_t HAL_GetTick(void)
{
	return dwTick;
}
