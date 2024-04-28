#include "ecu_7_segment.h"
    
Std_ReturnType seven_segment_intialize(const segment_t *seg){
    Std_ReturnType ret = E_OK;
    if(NULL == seg){
        ret = E_NOT_OK;
    }
    else{
        ret = gpio_pin_intialize(&(seg->segment_pins[SEGMENT_PIN0]));
        ret = gpio_pin_intialize(&(seg->segment_pins[SEGMENT_PIN1]));
        ret = gpio_pin_intialize(&(seg->segment_pins[SEGMENT_PIN2]));
        ret = gpio_pin_intialize(&(seg->segment_pins[SEGMENT_PIN3]));
    }
    return ret;
}

Std_ReturnType seven_segment_write_number(const segment_t *seg, uint8_t number){
    Std_ReturnType ret = E_OK;
    if((NULL == seg)&&(number <= 9)){
        ret = E_NOT_OK;
    }
    else{
       ret = gpio_pin_write_logic(&(seg->segment_pins[SEGMENT_PIN0]), ((number >> 0) & 0x01));
       ret = gpio_pin_write_logic(&(seg->segment_pins[SEGMENT_PIN1]), ((number >> 1) & 0x01));
       ret = gpio_pin_write_logic(&(seg->segment_pins[SEGMENT_PIN2]), ((number >> 2) & 0x01));
       ret = gpio_pin_write_logic(&(seg->segment_pins[SEGMENT_PIN3]), ((number >> 3) & 0x01));
    }
    return ret;
}
