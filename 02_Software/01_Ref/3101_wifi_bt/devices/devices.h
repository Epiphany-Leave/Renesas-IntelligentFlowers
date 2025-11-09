#ifndef __DEVICES_H
#define __DEVICES_H

#include <config.h>

/* Peripheral Devices Include */
#if DEV_USE_GPIO
    #include "./dev_gpio.h"
#endif /* DEV_USE_GPIO */

#if DEV_USE_UART
    #include "./dev_uart.h"
#endif /* DEV_USE_UART */

#if DEV_USE_SPI
    #include "./dev_spi.h"
#endif /* DEV_USE_SPI */

#if DEV_USE_TIMER
    #include "./dev_timer.h"
#endif

#if DEV_USE_QSPI
    #include "./dev_qspi.h"
#endif /* DEV_USE_QSPI */

#if DEV_USE_I2C
    #include "./dev_i2c.h"
#endif /* DEV_USE_I2C */

#if DEV_USE_CANFD
    #include "./dev_canfd.h"
#endif /* DEV_USE_CANFD */

#if DEV_USE_RTC
    #include "./dev_rtc.h"
#endif /* DEV_USE_RTC */

#if DEV_USE_DAC
    #include "./dev_dac.h"
#endif /* DEV_USE_DAC */

#if DEV_USE_ADC
    #include "./dev_adc.h"
#endif /* DEV_USE_ADC */

#if DEV_USE_DMA
    #include "./dev_dma.h"
#endif /* DEV_USE_DMA */

#if DEV_USE_WDT
    #include "./dev_wdt.h"
#endif /* DEV_USE_WDT */

/* Modules Devices Include */

#if DEV_USE_LCD
    #include "./LCD/dev_lcd.h"
#endif /* DEV_USE_LCD */

#if DEV_USE_TOUCH
    #include "./touch/dev_touch.h"
#endif /* DEV_USE_TOUCH */

#if DEV_USE_OLED
    #include "./OLED/dev_oled.h"
#endif /* DEV_USE_OLED */

#if DEV_USE_EEPROM
    #include "./EEPROM/dev_eeprom.h"
#endif /* DEV_USE_EEPROM */

#if DEV_USE_W25Q
    #include "./W25Q/dev_w25q.h"
#endif /* DEV_USE_W25Q */

#if DEV_USE_IRDA
    #include "./IRDA/dev_irda.h"
#endif /* DEV_USE_IRDA */

#if DEV_USE_WiFiBt
    #include "./wifi_bluetooth/dev_wifi_bt.h"
#endif /* DEV_USE_WiFiBt */

#if DEV_USE_DS18B20
    #include "./DS18B20/dev_ds18b20.h"
#endif /* DEV_USE_DS18B20 */

#if DEV_USE_DHT11
    #include "./DHT11/dev_dht11.h"
#endif /* DEV_USE_DHT11 */

#if DEV_USE_ULTRA
    #include "./ultra/dev_ultra.h"
#endif /* DEV_USE_ULTRA */

#if DEV_USE_ADXL345
    #include "./ADXL345/dev_adxl345.h"
#endif /* DEV_USE_ADXL345 */

#if DEV_USE_MOTOR
    #include "./motor/dev_motor.h"
#endif /* DEV_USE_MOTOR */

#if DEV_USE_PHOTOSENS
    #include "./photosens/dev_photosens.h"
#endif /* DEV_USE_PHOTOSENS */

#if DEV_USE_SR501
    #include "./sr501/dev_sr501.h"
#endif /* DEV_USE_SR501 */

#if DEV_USE_SPIDAC
    #include "./spi_dac/dev_spi_dac.h"
#endif /* DEV_USE_SPIDAC */

#endif /* __DEVICES_H */
