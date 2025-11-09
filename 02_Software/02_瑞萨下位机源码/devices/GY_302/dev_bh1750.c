/*
 * drv_touch.c
 *
 *  Created on: 2023年4月20日
 *      Author: slhuan
 */

#include "dev_bh1750.h"
#include <stdio.h>

static void I2C2WaitTxCplt(void);
static void I2C2WaitRxCplt(void);

static int  BH1750DrvInit(struct BH1750Dev* ptDev);
static int  BH1750DrvWriteByte(struct BH1750Dev* ptDev, \
                               uint8_t* ucAddr, \
                               uint8_t* ucData);
static int  BH1750DrvRead(struct BH1750Dev* ptDev, \
                          uint8_t* rbuf, \
                          uint8_t dwSize);
						  
static volatile bool gI2C1TxCplt = false;
static volatile bool gI2C1RxCplt = false;

static struct BH1750Dev gBH1750Dev = {
                                    .name = "BH1750",
                                    .Init = BH1750DrvInit,
									.Write = BH1750DrvWriteByte,
                                    .Read = BH1750DrvRead
};


struct BH1750Dev* BH1750GetDevice(void)
{
    return &gBH1750Dev;
}

//BH1750回调函数
void g_i2c1_callback(i2c_master_callback_args_t *p_args)
{
    if(I2C_MASTER_EVENT_RX_COMPLETE == p_args->event)
    {
        gI2C1RxCplt = true;
    }
    else if(I2C_MASTER_EVENT_TX_COMPLETE == p_args->event)
    {
        gI2C1TxCplt = true;
    }
	else
	{
		gI2C1RxCplt = gI2C1TxCplt = false;
	}
}

/*I2C2总线的接收标志函数*/
static void I2C2WaitTxCplt(void)
{
    uint16_t wTimeout = 50;
    while(!gI2C1TxCplt && wTimeout)
    {
        R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);
        wTimeout--;
    }
    gI2C1TxCplt = false;
}

static void I2C2WaitRxCplt(void)
{
    uint16_t wTimeout = 50;
    while(!gI2C1RxCplt && wTimeout)
    {
        R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);
        wTimeout--;
    }
    gI2C1RxCplt = false;
}

//BH1750初始化
static int BH1750DrvInit(struct BH1750Dev* ptDev)
{
    if(NULL == ptDev->name)    return -1;
    fsp_err_t err = g_i2c_master1.p_api->open(g_i2c_master1.p_ctrl, g_i2c_master1.p_cfg);
    if(FSP_SUCCESS == err)
    {
        printf("Success to open device: I2C1!\r\n");
        return 0;
    }
    else
        printf("Failed to open device: I2C1!\r\n");

    return -1;
}

/*
	BH1750写一个字节
	param：ucAddr-读写信号
	param：ucData-数据
	返回值：成功 - 0；失败 - 非0
*/
static int BH1750DrvWriteByte(struct BH1750Dev* ptDev, uint8_t* ucAddr, uint8_t* ucData)
{
    if(NULL == ptDev->name)    return -1;
//	uint16_t wData = (uint16_t)((ucData<<8) | ucAddr);
    /* 发送一个字节的地址数据 */
	g_i2c_master1.p_api->write(g_i2c_master1.p_ctrl, ucAddr, (unsigned int)1, false);
    g_i2c_master1.p_api->write(g_i2c_master1.p_ctrl, ucData, (unsigned int)1, false);
    I2C2WaitTxCplt();

    return 0;
}

/*
	BH1750读Size个字节
	返回值：成功 - 0；失败 - 非0
*/
static int BH1750DrvRead(struct BH1750Dev* ptDev, uint8_t* rbuf, uint8_t dwSize)
{
    if(NULL == ptDev->name)    return -1;

    /* 读取该地址的n个字节数据 */
    g_i2c_master1.p_api->read(g_i2c_master1.p_ctrl, rbuf, dwSize, false);
    I2C2WaitRxCplt();
    return 0;
}

int BH1750_Read_Measure(void)
{
	uint8_t data[2] = {0};
	int receive_data = 0;
	
	gBH1750Dev.Write(&gBH1750Dev, (uint8_t*)BH1750_Wr, (uint8_t*)BH1750_Rd);
	gBH1750Dev.Read(&gBH1750Dev, (uint8_t*)data, sizeof(data));	
	
	receive_data = ((data[0]<<8) + data[1]);
	
	return receive_data;
}

//BH1750s上电
void BH1750_Power_ON(void)
{
	gBH1750Dev.Write(&gBH1750Dev, (uint8_t*)BH1750_Wr, (uint8_t*)POWER_ON);
}

//BH1750s断电
void BH1750_Power_OFF(void)
{
	gBH1750Dev.Write(&gBH1750Dev, (uint8_t*)BH1750_Wr, (uint8_t*)POWER_OFF);
}

//BH1750复位	仅在上电时有效
void BH1750_RESET(void)
{
	gBH1750Dev.Write(&gBH1750Dev, (uint8_t*)BH1750_Wr, (uint8_t*)MODULE_RESET);
}

//BH1750初始化
void BH1750_Init(void)
{
	gBH1750Dev.Init(&gBH1750Dev);		/* I2C初始化 */
	
	BH1750_Power_ON();	//BH1750s上电
	//BH1750_RESET();			//BH1750复位
//	ptBH1750Dev->Write(ptBH1750Dev, (uint8_t*)BH1750_Wr, (uint8_t*)Measure_Mode);
//	R_BSP_SoftwareDelay(180, BSP_DELAY_UNITS_MILLISECONDS);
}

//获取光照强度
float LIght_Intensity(void)
{
	BH1750_Power_ON();
	gBH1750Dev.Write(&gBH1750Dev, (uint8_t*)BH1750_Wr, (uint8_t*)Measure_Mode);
	
	R_BSP_SoftwareDelay(180, BSP_DELAY_UNITS_MILLISECONDS);
	
	return (float)(BH1750_Read_Measure()/1.2*Resolurtion);
}

