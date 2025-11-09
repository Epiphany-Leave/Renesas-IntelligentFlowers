#ifndef __DRV_BH1750_IIC_H_
#define __DRV_BH1750_IIC_H_


/**********************************

**********************************/
#include "hal_data.h"


#define IIC_SDA_PIN   				BSP_IO_PORT_05_PIN_11  //								
#define IIC_SCL_PIN    				BSP_IO_PORT_05_PIN_12			// 

#define  IIC_SDA_H   			R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_05_PIN_11, BSP_IO_LEVEL_HIGH)
#define  IIC_SDA_L   			R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_05_PIN_11, BSP_IO_LEVEL_LOW)

#define  IIC_SCL_H   			R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_05_PIN_12, BSP_IO_LEVEL_HIGH)
#define  IIC_SCL_L   			R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_05_PIN_12, BSP_IO_LEVEL_LOW)


//#define  IIC_SDA_Read   R_IOPORT_PinRead(&g_ioport_ctrl, BSP_IO_PORT_05_PIN_11)

/**********************************
��������
**********************************/
void BH1750_GPIO_Init(void);
void SDA_Pin_IN(void);
void SDA_Pin_Output(void);
	

void BH1750_IIC_Init(void);
void BH1750_IIC_start(void); 
void BH1750_IIC_stop(void);
uint8_t BH1750_IIC_Get_ack(void);
void BH1750_IIC_ACK(void);
void BH1750_IIC_NACK(void);
void BH1750_IIC_write_byte(uint8_t Data);
uint8_t BH1750_IIC_read_byte(void);

#endif


