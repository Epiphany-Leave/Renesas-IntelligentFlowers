#ifndef __DRIVERS_H
#define __DRIVERS_H

#include <config.h>

#if DRV_USE_GPIO
#include "./drv_gpio.h"
#endif /* DRV_USE_GPIO */

#if DRV_USE_UART
#include "./drv_uart.h"
#endif /* DRV_USE_UART */

#if DRV_USE_SCI_SPI
    #include "./drv_sci_spi.h"
#endif /* DRV_USE_SCI_SPI */

#if DRV_USE_BH1750
    #include "./drv_bh1750_iic.h"
#endif /* DRV_USE_SCI_I2C */

#if DRV_USE_BH1750_I2C
    #include "./drv_sci_i2c.h"
#endif /* DRV_USE_SCI_I2C */

#if DRV_USE_SYSTICK
#include "./drv_systick.h"
#endif /* DRV_USE_SYSTICK */

#if DRV_USE_PWM
#include "./drv_gpt_pwm.h"
#endif /* DRV_USE_SYSTICK */

#if DRV_USE_SPI
    #include "./drv_spi.h"
#endif /* DRV_USE_SPI */

#if DRV_USE_QSPI
    #include "./drv_qspi.h"
#endif /* DRV_USE_QSPI */

#if DRV_USE_I2C
    #include "./drv_i2c.h"
#endif /* DRV_USE_I2C */

#if DRV_USE_CANFD
    #include "./drv_canfd.h"
#endif /* DRV_USE_CANFD */

#if DRV_USE_GPT
#include "./drv_gpt.h"
#endif /* DRV_USE_GPT */

#if DRV_USE_RTC
#include "./drv_rtc.h"
#endif /* DRV_USE_RTC */

#if DRV_USE_DAC
#include "./drv_dac.h"
#endif /* DRV_USE_DAC */

#if DRV_USE_ELC
#include "./drv_elc.h"
#endif /* DRV_USE_ELC */

#if DRV_USE_DMA
#include "./drv_dma.h"
#endif /* DRV_USE_DMA */

#if DRV_USE_ADC
#include "./drv_adc.h"
#endif /* DRV_USE_ADC */

#if DRV_USE_TSN
#include "./drv_tsn.h"
#endif /* DRV_USE_TSN */

#if DRV_USE_WDT
#include "./drv_wdt.h"
#endif /* DRV_USE_WDT */

#if DRV_USE_IWDT
#include "./drv_iwdt.h"
#endif /* DRV_USE_IWDT */

#endif /* __DRIVERS_H */
