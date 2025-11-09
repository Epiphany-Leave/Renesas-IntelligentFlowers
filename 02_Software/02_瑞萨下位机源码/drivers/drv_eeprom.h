/*
 * drv_eeprom.h
 *
 *  Created on: 2023年4月13日
 *      Author: slhuan
 */

#ifndef DRV_EEPROM_H_
#define DRV_EEPROM_H_

#define EE_PAGE_SIZE    (8)
#define EE_PAGE_NUM     (32)

typedef enum{
    isWrite,
    isRead
}EEPROM_WR_e;


#endif /* DRV_EEPROM_H_ */
