/*
 * drv_systick.h
 *
 *  Created on: 2023年3月28日
 *      Author: slhuan
 */

#ifndef HAL_SYSTICK_H_
#define HAL_SYSTICK_H_

#include "hal_data.h"

#define HAL_MAX_DELAY      0xFFFFFFU

void SystickTimerDevicesCreate(void);
uint32_t HAL_GetTick(void);

#endif /* HAL_SYSTICK_H_ */
