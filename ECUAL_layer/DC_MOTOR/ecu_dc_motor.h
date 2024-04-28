/* 
 * File:   ecu_dc_motor.h
 * Author: Mohamed Elsayed
 *
 * Created on October 24, 2023, 7:03 PM
 */

#ifndef ECU_DC_MOTOR_H
#define	ECU_DC_MOTOR_H

/*SECTION : INCLUDES*/
#include "../../MCAL_layer/GPIO/hal_gpio.h"


/* SECTION : MACROS DECLARATION*/
#define MOTOR_ON   0X01U
#define MOTOR_OFF  0X00U

#define MOTOR_PIN1 0X00U
#define MOTOR_PIN2 0X01U

/*SECTION : MACRO FUNCTIONS*/

/*SECTION : DATA TYPE DECLARATION*/
typedef enum{
    MOTOR_RIGHT,
    MOTOR_LEFT
}motor_direction;

typedef struct{
    pin_config_t dc_motor[2];
}motor_t;

/*FUNCTIONS DECLARATIONS*/
Std_ReturnType motor_intialize(const motor_t *mtr);
Std_ReturnType motor_move_right(const motor_t *mtr);
Std_ReturnType motor_move_left(const motor_t *mtr);
Std_ReturnType motor_stop(const motor_t *mtr);

#endif	/* ECU_DC_MOTOR_H */

