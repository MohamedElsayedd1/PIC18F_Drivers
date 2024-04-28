/* 
 * File:   ecu_dc_motor.h
 * Author: Mohamed Elsayed
 *
 * Created on October 24, 2023, 7:03 PM
 */

#include "ecu_dc_motor.h"

Std_ReturnType motor_intialize(const motor_t *mtr){
    Std_ReturnType ret = E_OK;
    if(NULL == mtr){
        ret = E_NOT_OK;
    }
    else{
        // SET THE PINS TO OUTPUT AND WRITE 0V ON IT
        gpio_pin_intialize(&(mtr->dc_motor[MOTOR_PIN1]));
        gpio_pin_intialize(&(mtr->dc_motor[MOTOR_PIN2]));
    }
    return ret;
}

Std_ReturnType motor_move_left(const motor_t *mtr){
    Std_ReturnType ret = E_OK;
    if(NULL == mtr){
        ret = E_NOT_OK;
    }
    else{
        gpio_pin_write_logic(&(mtr->dc_motor[MOTOR_PIN1]), GPIO_HIGH);
        gpio_pin_write_logic(&(mtr->dc_motor[MOTOR_PIN2]), GPIO_LOW);
    }
    return ret;
}

Std_ReturnType motor_move_right(const motor_t *mtr){
    Std_ReturnType ret = E_OK;
    if(NULL == mtr){
        ret = E_NOT_OK;
    }
    else{
        gpio_pin_write_logic(&(mtr->dc_motor[MOTOR_PIN1]), GPIO_LOW);
        gpio_pin_write_logic(&(mtr->dc_motor[MOTOR_PIN2]), GPIO_HIGH);
    }
    return ret;
}

Std_ReturnType motor_stop(const motor_t *mtr){
    Std_ReturnType ret = E_OK;
    if(NULL == mtr){
        ret = E_NOT_OK;
    }
    else{
        gpio_pin_write_logic(&(mtr->dc_motor[MOTOR_PIN1]), GPIO_LOW);
        gpio_pin_write_logic(&(mtr->dc_motor[MOTOR_PIN2]), GPIO_LOW);
    }
    return ret;
}