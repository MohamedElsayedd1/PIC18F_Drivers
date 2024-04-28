/* 
 * File:   ecu_button.h
 * Author: Mohamed Elsayed
 *
 * Created on October 10, 2023, 12:13 PM
 */

#ifndef ECU_BUTTON_H
#define	ECU_BUTTON_H

/*SECTION : INCLUDES*/
#include "../../MCAL_layer/GPIO/hal_gpio.h"


/* SECTION : MACROS DECLARATION*/

/*SECTION : MACRO FUNCTIONS*/

/*SECTION : DATA TYPE DECLARATION*/
typedef enum{
    BUTTON_RELEASED,
    BUTTON_PREASED
}button_state_t;

typedef enum{
    BUTTON_ACTIVE_LOW,
    BUTTON_ACTIVE_HIGH
}button_active_t;

typedef struct{
    pin_config_t button_pin;
    button_state_t button_state;
    button_active_t button_connection;
}button_t;

/*FUNCTIONS DECLARATIONS*/
Std_ReturnType button_intialize(const button_t *btn);
Std_ReturnType button_read_state(const button_t *btn, button_state_t *btn_state);

#endif	/* ECU_BUTTON_H */

