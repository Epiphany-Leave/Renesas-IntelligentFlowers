#include "delay.h"
#include <devices.h>
#include <hal_data.h>

void delay(unsigned long secs)
{
    struct TimerDev *pTimer = TimerDeviceFind("Systick");
    pTimer->Timeout(pTimer, secs*1000);
}

void mdelay(unsigned long msecs)
{
    struct TimerDev *pTimer = TimerDeviceFind("Systick");
    pTimer->Timeout(pTimer, msecs);
}

void udelay(unsigned long usecs)
{
    R_BSP_SoftwareDelay(usecs, BSP_DELAY_UNITS_MICROSECONDS);
}