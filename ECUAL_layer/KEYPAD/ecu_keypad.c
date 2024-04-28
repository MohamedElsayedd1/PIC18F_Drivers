/* 
 * File:   ecu_keypad.h
 * Author: Mohamed Elsayed
 *
 * Created on October 24, 2023, 7:03 PM
 */

#include "ecu_keypad.h"

static const uint8_t keypad_values[KEYPAD_ROWS][KEYPAD_COLUMNS] = {
                                                        {'7', '8', '9', '/'},
                                                        {'4', '5', '6', '*'},
                                                        {'1', '2', '3', '-'},
                                                        {'#', '0', '=', '+'}
                                                     };

Std_ReturnType keypad_intialize(const keypad_t *kpad){
    Std_ReturnType ret = E_OK;
    if(kpad == NULL){
        ret = E_NOT_OK;
    }
    else{
        for(uint8_t row_counter = 0; row_counter < KEYPAD_ROWS; row_counter++){
            gpio_pin_intialize(&(kpad->keypad_rows[row_counter]));
        }
        for(uint8_t column_counter = 0; column_counter < KEYPAD_COLUMNS; column_counter++){
            gpio_pin_direction_intialize(&(kpad->keypad_rows[column_counter]));
        }
    }
    return ret;
}

Std_ReturnType keypad_get_value(const keypad_t *kpad, uint8_t *value){
    Std_ReturnType ret = E_OK;
    uint8_t column_logic = GPIO_LOW;
    if((kpad == NULL) || (value == NULL)){
        ret = E_NOT_OK;
    }
    else{
        for(uint8_t row_counter = 0; row_counter < KEYPAD_ROWS; row_counter++){
            for(uint8_t column_counter = 0; column_counter < KEYPAD_COLUMNS; column_counter++){
                ret = gpio_pin_write_logic(&(kpad->keypad_rows[column_counter]), GPIO_LOW);
            }
            ret = gpio_pin_write_logic(&(kpad->keypad_rows[row_counter]), GPIO_HIGH);
            for(uint8_t column_counter = 0; column_counter < KEYPAD_COLUMNS; column_counter++){
                ret = gpio_pin_read_logic(&(kpad->keypad_columns[column_counter]), &column_logic);
                if(GPIO_HIGH == column_logic){
                    *value = keypad_values[row_counter][column_counter];
                }
            }
        }
    }
    return ret;
}

