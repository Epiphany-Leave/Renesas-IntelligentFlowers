/*
 * drv_gpio.h
 *
 *  Created on: 2023年4月13日
 *      Author: slhuan
 */

#ifndef __DEV_GPIO_H_
#define __DEV_GPIO_H_

#define IO_DEVICE_NAME_MAX_LENGTH   (16)

typedef struct IODev{
    char            *name;
    unsigned int    port;
    unsigned char   value;
    int             (*Init)(struct IODev *ptDev);
    int             (*Write)(struct IODev *ptDev, unsigned char level);
    int             (*Read)(struct IODev *ptDev);
    struct IODev *next;
}IODevice, *pIODevice;

void IODevicesRegister(void);
void IODeviceInsert(struct IODev *ptdev);
struct IODev *IODeviceFind(const char *name);
void IODeviceList(void);


#endif /* __DEV_GPIO_H_ */
