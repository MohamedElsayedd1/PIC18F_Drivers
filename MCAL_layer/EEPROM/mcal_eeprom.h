/* 
 * File:   mcal_eeprom.h
 * Author: Mohamed Elsayed
 *
 * Created on January 24, 2024, 8:12 PM
 */

#ifndef MCAL_EEPROM_H
#define	MCAL_EEPROM_H

/*SECTION : INCLUDES*/
#include "pic18f4620.h"
#include "../std_types.h"
#include "../INTERRUPT/mcal_interrupt_config.h"

/* SECTION : MACROS DECLARATION*/
#define EEPROM_CLEAR_EEPGD()   (EECON1bits.EEPGD = 0)
#define EEPROM_CLEAR_CFGS()    (EECON1bits.CFGS = 0)
#define EEPROM_ENABLE_READ()   (EECON1bits.RD = 1)
#define EEPROM_SET_WREN()      (EECON1bits.WREN = 1)
#define EEPROM_CLEAR_WREN()    (EECON1bits.WREN = 0)
#define EEPROM_ENABLE_WRITE()  (EECON1bits.WR = 1)

/*SECTION : MACRO FUNCTIONS*/

/*SECTION : DATA TYPE DECLARATION*/

/*FUNCTIONS DECLARATIONS*/
Std_ReturnType EEPROM_ReadByte(uint16_t Address, uint8_t *Data);
Std_ReturnType EEPROM_WriteByte(uint16_t Address, uint8_t Data);

#endif	/* MCAL_EEPROM_H */
