/* 
 * File:   ecu_7_segment.h
 * Author: Mohamed Elsayed
 *
 * Created on November 4, 2023, 3:50 PM
 */

#ifndef ECU_7_SEGMENT_H
#define	ECU_7_SEGMENT_H

/*SECTION : INCLUDES*/
#include "../../MCAL_layer/GPIO/hal_gpio.h"

/* SECTION : MACROS DECLARATION*/
#define SEGMENT_PIN0 0
#define SEGMENT_PIN1 1
#define SEGMENT_PIN2 2
#define SEGMENT_PIN3 3

/*SECTION : MACRO FUNCTIONS*/

/*SECTION : DATA TYPE DECLARATION*/
typedef enum{
    SEGMENT_COMMON_ANODE,
    SEGMENT_COMMON_CATHODE
}segment_type_t;

typedef struct{
    pin_config_t segment_pins[4];
    segment_type_t segment_type;
}segment_t;

/*FUNCTIONS DECLARATIONS*/
Std_ReturnType seven_segment_intialize(const segment_t *seg);
Std_ReturnType seven_segment_write_number(const segment_t *seg, uint8_t number);

#endif	/* ECU_7_SEGMENT_H */

