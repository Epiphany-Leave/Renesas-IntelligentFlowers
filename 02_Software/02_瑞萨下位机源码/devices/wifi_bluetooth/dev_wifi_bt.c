#include "dev_wifi_bt.h"
#include <devices.h>
#include <errno.h>
#include <libs.h>
#include "hal_data.h"
#include <stdio.h>

static UartDevice *pWiFiBtDev = NULL;
static int WiFiBtDevCmdRet(const char *ret, unsigned int timeout);
static struct RingBuffer *gNetBuffer = NULL;

//W800初始化
int WiFiBtDevInit(void)
{
    pWiFiBtDev = UartDeviceFind("WiFi");
    if(NULL==pWiFiBtDev) return -ENXIO;
    if(pWiFiBtDev->Init(pWiFiBtDev) != ESUCCESS)    return -EIO;
    
    int ret = -EIO;
    
    ret = WiFiBtDevReset();
    if(ESUCCESS != ret) return ret;
    mdelay(2000);
    ret = WiFiBtDevSetWorkType(STA);
    if(ESUCCESS != ret) return ret;
    ret = WiFiBtDevEnableDHCP();
    if(ESUCCESS != ret) return ret;
    gNetBuffer = RingBufferNew(1024);
    assert(NULL != gNetBuffer);
    
    return ret;
}

static unsigned char gDetectBuff[256] = {0};
static unsigned int gSocketPort = 0;
static unsigned int gDataLength = 0;
static unsigned char backcnt = 0;

/**/
void WiFiNetDataPreDeal(unsigned char data)
{
    static unsigned int index = 0;
    static unsigned int status = 0;
    unsigned char *pbuf = gDetectBuff;
    unsigned int i = index;
    pbuf[i] = data;
    index = (index + 1)%256;
    switch(status)
    {
        case 0: // Head Status
        {
            if(strstr((char*)pbuf, "+SKTRPT="))
            {
                status = 1;
                index = 0;
            }
            break;
        }
        case 1: // Socket Num Status;
        {
            if(pbuf[i] == ',')
            {
                for(unsigned int m=0; m<i; m++)
                {
                    gSocketPort = gSocketPort * 10 + pbuf[m] - '0';
                }
                status = 2;
                index = 0;
            }
            else if(i >= 9)
            {
                status = 0;
                index = 0;
            }
            break;
        }
        case 2: // Data Length Status
        {
            if(pbuf[i] == ',')
            {
                for(unsigned int m=0; m<i; m++)
                {
                    gDataLength = gDataLength * 10 + pbuf[m] - '0';
                }
                status = 3;
                index = 0;
            }
            else if(i >= 9)
            {
                status = 0;
                index = 0;
            }
            break;
        }
        case 3: // Find double \\r\\n
        {
            if(pbuf[i]=='\n')
            {
                backcnt++;
            }
            if(backcnt == 2)
            {
                status = 4;
                index = 0;
            }
            break;
        }
        case 4: // Net Data Save in RingBuffer
        {
            if(index == gDataLength)
            {
                if(NULL == gNetBuffer)  return;
                gNetBuffer->Write(gNetBuffer, pbuf, gDataLength);
                status = 0;
                backcnt = 0;
                index = 0;
                gDataLength = 0;
                gSocketPort = 0;
                memset(gDetectBuff, 0, 256);
            }
            break;
        }
        default:break;
    }
}

/* AT+指令返回判定 */
static int WiFiBtDevCmdRet(const char *ret, unsigned int timeout)
{
    unsigned char i = 0;
    unsigned char buf[256] = {0};
    while(timeout)
    {
        if(strstr((char*)buf, ret))
        {
            xprintf("%s\r\n", buf);
            return ESUCCESS;
        }
        else if(strstr((char*)buf, "+ERR"))
        {
            xprintf("%s\r\n", buf);
            return -EIO;
        }
        else if(pWiFiBtDev->Read(pWiFiBtDev, &buf[i], 1)==1)
        {
            i = (i+1)%256;
//			i++;
        }
        R_BSP_SoftwareDelay(500, BSP_DELAY_UNITS_MICROSECONDS);
        timeout--;
    }
    xprintf("%s\r\n", buf);
    return -EIO;
}

