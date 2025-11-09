#include "dev_dht11.h"
#include "devices.h"
#include <libs.h>
#include <errno.h>
#include "hal_data.h"
#include <stdio.h>

static int DHT11DevInit(struct DHT11Dev *ptdev);
static int DHT11DevRead(struct DHT11Dev *ptdev);

static IODevice *gDataDevice;
static struct DHT11Dev gDevice = {
    .tempture = 0,
    .humidity = 0,
    .Init = DHT11DevInit,
    .Read = DHT11DevRead
};

struct DHT11Dev *DHT11GetDevice(void)
{
    return &gDevice;
}

static int DHT11DevInit(struct DHT11Dev *ptdev)
{
    if(NULL == ptdev)   return -EINVAL;
    gDataDevice = IODeviceFind("DHT11 IO");
    if(NULL == gDataDevice)
    {
        printf("Failed to find DHT11 IO!\r\n");
        return -ENXIO;
    }
    if(ESUCCESS != gDataDevice->Init(gDataDevice))
    {
        printf("Failed to init GPIO!\r\n");
        return -EIO;
    }
    return ESUCCESS;
}

static int DHT11DevRead(struct DHT11Dev *ptdev)
{
    if(NULL == ptdev)   return -EINVAL;
    unsigned int timeout = 100;
    static long long tmp = 0;
    unsigned int tempture_data_inter = 0, tempture_data_dec = 0;
    unsigned int humidity_data_inter = 0, humidity_data_dec = 0;
    unsigned char crc_data = 0;

    tmp = 0;
    // 主机拉低最少18ms
    gDataDevice->Write(gDataDevice, 0);
    mdelay(20);
    // 拉高等待DHT11响应，20-40us
    gDataDevice->Write(gDataDevice, 1);
    udelay(30);
    gDataDevice->Read(gDataDevice);
    while((gDataDevice->value==1) && (timeout!=0))
    {
        gDataDevice->Read(gDataDevice);
        udelay(1);
        timeout--;
    }
    if(timeout==0)  return -EIO;  // 超时未响应
    
    // 响应DHT11拉低总线80us后再拉高总线80us
    timeout = 100;
    gDataDevice->Read(gDataDevice);
    while((gDataDevice->value==0) && (timeout!=0))
    {
        gDataDevice->Read(gDataDevice);
        udelay(1);
        timeout--;
    }
    if(timeout==0)  return -EIO;  // 总线响应拉低超时
    
    timeout = 100;
    gDataDevice->Read(gDataDevice);
    while((gDataDevice->value==1) && (timeout!=0))
    {
        gDataDevice->Read(gDataDevice);
        udelay(1);
        timeout--;
    }
    if(timeout==0)  return -EIO;  // 超时未响应
    
    // 响应过后获取数据
    for(unsigned char i=0; i<40; i++)
    {
        timeout = 100;
        gDataDevice->Read(gDataDevice);
        while((gDataDevice->value==0) && (timeout!=0))
        {
            gDataDevice->Read(gDataDevice);
            udelay(1);
            timeout--;
        }
        if(timeout==0)  return -EIO;  // 超时未响应
        
        // 等待30us再次读取总线，高表示逻辑1，低表示逻辑0
        udelay(30);
        gDataDevice->Read(gDataDevice);
        if(gDataDevice->value==1)
        {
            tmp = (tmp<<1) + 1;
            udelay(40);    // 再次延时40us补满70us延时等待
        }
        else
        {
            tmp = (tmp<<1) + 0;
        }
    }
    // 数据获取完毕之后，DHT11会拉低总线50us
    timeout = 100;
    gDataDevice->Read(gDataDevice);
    while((gDataDevice->value==1) && (timeout!=0))
    {
        gDataDevice->Read(gDataDevice);
        udelay(1);
        timeout--;
    }
    if(timeout==0)  return -EIO;  // 超时未响应
    
    // 主机获取总线权限并拉高延时20ms
    gDataDevice->Write(gDataDevice, 1);
    mdelay(20);
    
    // 处理数据
    crc_data = tmp & 0xFF;
    tempture_data_dec = (tmp>>8) & 0xFF;
    tempture_data_inter = (tmp>>16) & 0xFF;
    humidity_data_dec = (tmp>>24) & 0xFF;
    humidity_data_inter = (tmp>>32) & 0xFF;
    
    if(crc_data==(tempture_data_inter + humidity_data_inter + tempture_data_dec + humidity_data_dec))
    {
        ptdev->tempture = tempture_data_inter;
        ptdev->humidity = humidity_data_inter;
        return ESUCCESS;
    }
    return -EIO;
}

