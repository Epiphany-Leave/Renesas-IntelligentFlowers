/*
 * drv_i2c.h
 *
 *  Created on: 2023年4月13日
 *      Author: slhuan
 */

#ifndef DRV_I2C_H_
#define DRV_I2C_H_

#include "hal_data.h"

typedef struct I2CDev{
    char    *name;
    int     (*Init)(struct I2CDev *ptDev);
    int     (*Write)(struct I2CDev *ptDev, unsigned char ucAddr, unsigned char const *wBuf, unsigned int dwSize);
    int     (*Read)(struct I2CDev *ptDev, unsigned char ucAddr,  unsigned char *rBuf, unsigned int dwSize);
}I2CDev, *PI2CDev;

struct I2CDev* BH1750DevGet(void);

#endif /* DRV_I2C_H_ */
