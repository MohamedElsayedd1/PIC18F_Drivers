/* 
 * File:   hal_gpio.h
 * Author: Mohamed Elsayed
 *
 * Created on September 30, 2023, 11:53 AM
 */

#include "ecu_led.h"

/**
 * @brief intialize the assigned pin to OUTPUT and OFF (0V)
 * @param led : pointer to the led module configuration
 * @return status of the function
 *         (E_OK)      -> the function done successfully
 *         (E_NOT_OK)  -> the function has issue to perform this action
 */
Std_ReturnType led_intialize(const led_t *led){
    Std_ReturnType ret = E_OK;
    if(NULL == led){
        ret = E_NOT_OK;
    }
    else{
        pin_config_t pin_obj = {led->port
                           , led->pin
                           , GPIO_OUTPUT
                           , led->led_status
        };
        gpio_pin_intialize(&pin_obj);
    }
    return ret;
}

/**
 * @brief Turn the led on
 * @param led : pointer to the led module configuration
 * @return status of the function
 *         (E_OK)      -> the function done successfully
 *         (E_NOT_OK)  -> the function has issue to perform this action
 */
Std_ReturnType led_on(const led_t *led){
    Std_ReturnType ret = E_OK;
    if(NULL == led){
        ret = E_NOT_OK;
    }
    else{
        pin_config_t pin_obj = {led->port
                           , led->pin
                           , GPIO_OUTPUT
                           , led->led_status
        };
        gpio_pin_write_logic(&pin_obj, GPIO_HIGH);
    }
    return ret;
}

/**
 * @brief turn the led off
 * @param led : pointer to the led module configuration
 * @return status of the function
 *         (E_OK)      -> the function done successfully
 *         (E_NOT_OK)  -> the function has issue to perform this action
 */
Std_ReturnType led_off(const led_t *led){
    Std_ReturnType ret = E_OK;
    if(NULL == led){
        ret = E_NOT_OK;
    }
    else{
        pin_config_t pin_obj = {led->port
                           , led->pin
                           , GPIO_OUTPUT
                           , led->led_status
        };
        gpio_pin_write_logic(&pin_obj, GPIO_LOW);
    }
    return ret;
}

/**
 * @brief toggle the let
 * @param led : pointer to the led module configuration
 * @return status of the function
 *         (E_OK)      -> the function done successfully
 *         (E_NOT_OK)  -> the function has issue to perform this action
 */
Std_ReturnType led_toggle(const led_t *led){
    Std_ReturnType ret = E_OK;
    if(NULL == led){
        ret = E_NOT_OK;
    }
    else{
        pin_config_t pin_obj = {led->port
                           , led->pin
                           , GPIO_OUTPUT
                           , led->led_status
        };
        gpio_pin_pin_toggle(&pin_obj);
    }
    return ret;
}
