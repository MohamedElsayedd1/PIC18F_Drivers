/* 
 * File:   ecu_button.c
 * Author: Mohamed Elsayed
 *
 * Created on September 30, 2023, 12:48 PM
 */
#include "ecu_button.h"

/**
 * @brief intialize button bin to INPUT
 * @param btn : pointer to the button configurations 
 * @return @return status of the function
 *         (E_OK)      -> the function done successfully
 *         (E_NOT_OK)  -> the function has issue to perform this action
 */
Std_ReturnType button_intialize(const button_t *btn){
    Std_ReturnType ret = E_OK;
    if(btn == NULL){
        ret = E_NOT_OK;
    }
    ret = gpio_pin_direction_intialize(&(btn->button_pin));
    return ret;
}

/**
 * @brief read the logic or the state on the button pin
 * @param btn : pointer to the button configurations 
 * @param btn_state : pointer to the button state @ref button_state_t
 * @return @return status of the function
 *         (E_OK)      -> the function done successfully
 *         (E_NOT_OK)  -> the function has issue to perform this action
 */
Std_ReturnType button_read_state(const button_t *btn, button_state_t *btn_state){
    Std_ReturnType ret = E_OK;
    logic_t btn_logic = GPIO_LOW;
    if(btn == NULL || btn_state == NULL){
        ret = E_NOT_OK;
    }
    else{
        gpio_pin_read_logic(&(btn->button_pin), &btn_logic);
        if(BUTTON_ACTIVE_HIGH == btn->button_state){
            if(btn_logic == GPIO_HIGH){
                *btn_state = BUTTON_RELEASED;
            }
            else if (btn_logic == GPIO_LOW){
                *btn_state = BUTTON_PREASED;
            }
        }
        else if(BUTTON_ACTIVE_LOW == btn->button_state){
            if(btn_logic == GPIO_HIGH){
                *btn_state = BUTTON_PREASED;
            }
            else if (btn_logic == GPIO_LOW){
                *btn_state = BUTTON_RELEASED;
            }
        }
        else{ /* Nothing */}
        ret = E_OK;
    }
    return ret;
}
