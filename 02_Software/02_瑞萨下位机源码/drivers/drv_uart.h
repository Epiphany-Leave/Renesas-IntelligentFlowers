/*
 * drv_uart.h
 *
 *  Created on: 2023年4月13日
 *      Author: slhuan
 */
#ifndef __DRV_UART_H
#define __DRV_UART_H

#define UART_DEVICE_NAME_MAX_LENGTH     (16)
#define UART_TOTAL_CHANNELS             (10)
#define UART_RECEIVE_BUFFER_SIZE        (1024)

void UartDevicesCreate(void);

#endif /* __DRV_UART_H */
