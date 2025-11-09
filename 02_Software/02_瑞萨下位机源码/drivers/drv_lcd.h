/*
 * drv_disp.h
 *
 *  Created on: 2023年4月12日
 *      Author: slhuan
 */

#ifndef DRV_LCD_H_
#define DRV_LCD_H_

#include "hal_data.h"

//typedef enum{
//    notLight,
//    isLight
//}Black;   /* 背光引脚控制状态 */

//typedef enum{
//    isReset,
//    notReset
//}Reset;   /* 复位引脚控制状态 */

//typedef enum{
//    isSelect,
//    notSelect
//}CS;      /* 片选信号控制状态 */

//typedef enum{
//    isCommand,
//    isData
//}DCX;     /* 数据/命令切换控制状态 */

#define LCD_BL_H()	R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_06_PIN_10, BSP_IO_LEVEL_HIGH)
#define LCD_BL_L()	R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_06_PIN_10, BSP_IO_LEVEL_LOW)

#define LCD_CS_H()	R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_14, BSP_IO_LEVEL_HIGH)
#define LCD_CS_L()	R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_14, BSP_IO_LEVEL_LOW)

#define LCD_DC_D()	R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_06_PIN_09, BSP_IO_LEVEL_HIGH)
#define LCD_DC_C()	R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_06_PIN_09, BSP_IO_LEVEL_LOW)

#define LCD_RES_H()	R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_06_PIN_12, BSP_IO_LEVEL_HIGH)
#define LCD_RES_L()	R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_06_PIN_12, BSP_IO_LEVEL_LOW)

#define USE_HORIZONTAL 1

#endif /* DRV_LCD_H_ */
