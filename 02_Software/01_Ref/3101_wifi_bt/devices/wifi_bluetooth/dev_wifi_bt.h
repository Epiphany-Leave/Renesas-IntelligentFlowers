#ifndef __DEV_WIFI_BT_H
#define __DEV_WIFI_BT_H

typedef enum{
    STA     = 0,
    SoftAP  = 2,
    APSTA   = 3
}WorkType;

typedef enum{
    TCP = 0,
    UDP = 1
}NetworkProtocol;

typedef enum{
    Client = 0,
    Server = 1
}LocalRole;

typedef struct{
    NetworkProtocol Protocl;
    LocalRole       Role;
    char            *IP;
    unsigned int    RemotePort;
    unsigned int    LocalPort;
    unsigned int    SocketPort;
}ConnectInfo;

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
void process_command(char cmd, char state);
void device_data_analyze(const char *data);
void resetWiFiModule(void);

#endif /* __DEV_WIFI_BT_H */