int WiFiBtDevReset(void)
{
    char str[64];
    sprintf(str, "AT+Z\r\n");
    if(pWiFiBtDev->Write(pWiFiBtDev, (unsigned char*)str, strlen(str)) != ESUCCESS)
        return -EIO;
    return WiFiBtDevCmdRet("+OK", 10000);
}


//设置工作模式
int WiFiBtDevSetWorkType(WorkType type)
{
    char str[64];
    sprintf(str, "AT+WPRT=%d\r\n", type);
    if(pWiFiBtDev->Write(pWiFiBtDev, (unsigned char*)str, strlen(str)) != ESUCCESS)
        return -EIO;
    return WiFiBtDevCmdRet("+OK", 100);
}

int WiFiBtDevSetRPTM(void)
{
    char str[64];
    sprintf(str, "AT+SKRPTM=1\r\n");
    if(pWiFiBtDev->Write(pWiFiBtDev, (unsigned char*)str, strlen(str)) != ESUCCESS)
        return -EIO;
    return WiFiBtDevCmdRet("+OK", 100);
}

//使能DHCP自动分配
int WiFiBtDevEnableDHCP(void)
{
    int ret = -EIO;
    char *str = "AT+NIP=0\r\n";
    ret = pWiFiBtDev->Write(pWiFiBtDev, (unsigned char*)str, strlen(str));
    if(ESUCCESS != ret) return ret;
    ret = WiFiBtDevCmdRet("+OK", 100);
    return ret;
}

//失能DHCP自动分配，配置新的DHCP状态
int WiFiBtDevDisableDHCP(const char *ip, const char *netmask, const char *gateway)
{
    int ret = -EIO;
    char str[64];
    sprintf(str, "AT+NIP=1,%s,%s,%s\r\n", ip, netmask, gateway);
    ret = pWiFiBtDev->Write(pWiFiBtDev, (unsigned char*)str, strlen(str));
    if(ESUCCESS != ret) return ret;
    ret = WiFiBtDevCmdRet("+OK", 100);
    return ret;
}

/**
*扫描网络情况
*/
int WiFiBtDevNetScan(void)
{
    const char *str="AT+WSCAN\r\n";
    if(pWiFiBtDev->Write(pWiFiBtDev, (unsigned char*)str, strlen(str)) != ESUCCESS)
        return -EIO;
    unsigned short timeout = 5000;
    while(timeout)
    {
        unsigned char c = 0;
        if(pWiFiBtDev->Read(pWiFiBtDev, &c, 1)==1)
        {
			printf("%c", c);
        }
        mdelay(1);
        timeout--;
    }
    return ESUCCESS;
}

/**
*连接热点
*name：	   热点名称
*password：热点密码
*/
int WiFiBtDevConnectWiFi(const char *name, const char *password)
{
    int ret = -EIO;

    char ssid[32];
    sprintf(ssid, "AT+SSID=%s\r\n", name);
    xprintf("%s\r\n", ssid);
    ret = pWiFiBtDev->Write(pWiFiBtDev, (unsigned char*)ssid, strlen(ssid));
    if(ESUCCESS != ret) return ret;
    ret = WiFiBtDevCmdRet("+OK", 1000);
    if(ESUCCESS != ret) return ret;
    
    char key[32] ;
    sprintf(key, "AT+KEY=1,0,%s\r\n", password);
    xprintf("%s\r\n", key);
    ret = pWiFiBtDev->Write(pWiFiBtDev, (unsigned char*)key, strlen(key));
    if(ESUCCESS != ret) return ret;
    ret = WiFiBtDevCmdRet("+OK", 1000);
    if(ESUCCESS != ret) return ret;
    
    char join[32] = "AT+WJOIN\r\n";
    xprintf("%s\r\n", join);
    ret = pWiFiBtDev->Write(pWiFiBtDev, (unsigned char*)join, strlen(join));
    if(ESUCCESS != ret) return ret;
	R_BSP_SoftwareDelay(100, BSP_DELAY_UNITS_MILLISECONDS);
    ret = WiFiBtDevCmdRet("+OK", 10000);
	int ret1 = WiFiBtDevCmdRet("\r\n\r\n", 3000);
    if(ret==ESUCCESS && ret1)   R_BSP_SoftwareDelay(1000, BSP_DELAY_UNITS_MILLISECONDS);
    return ret;
}

