/*
 * drv_gpt.h
 *
 *  Created on: 2023年4月23日
 *      Author: slhuan
 */

#ifndef __DRV_GPT_H
#define __DRV_GPT_H

#define GPT_DEVICE_NAME_MAX_LENGTH     (16)
#define GPT_TOTAL_CHANNELS             (10)
#define GPT_RECEIVE_BUFFER_SIZE        (1024)

void GPTTimerDevicesCreate(void);

#endif /* __DRV_GPT_H */
