#ifndef __CONF_H
#define __CONF_H

/* Peripheral Devices Enable/Disable */
#define DEV_USE_GPIO        1
#define DEV_USE_UART        1
#define DEV_USE_SPI         0
#define DEV_USE_TIMER       1
#define DEV_USE_QSPI        0
#define DEV_USE_I2C         0
#define DEV_USE_CANFD       0
#define DEV_USE_RTC         0
#define DEV_USE_DAC         0
#define DEV_USE_ADC         1
#define DEV_USE_DMA         0
#define DEV_USE_WDT         0

/* Modules Devices Enable/Disable */
#define DEV_USE_LCD         1
#define DEV_USE_TOUCH       0
#define DEV_USE_OLED        0
#define DEV_USE_EEPROM      0
#define DEV_USE_W25Q        0
#define DEV_USE_IRDA        0
#define DEV_USE_WiFiBt      1
#define DEV_USE_DS18B20     0
#define DEV_USE_DHT11       1
#define DEV_USE_BH1750		1
#define DEV_USE_PH			1
#define DEV_USE_MH			1
#define DEV_USE_MQ			1
#define DEV_USE_ULTRA       0
#define DEV_USE_ADXL345     0
#define DEV_USE_MOTOR       0
#define DEV_USE_PHOTOSENS   0
#define DEV_USE_SR501       0
#define DEV_USE_SPIDAC      0

/* Libraries Enable/Disable */
#define LIBS_USE_DELAY      1
#define LIBS_USE_PRINTF     1
#define LIBS_USE_BUFFER     1

/* Peripheral Drivers Enable/Disable */
#if DEV_USE_GPIO
    #define DRV_USE_GPIO    1
#endif /* DEV_USE_GPIO */

#if DEV_USE_UART
    #define DRV_USE_UART    1
#endif /* DEV_USE_UART */

#if DEV_USE_SPI
    #define DRV_USE_SCI_SPI 1
    #define DRV_USE_SPI     1
#endif /* DEV_USE_SPI */

#if DEV_USE_TIMER
    #define DRV_USE_GPT     1
    #define DRV_USE_SYSTICK 1
	#define DRV_USE_PWM		1
#endif /* DEV_USE_TIMER */

#if DEV_USE_QSPI
    #define DRV_USE_QSPI    1
#endif /* DEV_USE_QSPI */

#if DEV_USE_I2C
    #define DRV_USE_SCI_I2C 1
    #define DRV_USE_I2C     1
#endif /* DEV_USE_I2C */

#if DEV_USE_CANFD
    #define DRV_USE_CANFD   1
#endif /* DEV_USE_CANFD */

#if DEV_USE_RTC
    #define DRV_USE_RTC     1
#endif /* DEV_USE_RTC */

#if DEV_USE_DAC
    #define DRV_USE_DAC     1
#endif /* DEV_USE_DAC */

#if DEV_USE_ADC
    #define DRV_USE_ADC     1
    #define DRV_USE_TSN     0
#endif /* DEV_USE_ADC */

#if DEV_USE_DMA
    #define DRV_USE_DMA     1
#endif /* DEV_USE_DMA */

#if DEV_USE_WDT
    #define DRV_USE_WDT     1
    #define DRV_USE_IWDT    1
#endif /* DEV_USE_WDT */

#if DEV_USE_BH1750
	#define DRV_USE_BH1750	1
#endif /* DEV_USE_BH1750 */

#endif /* __CONF_H */