/**
*断开热点
*/
int WiFiBtDevDisconnectWiFi(void)
{
    int ret = -EIO;
    char *leavw = "AT+WLEAV\r\n";
    ret = pWiFiBtDev->Write(pWiFiBtDev, (unsigned char*)leavw, strlen(leavw));
    if(ESUCCESS != ret) return ret;
    ret = WiFiBtDevCmdRet("+OKret", 100);
    return ret;
}

/**
*获取本地IP
*/
int WiFiBtDevGetLocalIP(void)
{
    int ret = -EIO;
    char lkstt[32] = "AT+LKSTT\r\n";
    ret = pWiFiBtDev->Write(pWiFiBtDev, (unsigned char*)lkstt, strlen(lkstt));
    if(ESUCCESS != ret) return ret;
    unsigned short timeout = 1000;
    while(timeout)
    {
        unsigned char c = 0;
        if(pWiFiBtDev->Read(pWiFiBtDev, &c, 1)==1)
        {
			printf("%c", c);
        }
        mdelay(1);
        timeout--;
    }
    return ESUCCESS;
}

/**
*网络通信
*info：通信所需参数
*/
int WiFiBtDevConnect(ConnectInfo *info)
{
    int ret = -EIO;
    char skct[128];
    sprintf(skct, "AT+SKCT=%d,%d,%s,%d,%d\r\n", \
                   info->Protocl, \
                   info->Role, \
                   info->IP, \
                   info->RemotePort, \
                   info->LocalPort
                   );
    ret = pWiFiBtDev->Write(pWiFiBtDev, (unsigned char*)skct, strlen(skct));
    if(ESUCCESS != ret) return ret;
    mdelay(100);
    /* 获取连接成功后的socket号 */
    unsigned short timeout = 1000;
    unsigned char i = 0, status = 0;
    unsigned char buf[64] = {0};
    while(timeout!=0 && status != 2)
    {
        if(pWiFiBtDev->Read(pWiFiBtDev, &buf[i], 1)==1)
        {
            xprintf("%c", buf[i]);
            switch(status)
            {
                case 0:
                {
                    if(strstr((char*)buf, "+OK="))
                    {
                        status = 1;
                    }
                    if(strstr((char*)buf, "+ERR="))
                    {
                        return -EIO;
                    }
                    break;
                }
                case 1:
                {
                    if(buf[i]>='1' && buf[i]<='9')
                    {
                        info->SocketPort = info->SocketPort*10 + buf[i] - '0';
                    }
                    if(buf[i]=='\n')
                    {
                        status = 2;
                    }
                break;
                }
                default:break;
            }
            i = (i+1)%64;
        }
        timeout--;
        mdelay(1);
    }
    if(status==2)
    {
        ret = WiFiBtDevSetRPTM();
        if(ESUCCESS != ret) return ret;
        printf("IP:%s - SocketPort:%d\r\n", info->IP, info->SocketPort);
        return ESUCCESS;
    }
    return -EIO;
}

/**
*断开指定socket的连接
*/
int WiFiBtDevDisconnect(ConnectInfo info)
{
    int ret = -EIO;
    char skcls[32];
    sprintf(skcls, "AT+SKCLS=%d\r\n", info.SocketPort);
    xprintf("%s", skcls);
    ret = pWiFiBtDev->Write(pWiFiBtDev, (unsigned char*)skcls, strlen(skcls));
    if(ESUCCESS != ret) return ret;
    ret = WiFiBtDevCmdRet("+OK", 100);
    return ret;
}

