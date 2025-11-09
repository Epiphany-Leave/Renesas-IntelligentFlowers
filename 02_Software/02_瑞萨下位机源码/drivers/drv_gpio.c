#include "drv_gpio.h"
#include "hal_data.h"
#include "delay/delay.h"
#include <devices.h>
#include <drivers.h>
#include <errno.h>
#include <stdio.h>

static int IODrvInit(struct IODev *ptdev);
static int IODrvWrite(struct IODev *ptdev, unsigned char level);
static int IODrvRead(struct IODev *ptdev);

static struct IODev gDHTIODev = {
    .name = "DHT11 IO",
    .port = BSP_IO_PORT_05_PIN_03 ,
    .value = 0,
    .Init = IODrvInit,
    .Read = IODrvRead,
    .Write = IODrvWrite,
    .next = NULL
};

static struct IODev gKeyDev = {
    .name = "UserKey",
    .Init = IODrvInit,
    .Write = NULL,
    .Read = NULL
};


void IODevicesCreate(void)
{
    IODeviceInsert(&gDHTIODev);
	IODeviceInsert(&gKeyDev);
}

struct IODev *IOGetDecvice(const char *name)
{
    if(strcmp(name, "UserKey") == 0)
        return &gKeyDev;
	else if(strcmp(name, "DHT11 IO") == 0)
        return &gDHTIODev;

    return NULL;
}

static int IODrvInit(struct IODev *ptdev)
{
    if(ptdev == NULL)       return -EINVAL;
    if(ptdev->name == NULL) return -EINVAL;
    
	if(strcmp(ptdev->name, "DHT11 IO") == 0)
	{
		fsp_err_t err = g_ioport.p_api->open(g_ioport.p_ctrl, g_ioport.p_cfg);
		assert(FSP_SUCCESS == err);
		
		return ESUCCESS;
	}
	
	//中断
	if(strcmp(ptdev->name, "UserKey") == 0)
	{
		fsp_err_t err = g_external_irq6.p_api->open(g_external_irq6.p_ctrl, g_external_irq6.p_cfg);
		assert(FSP_SUCCESS == err);
		err = g_external_irq8.p_api->open(g_external_irq8.p_ctrl, g_external_irq8.p_cfg);
		assert(FSP_SUCCESS == err);
		err = g_external_irq9.p_api->open(g_external_irq9.p_ctrl, g_external_irq9.p_cfg);
		assert(FSP_SUCCESS == err);
		
		err = g_external_irq6.p_api->enable(g_external_irq6.p_ctrl);
		assert(FSP_SUCCESS == err);
		err = g_external_irq8.p_api->enable(g_external_irq8.p_ctrl);
		assert(FSP_SUCCESS == err);
		err = g_external_irq9.p_api->enable(g_external_irq9.p_ctrl);
		assert(FSP_SUCCESS == err);
		printf("ICU OK!\r\n");
	
		return ESUCCESS;
	}
	return 0;
}

static int IODrvWrite(struct IODev *ptdev, unsigned char level)
{
    if(ptdev == NULL)       return -EINVAL;
    if(ptdev->name == NULL) return -EINVAL;
    
    fsp_err_t err = g_ioport.p_api->pinCfg(g_ioport.p_ctrl, ptdev->port, IOPORT_CFG_PORT_DIRECTION_OUTPUT);
    assert(FSP_SUCCESS == err);
    err = g_ioport.p_api->pinWrite(g_ioport.p_ctrl, ptdev->port, (bsp_io_level_t)level);
    assert(FSP_SUCCESS == err);

    return ESUCCESS;
}

static int IODrvRead(struct IODev *ptdev)
{
    if(ptdev == NULL)       return -EINVAL;
    if(ptdev->name == NULL) return -EINVAL;

    fsp_err_t err = g_ioport.p_api->pinCfg(g_ioport.p_ctrl, ptdev->port, IOPORT_CFG_PORT_DIRECTION_INPUT);
    assert(FSP_SUCCESS == err);
    err = g_ioport.p_api->pinRead(g_ioport.p_ctrl, ptdev->port, (bsp_io_level_t*)&ptdev->value);
    assert(FSP_SUCCESS == err);

    return ESUCCESS;
}

//-------ICU---------//
uint32_t last_interrupt_time = 0;

