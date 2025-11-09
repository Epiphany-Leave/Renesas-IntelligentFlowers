/*
 * drv_gpio.h
 *
 *  Created on: 2023年4月13日
 *      Author: slhuan
 */

#ifndef __DRV_GPIO_H_
#define __DRV_GPIO_H_

#define DEBOUNCE_TIME	20

void IODevicesCreate(void);
struct IODev *IOGetDecvice(const char *name);

//中断执行函数
void KeyProcessJitter(void);
void KeyProcessMotor(void);
void KeyProcessWater(void);
void KeyProcessWind(void);

#endif /* __DRV_GPIO_H_ */