int WiFiBtDevWrite(ConnectInfo info, unsigned char *buf, unsigned int length)
{
    if(NULL == buf) return -EINVAL;
    if(0 == length) return -EINVAL;
    
    int ret = -EIO;
    char sksnd[32];
    sprintf(sksnd, "AT+SKSND=%d,%d\r\n", info.SocketPort, length);
    xprintf("%s", sksnd);
    ret = pWiFiBtDev->Write(pWiFiBtDev, (unsigned char*)sksnd, strlen(sksnd));
    if(ESUCCESS != ret) return ret;
    
    unsigned char tmp[32] = {0};
    unsigned char i = 0, step = 0;
    unsigned int nDataLength = 0;
    unsigned int timeout = 1000;
    while(step != 3 && timeout != 0)
    {
        if(pWiFiBtDev->Read(pWiFiBtDev, &tmp[i], 1) == 1)
        {
            switch(step)
            {
                case 0:
                {
                    if(strstr((char*)tmp, "+OK="))
                    {
                        step = 1;
                    }
                    break;
                }
                case 1:
                {
                    char c = tmp[i];
                    if(c >= '0' && c <= '9')
                    {
                        nDataLength = nDataLength * 10 + c - '0';
                    }
                    if(c == '\r')
                    {
                        step = 2;
                    }
                    break;
                }
                case 2:
                {
                    if(strstr((char*)tmp, "\r\n\r\n"))
                    {
                        step = 3;
                    }
                    break;
                }
                default:break;
            }
            i++;
        }
        timeout--;
        mdelay(1);
    }
    
    if(step==3)
    {
        if(nDataLength < length)
            length = nDataLength;
        ret = pWiFiBtDev->Write(pWiFiBtDev, (unsigned char*)buf, length);
        if(ESUCCESS != ret) return -EIO;
    }
    
    return ESUCCESS;
}

int WiFiBtDevRead(ConnectInfo info, unsigned char *buf, unsigned int length)
{
    if(NULL == buf) return -EINVAL;
    if(0 == length) return -EINVAL;

    if(gNetBuffer->info.nValidLength < length)
        length = gNetBuffer->info.nValidLength;
    if(gNetBuffer->Read(gNetBuffer, (unsigned char*)buf, length) != length)
        return -EIO;
    
    return (int)length;
}



//开启TCP透传
int TCP_Connect(ConnectInfo info, char *TCP_Info)
{	
	int ret = -EIO;
	char sksdf[32];
    sprintf(sksdf, "AT+SKSDF=%d\r\n", info.SocketPort);
	ret = pWiFiBtDev->Write(pWiFiBtDev, (unsigned char*)sksdf, strlen(sksdf));
	xprintf("%s\r\n", sksdf);
	if(ESUCCESS != ret) return ret;
    ret = WiFiBtDevCmdRet("+OK", 1000);
	if(ESUCCESS != ret) return ret;
	
	char atm[32];
	sprintf(atm, "AT+ATM=!1\r\n");
	ret = pWiFiBtDev->Write(pWiFiBtDev, (unsigned char*)atm, strlen(atm));
	xprintf("%s\r\n", atm);
    if(ESUCCESS != ret) return ret;
    ret = WiFiBtDevCmdRet("+OK", 1000);
	if(ESUCCESS != ret) return ret;
	
	char entm[32];
	sprintf(entm, "AT+ENTM\r\n");
	ret = pWiFiBtDev->Write(pWiFiBtDev, (unsigned char*)entm, strlen(entm));
	xprintf("%s\r\n", entm);
    if(ESUCCESS != ret) return ret;
    ret = WiFiBtDevCmdRet("+OK", 1000);
	if(ESUCCESS != ret) return ret;
	
	ret = pWiFiBtDev->Write(pWiFiBtDev, (unsigned char*)TCP_Info, strlen(TCP_Info));
	xprintf("%s\r\n", TCP_Info);
    if(ESUCCESS != ret) return ret;
	
    return ret;
}

int TCP_Data(char *Data)
{
	int ret = pWiFiBtDev->Write(pWiFiBtDev, (unsigned char*)Data, strlen(Data));
	xprintf("%s", Data);
	if(ESUCCESS != ret) return ret;
	
	mdelay(200);
	
    return ret;
}

