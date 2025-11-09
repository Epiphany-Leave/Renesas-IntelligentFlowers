/*
 * dev_uart.h
 *
 *  Created on: 2023?4?13?
 *      Author: slhuan
 */
#ifndef __DEV_UART_H
#define __DEV_UART_H

typedef struct UartDev{
    char *name;
    unsigned char channel;
    int (*Init)(struct UartDev *ptdev);
    int (*Write)(struct UartDev *ptdev, unsigned char * const buf, unsigned int length);
    int (*Read)(struct UartDev *ptdev, unsigned char *buf, unsigned int length);
    struct UartDev *next;
}UartDevice;

void UartDevicesRegister(void);
void UartDeviceInsert(struct UartDev *ptdev);
struct UartDev *UartDeviceFind(const char *name);
void UartDeviceList(void);

#endif /* __DEV_UART_H */
