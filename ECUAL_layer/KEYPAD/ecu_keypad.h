/* 
 * File:   ecu_keypad.h
 * Author: Mohamed Elsayed
 *
 * Created on November 12, 2023, 10:02 PM
 */

#ifndef ECU_KEYPAD_H
#define	ECU_KEYPAD_H

/*SECTION : INCLUDES*/
#include "../../MCAL_layer/GPIO/hal_gpio.h"

/* SECTION : MACROS DECLARATION*/
#define KEYPAD_ROWS    4
#define KEYPAD_COLUMNS 4

/*SECTION : MACRO FUNCTIONS*/

/*SECTION : DATA TYPE DECLARATION*/
typedef struct{
    pin_config_t keypad_rows[KEYPAD_ROWS];
    pin_config_t keypad_columns[KEYPAD_COLUMNS];
}keypad_t;

/*FUNCTIONS DECLARATIONS*/
Std_ReturnType keypad_intialize(const keypad_t *kpad);
Std_ReturnType keypad_get_value(const keypad_t *kpad, uint8_t *value);

#endif	/* ECU_KEYPAD_H */

