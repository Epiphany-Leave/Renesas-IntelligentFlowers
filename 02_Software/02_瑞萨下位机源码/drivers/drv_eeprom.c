/*
 * drv_sci_i2c_eeprom.c
 *
 *  Created on: 2023年3月31日
 *      Author: slhuan
 */

#include "drv_eeprom.h"
#include "drv_i2c.h"
#include <stdio.h>

static void I2C4WaitTxCplt(void);
static void I2C4WaitRxCplt(void);

static int  EEPROMDrvInit(struct I2CDev* ptDev);
static int  EEPROMDrvWriteByte(struct I2CDev* ptDev, \
                               unsigned char ucAddr, \
                               unsigned char const ucData);
static int  EEPROMDrvWritePage(struct I2CDev* ptDev, \
                               unsigned char ucAddr, \
                               unsigned char const* wbuf, \
                               unsigned int dwSize);
static int  EEPROMDrvWriteBuff(struct I2CDev* ptDev, \
                               unsigned char ucAddr, \
                               unsigned char const* wbuf, \
                               unsigned int dwSize);
static int  EEPROMDrvRead(struct I2CDev* ptDev, \
                          unsigned char ucAddr, \
                          unsigned char* rbuf, \
                          unsigned int dwSize);

static I2CDev gEepromDev = {
                             .name  = "EEPROM",
                             .Init  = EEPROMDrvInit,
                             .Write = EEPROMDrvWriteBuff,
                             .Read  = EEPROMDrvRead
};

static volatile bool gI2CTxCplt = false;
static volatile bool gI2CRxCplt = false;

struct I2CDev* EEPROMGetDevice(void)
{
    return &gEepromDev;
}


void sci_i2c4_master_callback(i2c_master_callback_args_t *p_args)
{
    if(I2C_MASTER_EVENT_RX_COMPLETE == p_args->event)
    {
        gI2CRxCplt = true;
    }
    else if(I2C_MASTER_EVENT_TX_COMPLETE == p_args->event)
    {
        gI2CTxCplt = true;
    }
}

static void I2C4WaitTxCplt(void)
{
    uint16_t wTimeout = 50;
    while(!gI2CTxCplt && wTimeout)
    {
        R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);
        wTimeout--;
    }
    gI2CTxCplt = false;
}

static void I2C4WaitRxCplt(void)
{
    uint16_t wTimeout = 50;
    while(!gI2CRxCplt && wTimeout)
    {
        R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);
        wTimeout--;
    }
    gI2CRxCplt = false;
}

static int EEPROMDrvInit(struct I2CDev* ptDev)
{
    if(NULL == ptDev->name)    return -1;
    fsp_err_t err = g_i2c4.p_api->open(g_i2c4.p_ctrl, g_i2c4.p_cfg);
    if(FSP_SUCCESS == err)
    {
        printf("Success to open device: i2c4!\r\n");
        return 0;
    }
    else
        printf("Failed to open device: i2c4!\r\n");

    return -1;
}

static int EEPROMDrvWriteByte(struct I2CDev* ptDev, unsigned char ucAddr, unsigned char const ucData)
{
    if(NULL == ptDev->name)    return -1;

    uint16_t wData = (uint16_t)((ucData<<8) | ucAddr);
    /* 发送一个字节的地址数据 */
    g_i2c4.p_api->write(g_i2c4.p_ctrl, (unsigned char*)&wData, (unsigned int)2, false);
    I2C4WaitTxCplt();

    return 0;
}

static int EEPROMDrvWritePage(struct I2CDev* ptDev, unsigned char ucAddr, unsigned char const* wbuf, unsigned int dwSize)
{
    if(NULL == ptDev->name)    return -1;

    unsigned char data[9] = {0};
    data[0] = ucAddr;
    for(unsigned char i=0; i<dwSize; i++)
    {
        data[i+1] = wbuf[i];
    }
    g_i2c4.p_api->write(g_i2c4.p_ctrl, (unsigned char*)data, (unsigned int)(dwSize+1), false);
    I2C4WaitTxCplt();
    R_BSP_SoftwareDelay((dwSize+1)*5, BSP_DELAY_UNITS_MILLISECONDS);
    return 0;
}

static int EEPROMDrvWriteBuff(struct I2CDev* ptDev, unsigned char ucAddr, unsigned char const* wbuf, unsigned int dwSize)
{
    if(NULL == ptDev->name)    return -1;

    if(dwSize == 1)
        return EEPROMDrvWriteByte(ptDev, ucAddr, *wbuf);

    /* 如果从当前地址开始写size字节会超过EEPROM的容量则返回错误值-1 */
    if((ucAddr+dwSize) >= 256)      return -1;

    unsigned char nAddr = ucAddr;
    /* 不管从何处开始，都将从起始地址开始把所在页写满 */
    /* 或者不会写满的情况下，从起始位置开始写size个字节 */
    unsigned char ucSize;  /* 当前写入的数据个数 */
    if( (ucAddr==0 || (ucAddr/EE_PAGE_SIZE)!=0) && (dwSize<=EE_PAGE_SIZE))
        ucSize = (unsigned char)dwSize;
    else
        ucSize = EE_PAGE_SIZE - (ucAddr % EE_PAGE_SIZE);
    EEPROMDrvWritePage(ptDev, nAddr, wbuf, ucSize);

    /* 写满起始位置开始的那一页之后，要将位置、数据地址和个数进行偏移计算 */
    nAddr += ucSize;
    wbuf += ucSize;
    dwSize -= ucSize;

    /* 如果写满起始地址所在页后还有数据，就进行下一步处理 */
    while(dwSize != 0)
    {
        /* 如果剩余数据个数大于一页的个数，就写满下一页 */
        /* 否则将剩余数据全部写到下一页 */
        if(dwSize <= EE_PAGE_SIZE)
            ucSize = (unsigned char)dwSize;
        else
            ucSize = EE_PAGE_SIZE;

        EEPROMDrvWritePage(ptDev, nAddr, wbuf, ucSize);
        /* 继续偏移 */
        nAddr += ucSize;
        wbuf += ucSize;
        dwSize -= ucSize;
    }

    return 0;
}

static int EEPROMDrvRead(struct I2CDev* ptDev, unsigned char ucAddr, unsigned char* rbuf, unsigned int dwSize)
{
    if(NULL == ptDev->name)    return -1;

    /* 发送一个字节的地址数据 */
    g_i2c4.p_api->write(g_i2c4.p_ctrl, (unsigned char*)&ucAddr, (unsigned int)1, true);
    I2C4WaitTxCplt();
    /* 读取该地址的一个字节数据 */
    R_BSP_SoftwareDelay(300, BSP_DELAY_UNITS_MICROSECONDS);
    g_i2c4.p_api->read(g_i2c4.p_ctrl, (unsigned char*)rbuf, (unsigned int)dwSize, false);
    I2C4WaitRxCplt();
    return 0;
}
