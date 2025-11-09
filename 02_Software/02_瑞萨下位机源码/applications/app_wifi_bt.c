#include "app.h"
#include <devices.h>
#include <errno.h>
#include <libs.h>
#include "hal_data.h"
#include <stdio.h>

extern uint8_t TCP_Flag;

void WiFiBtAppTest(void)
{
    UartDevicesRegister();
    TimerDevicesRegister();
    
    if(ESUCCESS != WiFiBtDevInit())
    {
        xprintf("Failed to Init WiFi Device!\r\n");
        return;
    }
	WiFiBtDevSetWorkType(STA);
	WiFiBtDevEnableDHCP();
    if(ESUCCESS != WiFiBtDevConnectWiFi("xyx", "123456789"))
    {
        xprintf("Failed to connect WiFi\r\n");
        return;
    }
	WiFiBtDevGetLocalIP();
	printf("WiFi OK!\r\n");
    R_BSP_SoftwareDelay(1000, BSP_DELAY_UNITS_MILLISECONDS);
    
    ConnectInfo connect = {
        .Protocl = TCP,
        .Role = Client,
        .IP = "183.230.40.40",
        .RemotePort = 1811,
        .LocalPort = 2156
    };
	char *TCP_Info = "*632605#123456#test3*\r\n";
	
	
    int ret1 = WiFiBtDevConnect(&connect);
	int ret2 = TCP_Connect(connect, TCP_Info);
	R_BSP_SoftwareDelay(200, BSP_DELAY_UNITS_MILLISECONDS);
	
    while(1)
    {
        if(ret1 != ESUCCESS)
        {
            xprintf("Failed to connect net!\r\n");
            ret1 = WiFiBtDevConnect(&connect);
			R_BSP_SoftwareDelay(2000, BSP_DELAY_UNITS_MILLISECONDS);
        }
		else if(ret2 != ESUCCESS)
		{
			xprintf("Failed to ENTM!\r\n");
			ret2 = TCP_Connect(connect, TCP_Info);
			R_BSP_SoftwareDelay(2000, BSP_DELAY_UNITS_MILLISECONDS);
		}
        else
        {
            char str[64] = {0};
            int len = WiFiBtDevRead(connect, (unsigned char*)str, 64);
            if(len > 0)
                WiFiBtDevWrite(connect, (unsigned char*)str, (unsigned int)len);
			
			TCP_Data("T25H56L123M3000S1.54P2.34\r\n");
        }
    }
}

//---Run Code---//
void WiFiTCP_Init(int *ret1, int *ret2,ConnectInfo connect, char* TCP_Info)
{
	if(ESUCCESS != WiFiBtDevInit())
    {
        xprintf("Failed to Init WiFi Device!\r\n");
        return;
    }
	WiFiBtDevSetWorkType(STA);
	WiFiBtDevEnableDHCP();
    if(ESUCCESS != WiFiBtDevConnectWiFi("Y", "Ysx200423"))
    {
        xprintf("Failed to connect WiFi\r\n");
        return;
    }
	WiFiBtDevGetLocalIP();
	printf("WiFi OK!\r\n");
    R_BSP_SoftwareDelay(1000, BSP_DELAY_UNITS_MILLISECONDS);
	
//	ConnectInfo connect = {
//        .Protocl = TCP,
//        .Role = Client,
//        .IP = "183.230.40.40",
//        .RemotePort = 1811,
//        .LocalPort = 2050
//    };
//	char *TCP_Info = "*632605#1234567#project*\r\n";
	
	
    *ret1 = WiFiBtDevConnect(&connect);
	*ret2 = TCP_Connect(connect, TCP_Info);
	R_BSP_SoftwareDelay(200, BSP_DELAY_UNITS_MILLISECONDS);
}

//void TCP_ReConnect(int ret1, int ret2)
//{
//	if(ret1 != ESUCCESS)
//    {
//        xprintf("Failed to connect net!\r\n");
//		R_BSP_SoftwareDelay(2000, BSP_DELAY_UNITS_MILLISECONDS);
//    }
//	else if(ret2 != ESUCCESS)
//	{
//		xprintf("Failed to ENTM!\r\n");
//		R_BSP_SoftwareDelay(2000, BSP_DELAY_UNITS_MILLISECONDS);
//	}
//}

void TCP_CmdRet(int ret1, int ret2, int ret3)
{
	if(ret1 != ESUCCESS)
    {
        xprintf("Failed to connect net!\r\n");
		R_BSP_SoftwareDelay(2000, BSP_DELAY_UNITS_MILLISECONDS);
    }
	else if(ret2 != ESUCCESS)
	{
		xprintf("Failed to ENTM!\r\n");
		R_BSP_SoftwareDelay(2000, BSP_DELAY_UNITS_MILLISECONDS);
	}
	else if(ret3 != ESUCCESS)
	{
		xprintf("Failed Data to Server!\r\n");
		R_BSP_SoftwareDelay(2000, BSP_DELAY_UNITS_MILLISECONDS);
	}
}




