/* 
 * File:   ecu_relay.c
 * Author: Mohamed Elsayed
 *
 * Created on September 30, 2023, 12:48 PM
 */

#include "ecu_relay.h"

/**
 * 
 * @param rly
 * @return status of the function
 *         (E_OK)      -> the function done successfully
 *         (E_NOT_OK)  -> the function has issue to perform this action 
 */
Std_ReturnType relay_intialize(const relay_t *rly){
    Std_ReturnType ret = E_OK;
    if(rly == NULL){
        ret = E_NOT_OK;
    }
    else{
        gpio_pin_intialize(&(rly->relay_pin));
    }
    return ret;
}

/**
 * 
 * @param rly
 * @return status of the function
 *         (E_OK)      -> the function done successfully
 *         (E_NOT_OK)  -> the function has issue to perform this action 
 */
Std_ReturnType relay_turn_on(relay_t *rly){
    Std_ReturnType ret = E_OK;
    if(rly == NULL){
        ret = E_NOT_OK;
    }
    else{
        gpio_pin_write_logic(&(rly->relay_pin), GPIO_HIGH);
        rly->relay_status = RELAY_ON;
    }
    return ret;
}

/**
 * 
 * @param rly
 * @return status of the function
 *         (E_OK)      -> the function done successfully
 *         (E_NOT_OK)  -> the function has issue to perform this action  
 */
Std_ReturnType relay_turn_off(relay_t *rly){
    Std_ReturnType ret = E_OK;
    if(rly == NULL){
        ret = E_NOT_OK;
    }
    else{
        gpio_pin_write_logic(&(rly->relay_pin), GPIO_LOW);
        rly->relay_status = RELAY_OFF;
    }
    return ret;
}