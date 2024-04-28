/* 
 * File:   ecu_relay.h
 * Author: Mohamed Elsayed
 *
 * Created on October 16, 2023, 6:53 PM
 */

#ifndef ECU_RELAY_H
#define	ECU_RELAY_H

/*SECTION : INCLUDES*/
#include "../../MCAL_layer/GPIO/hal_gpio.h"


/* SECTION : MACROS DECLARATION*/
#define RELAY_ON   0X01U
#define RELAY_OFF  0X00U

/*SECTION : MACRO FUNCTIONS*/
typedef struct{
    pin_config_t relay_pin;
    uint8_t      relay_status;
}relay_t;

/*SECTION : DATA TYPE DECLARATION*/

/*FUNCTIONS DECLARATIONS*/
Std_ReturnType relay_intialize(const relay_t *rly);
Std_ReturnType relay_turn_off(relay_t *rly);
Std_ReturnType relay_turn_on(relay_t *rly);

#endif	/* ECU_RELAY_H */

