/*
 * app.h
 *
 *  Created on: 2023年3月31日
 *      Author: slhuan
 */
#ifndef _APP_H_
#define _APP_H_

#include <devices.h>
//测试函数
void WiFiBtAppTest(void);
void DeviceTest(void);
void BH1750Test(void);
void ADCAppTest(void);
void DeviceTest(void);
void DispAppTest(void);
void BreathingLights(void);

//数据采集与处理
float BH1750Data(void);
void DeviceData(struct DHT11Dev *pDevice);
float MHData(struct ADCDev *ptDev);	
float MQData(struct ADCDev *ptDev);	
float PHData(struct ADCDev *ptDev);

//网络连接函数
void WiFiTCP_Init(int *ret1, int *ret2,ConnectInfo connect, char* TCP_Info);
//void TCP_ReConnect(int ret1, int ret2);
void TCP_CmdRet(int ret1, int ret2, int ret3);

//TFT屏
void DispApp_Init(void);

typedef struct Data{
	float MHvalue;
	float MQvalue;
	float PHvalue;
	float Light;
	unsigned int temp;
	unsigned int humi;
}Data_Value;

#endif /* _APP_H_ */
