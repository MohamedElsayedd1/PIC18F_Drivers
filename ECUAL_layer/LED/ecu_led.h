/* 
 * File:   ecu_led.h
 * Author: Mohamed Elsayed
 *
 * Created on September 30, 2023, 12:33 PM
 */

#ifndef ECU_LED_H
#define	ECU_LED_H

/*SECTION : INCLUDES*/
#include "../../MCAL_layer/GPIO/hal_gpio.h"
#include "ecu_led_cfg.h"

/* SECTION : MACROS DECLARATION*/

/*SECTION : MACRO FUNCTIONS*/

/*SECTION : DATA TYPE DECLARATION*/
typedef enum{
    LED_OFF,
    LED_ON
}led_status_t;

typedef struct{
    uint8_t port        : 3;
    uint8_t pin         : 3;
    uint8_t led_status  : 1; 
}led_t;

/*FUNCTIONS DECLARATIONS*/
Std_ReturnType led_intialize(const led_t *led);
Std_ReturnType led_on(const led_t *led);
Std_ReturnType led_off(const led_t *led);
Std_ReturnType led_toggle(const led_t *led);


#endif	/* ECU_LED_H */

 