static volatile bsp_io_level_t gLedLevel = BSP_IO_LEVEL_HIGH;
static volatile bsp_io_level_t gMotorLevel = BSP_IO_LEVEL_LOW;
static volatile bsp_io_level_t gWaterpumpLevel = BSP_IO_LEVEL_LOW;
static volatile bsp_io_level_t gWindLevel = BSP_IO_LEVEL_LOW;

volatile bool key_sw1_press = false;
volatile bool key_sw2_press = false;
volatile bool key_sw3_press = false;
volatile bool key_sw4_press = false;
void key_external_irq_callback(external_irq_callback_args_t * p_args)
{
	uint32_t current_time = HAL_GetTick();
	
    if(p_args->channel == 6)    // 按键的GPIO的ICU通道是6--P000
    {
		if((current_time - last_interrupt_time) > DEBOUNCE_TIME)
		{
			last_interrupt_time = current_time;
			key_sw1_press = true;
		}
	}
	else if(p_args->channel == 8)    // 按键的GPIO的ICU通道是8---P905
    {		
		if((current_time - last_interrupt_time) > DEBOUNCE_TIME)
		{
			last_interrupt_time = current_time;
			key_sw2_press = true;
		}
    }
	else if(p_args->channel == 9)    // 按键的GPIO的ICU通道是9---P906
    {
		if((current_time - last_interrupt_time) > DEBOUNCE_TIME)
		{
			last_interrupt_time = current_time;
			key_sw3_press = true;
		}
    }
	else if(p_args->channel == 10)
	{
		if((current_time - last_interrupt_time) > DEBOUNCE_TIME)
		{
			last_interrupt_time = current_time;
			key_sw4_press = true;
		}
	}
}

void KeyProcessJitter(void)
{
	if(key_sw1_press)
	{
		g_ioport.p_api->pinWrite(&g_ioport_ctrl, BSP_IO_PORT_04_PIN_00, gLedLevel);
		g_ioport.p_api->pinWrite(&g_ioport_ctrl, BSP_IO_PORT_09_PIN_01, gMotorLevel);
//		g_ioport.p_api->pinWrite(&g_ioport_ctrl, BSP_IO_PORT_03_PIN_13, gMotorLevel);
		gMotorLevel = !gMotorLevel;
		printf("Turn LED!\r\n");
		gLedLevel = !gLedLevel;  
		key_sw1_press = false;
	}
}

void KeyProcessMotor(void)
{
	if(key_sw2_press)
	{
		g_ioport.p_api->pinWrite(&g_ioport_ctrl, BSP_IO_PORT_09_PIN_01, gMotorLevel);
		g_ioport.p_api->pinWrite(&g_ioport_ctrl, BSP_IO_PORT_03_PIN_13, gMotorLevel);
		printf("Turn Motor\r\n");
		gMotorLevel = !gMotorLevel;
		if(gMotorLevel)
			ST7735_DrawString(0,102,"Open Motor",Font_7x10,ST7735_WHITE,ST7735_BLACK);
		else
			ST7735_DrawString(0,102,"Close Motor",Font_7x10,ST7735_WHITE,ST7735_BLACK);
		key_sw2_press = false;
	}
}

void KeyProcessWater(void)
{
	if(key_sw3_press)
	{
		g_ioport.p_api->pinWrite(&g_ioport_ctrl, BSP_IO_PORT_02_PIN_11, gWaterpumpLevel);
		printf("Turn gWaterpumpLevel\r\n");
		gWaterpumpLevel = !gWaterpumpLevel;
		if(gWaterpumpLevel)
			ST7735_DrawString(44,116,"Open Water",Font_7x10,ST7735_WHITE,ST7735_BLACK);
		else
			ST7735_DrawString(44,116,"Close Water",Font_7x10,ST7735_WHITE,ST7735_BLACK);
		key_sw3_press = false;
	}
}

void KeyProcessWind(void)
{
	if(key_sw4_press)
	{
		g_ioport.p_api->pinWrite(&g_ioport_ctrl, BSP_IO_PORT_07_PIN_08, gMotorLevel);
		g_ioport.p_api->pinWrite(&g_ioport_ctrl, BSP_IO_PORT_04_PIN_11, gMotorLevel);
		printf("Turn Wind\r\n");
		gWindLevel = !gWindLevel;
		if(gWindLevel)
			ST7735_DrawString(0,102,"Open Wind",Font_7x10,ST7735_WHITE,ST7735_BLACK);
		else
			ST7735_DrawString(0,102,"Close Wind",Font_7x10,ST7735_WHITE,ST7735_BLACK);
		key_sw4_press = false;
	}
}
