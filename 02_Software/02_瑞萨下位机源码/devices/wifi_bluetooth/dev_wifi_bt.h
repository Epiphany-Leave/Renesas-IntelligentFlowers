#ifndef __DEV_WIFI_BT_H
#define __DEV_WIFI_BT_H

//工作模式
typedef enum{
    STA     = 0,
    SoftAP  = 2,
    APSTA   = 3
}WorkType;

//协议
typedef enum{
    TCP = 0,
    UDP = 1
}NetworkProtocol;

//本机角色
typedef enum{
    Client = 0,
    Server = 1
}LocalRole;

//TCP参数
typedef struct{
    NetworkProtocol Protocl;	//协议
    LocalRole       Role;		//本机角色
    char            *IP;		//IP地址
    unsigned int    RemotePort;	//远端端口
    unsigned int    LocalPort;	//本机端口
    unsigned int    SocketPort;
}ConnectInfo;

enum
{
	ret1,
	ret2,
};
//MQTT参数
//typedef struct{
//	char            *IP;		//IP地址
//    unsigned int    RemotePort;	//远端端口
//	char			*Username;
//	char			*Secter;
//}ConnectMQTTInfo;

int WiFiBtDevInit(void);
void WiFiNetDataPreDeal(unsigned char data);
int WiFiBtDevReset(void);
int WiFiBtDevSetWorkType(WorkType type);
int WiFiBtDevSetRPTM(void);
int WiFiBtDevEnableDHCP(void);
int WiFiBtDevDisableDHCP(const char *ip, const char *netmask, const char *gateway);
int WiFiBtDevNetScan(void);
int WiFiBtDevConnectWiFi(const char *name, const char *password);
int WiFiBtDevDisconnectWiFi(void);
int WiFiBtDevGetLocalIP(void);
int WiFiBtDevConnect(ConnectInfo *info);
int WiFiBtDevDisconnect(ConnectInfo info);
int WiFiBtDevWrite(ConnectInfo info, unsigned char *buf, unsigned int length);
int WiFiBtDevRead(ConnectInfo info, unsigned char *buf, unsigned int length);

int TCP_Connect(ConnectInfo info, char *TCP_Info);
int TCP_Data(char *Data);

#endif /* __DEV_WIFI_BT_H */
