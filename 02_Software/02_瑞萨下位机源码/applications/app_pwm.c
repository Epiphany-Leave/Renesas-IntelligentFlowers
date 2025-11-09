/*
 * app_pwm.c
 *
 *  Created on: 2023年4月23日
 *      Author: slhuan
 */

#include "app.h"
#include <devices.h>
#include <errno.h>
#include <libs.h>
#include "hal_data.h"
#include <stdio.h>
#include <drivers.h>

void BreathingLights(void)
{
    UartDevicesRegister();
    TimerDevicesRegister();
    IODevicesRegister();
	
    GptPWMDrvInit();
	
	while(1)
	{
		KeyProcessJitter();
	}
}

