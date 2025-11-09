#ifndef __DEV_BH1750_1_H_
#define __DEV_BH1750_1_H_

/**********************************
����ͷ�ļ�
**********************************/
#include "drv_bh1750_iic.h"
#include "hal_data.h"


#define BH1750_Wr 0x46	 //
#define BH1750_Rd 0x47  //

//extern float BH1750_temp;
/**********************************

**********************************/
void Multiple_read_BH1750(void);
void Single_Write_BH1750(uint8_t REG_Address);
void BH1750_Init(void);
float LIght_Intensity(void);

#endif






