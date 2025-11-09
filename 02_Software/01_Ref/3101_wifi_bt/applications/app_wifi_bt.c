#include "app.h"
#include <devices.h>
#include <errno.h>
#include <libs.h>
#include "hal_data.h"
#include <stdio.h>


void WiFiBtAppTest(void)
{
    UartDevicesRegister();
    TimerDevicesRegister();
	
//	resetWiFiModule(); // 增加复位代码
    
    if(ESUCCESS != WiFiBtDevInit())
    {
        xprintf("Failed to Init WiFi Device!\r\n");
        return;
    }
	WiFiBtDevSetWorkType(STA);
	WiFiBtDevEnableDHCP();
    if(ESUCCESS != WiFiBtDevConnectWiFi("YSX", "Ysx200423"))
    {
        xprintf("Failed to connect WiFi\r\n");
        return;
    }
	WiFiBtDevGetLocalIP();
	printf("WiFi OK!\r\n");
    mdelay(3000);
    
/*--------- 网络连接 ---------*/
	ConnectInfo connect = {
        .Protocl = TCP,
        .Role = Client,
        .IP = "183.230.40.40",
        .RemotePort = 1811,
        .LocalPort = 6832
    };
	char *TCP_Info = "*632605#198018#flower*\r\n";
	
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
			
			TCP_Data("T25H88L123M2300S1.54P7.34\r\n");
			mdelay(1000);
			
//			unsigned char buffer[256];
//			int len = WiFiBtDevRead(connect, buffer, sizeof(buffer));
//			if (len > 0) {
//				// Call Lua script to parse and execute commands
//				printf("%s\r\n", buffer);
//				device_data_analyze((const char*)buffer);
//			}
//			mdelay(1000);
//			
        }
    }
}
