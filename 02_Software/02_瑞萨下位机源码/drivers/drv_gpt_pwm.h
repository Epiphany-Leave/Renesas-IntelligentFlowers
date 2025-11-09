/*
 * drv_gpt_pwm.h
 *
 *  Created on: 2023年4月23日
 *      Author: slhuan
 */

#ifndef DRV_GPT_PWM_H_
#define DRV_GPT_PWM_H_

#include "hal_data.h"
#include "app.h"

void GptPWMDrvInit(void);
void PWM_LED(Data_Value Data);

#endif /* DRV_GPT_PWM_H_ */
