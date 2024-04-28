/* 
 * File:   hal_gpio.c
 * Author: Mohamed Elsayed
 *
 * Created on September 30, 2023, 11:53 AM
 */

#include "mcal_eeprom.h"
Std_ReturnType EEPROM_ReadByte(uint16_t Address, uint8_t *Data){
    Std_ReturnType ret = E_OK;
    if(Data == NULL){
        ret = E_NOT_OK;
    }
    /*Writing address in EEADR-EEADRH registers*/
    EEADRH = (uint8_t)((Address >> 8) & 0x03);
    EEADR = (uint8_t)(Address & 0xFF);
    /*Clearing the EEPGD bit to select EEPROM/FLASH over configuration bits*/
    EEPROM_CLEAR_EEPGD();
    /*Clearing the EEPGD bit to select EEPROM over FLASH */
    EEPROM_CLEAR_CFGS();
        /*Set the RD register to enable reading*/
    EEPROM_ENABLE_READ();
    EECON1bits.RD = 1;
    /*2 No operation instructions*/
    /*Return the data from EEDATA register*/
    *Data = EEDATA;
    return ret;
}

Std_ReturnType EEPROM_WriteByte(uint16_t Address, uint8_t Data){
    Std_ReturnType ret = E_OK;
    /*Store the current global interrupt status*/
    uint8_t volatile interrupt_status = INTCONbits.GIEH;
    /*Writing address in EEADR-EEADRH registers*/
    EEADRH = (uint8_t)((Address >> 8) & 0x03);
    EEADR = (uint8_t)(Address & 0xFF);
    /*Write the data in EEDATA register*/
    EEDATA = Data;
    /*Clearing the EEPGD bit to select EEPROM/FLASH over configuration bits*/
    EEPROM_CLEAR_EEPGD();
    /*Clearing the EEPGD bit to select EEPROM over FLASH */
    EEPROM_CLEAR_CFGS();
    /*Set the WRN bit to allow write cycles*/
    EEPROM_SET_WREN();
    /*Disable global interrupt*/
    #if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_PRIORITY_ENABLE
    INTERRUT_GlobalInterruptHighDisable();
    INTERRUT_GlobalInterruptLowDisable();
    #elif INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_PRIORITY_DISABLE
    INTERRUPT_GlobalInterruptDisable();
    INTERRUPT_PeripheralInterruptDisable();
    #endif
    /*Write unlocking values into EECON2*/
    EECON2 = 0x55;
    EECON2 = 0xAA;
     /*Set the WR bit to enable writing*/
    EEPROM_ENABLE_WRITE();
    /*Check if the value has been written*/
    while(EECON1bits.WR);
    /*Clear the WRN bit*/
    EEPROM_CLEAR_WREN();
    /*Set the global interrupt to its previous state*/
    INTCONbits.GIEH = interrupt_status;
    INTCONbits.GIEL = interrupt_status;
    return ret;
}