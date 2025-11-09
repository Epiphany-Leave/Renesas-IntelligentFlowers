#ifndef _DEV_BH1750_H_
#define _DEV_BH1750_H_

#include "hal_data.h"

typedef struct BH1750Dev{
    char    *name;
    int     (*Init)(struct BH1750Dev *ptDev);
    int     (*Write)(struct BH1750Dev *ptDev, uint8_t* ucAddr, uint8_t* ucData);
    int     (*Read)(struct BH1750Dev* ptDev, uint8_t* rbuf, uint8_t dwSize);
}BH1750Dev, *PIBH1750Dev;

struct BH1750Dev* BH1750DevGet(void);

//BH1750的地址
#define BH1750_Wr			0x46	//写地址
#define BH1750_Rd			0x47	//读地址

//BH1750指令码
#define POWER_OFF			0x00
#define POWER_ON			0x01
#define MODULE_RESET		0x07
#define	CONTINUE_H_MODE		0x10
#define CONTINUE_H_MODE2	0x11
#define CONTINUE_L_MODE		0x13
#define ONE_TIME_H_MODE		0x20
#define ONE_TIME_H_MODE2	0x21
#define ONE_TIME_L_MODE		0x23

//测量模式
#define Measure_Mode			CONTINUE_H_MODE

//分辨率	光照强度（单位lx）=（High Byte  + Low Byte）/ 1.2 * 测量精度
#if ((Measure_Mode==CONTINUE_H_MODE2)|(Measure_Mode==ONE_TIME_H_MODE2))
#define Resolurtion		0.5
#elif ((Measure_Mode==CONTINUE_H_MODE)|(Measure_Mode==ONE_TIME_H_MODE))
#define Resolurtion		1
#elif ((Measure_Mode==CONTINUE_L_MODE)|(Measure_Mode==ONE_TIME_L_MODE))
#define Resolurtion		4
#endif


void BH1750_Init(void);			//未包含IIC初始化
float LIght_Intensity(void);	//读取光照强度的值
int BH1750_Read_Measure(void);
void BH1750_Power_ON(void);
void BH1750_Power_OFF(void);
void BH1750_RESET(void);

#endif /* DEV_BH750_H_ */

