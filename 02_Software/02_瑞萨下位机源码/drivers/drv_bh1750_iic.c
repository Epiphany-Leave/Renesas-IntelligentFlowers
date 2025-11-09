#include "drv_bh1750_iic.h"
#include <devices.h>
#include <drivers.h>
#include <errno.h>
#include <stdio.h>
#include "delay/delay.h"

#define IIC_D     6

static bsp_io_level_t IIC_SDA_Read(void);

void SDA_Pin_IN(void)  //SDA输入
{
	g_ioport.p_api->pinCfg(g_ioport.p_ctrl, BSP_IO_PORT_05_PIN_11 , (uint32_t)IOPORT_CFG_PORT_DIRECTION_INPUT);
}

void SDA_Pin_Output(void) //
{
	g_ioport.p_api->pinCfg(g_ioport.p_ctrl, BSP_IO_PORT_05_PIN_11 , (uint32_t)IOPORT_CFG_PORT_DIRECTION_OUTPUT);
}
void BH1750_IIC_Delay()
{
	udelay(IIC_D);
}
static bsp_io_level_t IIC_SDA_Read(void)
{
	bsp_io_level_t level;
	R_IOPORT_PinRead(&g_ioport_ctrl, BSP_IO_PORT_05_PIN_11, &level);
	return level;
}
/*
 *@brief 		IIC总线初始化函数
 *@param[in]	NULL
 */
void BH1750_IIC_Init(void)	
{
		IIC_SDA_H;//释放IIC总线的数据线。
		IIC_SCL_H;//释放IIC总线的时钟线。
		BH1750_IIC_Delay();
}
/****************************************************************
通讯起始信号：当 SCL 线是高电平时 SDA 线从高电平向低电平切换，表示通讯的开始；
*****************************************************************/

void BH1750_IIC_start(void)  
{

		IIC_SDA_H;//拉高
		IIC_SCL_H;
		BH1750_IIC_Delay();
		IIC_SDA_L;
		IIC_SCL_L;		//拉低时钟线，准备开始时钟
		BH1750_IIC_Delay();
}
/****************************************************************
通讯停止信号：当 SCL 是高电平时 SDA线由低电平向高电平切换，表示通讯的停止。
*****************************************************************/

void BH1750_IIC_stop(void) 
{

		IIC_SCL_H;
		IIC_SDA_L;
		BH1750_IIC_Delay();
		IIC_SDA_H;	//通讯停止
		IIC_SCL_L;
		BH1750_IIC_Delay();
}
/****************************************************************
//接收应答信号函数
*****************************************************************/

uint8_t BH1750_IIC_Get_ack(void)
{
		uint16_t CNT;
		SDA_Pin_IN();
		IIC_SCL_L;		//拉低时钟线。
		IIC_SCL_H;		//拉高时钟线。
		BH1750_IIC_Delay();
		while((IIC_SDA_Read())  && (CNT < 100) )
		{
				CNT++;
				if(CNT == 100)
				{
						return 0;
				}
		}

		IIC_SCL_L;		//拉低时钟线。
		BH1750_IIC_Delay();
		SDA_Pin_Output();
		return 1;

}
/****************************************************************
//发送应答信号函数
*****************************************************************/

void BH1750_IIC_ACK(void)	
{ 

		IIC_SDA_L;	//拉低数据线，应答
		IIC_SCL_H;	//产生第九个时钟信号。
		BH1750_IIC_Delay();
		IIC_SCL_L;	
		BH1750_IIC_Delay();
}
//非应答

void BH1750_IIC_NACK(void)
{ 

		IIC_SDA_H;	//拉高数据线，非应答
		IIC_SCL_H;	//产生第九个时钟信号。
		BH1750_IIC_Delay();
		IIC_SCL_L;	

}

/****************************************************************
//向IIC总线写入一个字节的数据
*****************************************************************/

void BH1750_IIC_write_byte(uint8_t Data)
{

		uint8_t i;
		for(i=0;i<8;i++)//八位数据
		{
				if((Data & 0x80) == 0x80)
				{
						IIC_SDA_H;
						
				}
				else
				{
						IIC_SDA_L;
						
				}
				
				IIC_SCL_H; //一个时钟信号送入数据
				BH1750_IIC_Delay();
				IIC_SCL_L;
				BH1750_IIC_Delay();
				Data = Data << 1;//数据左移一位，把次高位放在最高位,为写入次高位做准备
				
		}
		IIC_SDA_L;		//应答处理前拉低，跳过应答
		BH1750_IIC_Get_ack();
}
/****************************************************************
//从IIC总线读取一个字节的数据函数
*****************************************************************/

uint8_t BH1750_IIC_read_byte(void)
{

		uint8_t i;
		uint8_t Data = 0;       //定义一个缓冲寄存器。
		SDA_Pin_IN();
		IIC_SCL_L;	//先拉低时钟线
		IIC_SDA_H;	//再拉高数据线
		BH1750_IIC_Delay();
		for(i = 0;i < 8;i++)
		{
				Data = Data<<1;		//将缓冲字节的数据左移一位，准备读取数据。
				
				IIC_SCL_H;				//拉高时钟线，开始读取下一位数据
				BH1750_IIC_Delay();
				if(IIC_SDA_Read())		//如果数据线为高平电平。
				{
						Data = Data|0x01;	//则给缓冲字节的最低位写1。
				}
				IIC_SCL_L;				//拉低时钟线，一位读取完成
				BH1750_IIC_Delay();
		}
		SDA_Pin_Output();
		return Data;					//返回读取的一个字节数据。
}





