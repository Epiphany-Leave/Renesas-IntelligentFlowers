#include "dev_bh1750_1.h"
#include "delay/delay.h"
#include "devices.h"
#include <libs.h>
#include <errno.h>
#include <stdio.h>

uint8_t BH1750_Buf[8];   
int     dis_data;                       //变量
//*********************************************************
//
//连续读出BH1750内部数据
//
//*********************************************************
void Multiple_read_BH1750(void)
{  
	uint8_t i;	
    BH1750_IIC_start();                          //起始信号
    BH1750_IIC_write_byte(BH1750_Rd);         //发送设备地址+读信号
	
		for (i=0; i<3; i++)                      //连续读取2个地址数据，存储中BUF
    {
        BH1750_Buf[i] = BH1750_IIC_read_byte();          //BUF[0]存储0x32地址中的数据
        if (i == 3)
        {

          BH1750_IIC_NACK();                //最后一个数据需要回NOACK
        }
        else
        {		
          BH1750_IIC_ACK();                //回应ACK
       }
   }

    BH1750_IIC_stop();                          //停止信号
    mdelay(5);
}

//*********************************

void Single_Write_BH1750(uint8_t REG_Address)
{
    BH1750_IIC_start();                  //起始信号
    BH1750_IIC_write_byte(BH1750_Wr);   //发送设备地址+写信号

    BH1750_IIC_write_byte(REG_Address);    //内部寄存器地址，
  //  BH1750_SendByte(REG_data);       //内部寄存器数据，
    BH1750_IIC_stop();                   //发送停止信号
}

//初始化

void BH1750_Init(void)       
{
   Single_Write_BH1750(0x01); 
}
//读取数据并处理返回

float LIght_Intensity(void)
{
    Single_Write_BH1750(0x01);   // power on
    Single_Write_BH1750(0x10);   // H- resolution mode

     mdelay(180);              //延时180ms

    Multiple_read_BH1750();       //连续读出数据，存储在BUF中

    dis_data=BH1750_Buf[0];
    dis_data=(dis_data<<8)+BH1750_Buf[1];//合成数据，即光照数据
    
    return (float)(dis_data/1.2);
}











