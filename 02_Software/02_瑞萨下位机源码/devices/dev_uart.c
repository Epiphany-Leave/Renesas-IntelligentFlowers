#include "dev_uart.h"
#include <drivers.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


static struct UartDev *gHeadUartDev;

void UartDevicesRegister(void)
{
    UartDevicesCreate();
    UartDeviceList();
}

void UartDeviceInsert(struct UartDev *ptdev)
{
    if(NULL == gHeadUartDev)
        gHeadUartDev = ptdev;
    else
    {
        ptdev->next = gHeadUartDev;
        gHeadUartDev = ptdev;
    }
}

struct UartDev *UartDeviceFind(const char *name)
{
    struct UartDev *ptdev = gHeadUartDev;
    while(ptdev)
    {
        if(strstr(ptdev->name, name))
        {
            return ptdev;
        }
        ptdev = ptdev->next;
    }
    return NULL;
}

void UartDeviceList(void)
{
    struct UartDev *ptdev = gHeadUartDev;
    printf("\r\nUart Device List:\r\n");
    while(ptdev)
    {
        printf("\t%s\r\n", ptdev->name);
        ptdev = ptdev->next;
    }
    printf("\r\n");
}
