/* 
 * File:   hal_gpio.c
 * Author: Mohamed Elsayed
 *
 * Created on September 30, 2023, 11:53 AM
 */

#include "hal_gpio.h"

/*ARRAY OF POINTERS TO REGISTERS*/
volatile uint8_t *tris_registers[] = {&TRISA, &TRISB, &TRISC, &TRISD, &TRISE};
volatile uint8_t *lat_registers[] = {&LATA, &LATB, &LATC, &LATD, &LATE};
volatile uint8_t *port_registers[] = {&PORTA, &PORTB, &PORTC, &PORTD, &PORTE};

/*PINS INTIALIZATION*/
/**
 * @brief Intialize the pin to OUTPUT and turn the pin of relay OFF
 * @param _pin_config
 * @return status of the function
 *         (E_OK)      -> the function done successfully
 *         (E_NOT_OK)  -> the function has issue to perform this action
 */
#if GPIO_PORT_PIN_CONFIGURATION == ENABLE
Std_ReturnType gpio_pin_direction_intialize(const pin_config_t *_pin_config){
    Std_ReturnType ret = E_OK;
    if(_pin_config->direction == GPIO_INPUT){
        SET_BIT    (*tris_registers[_pin_config->port], _pin_config->pin);
    }
    else if(_pin_config->direction == GPIO_OUTPUT){
        CLEAR_BIT  (*tris_registers[_pin_config->port], _pin_config->pin);
    }
    else{
        ret = E_NOT_OK;
    }
    return ret;
}
#endif

/**
 * @brief Turn the pin of relay ON 
 * @param _pin_config
 * @param direction_status
 * @return status of the function
 *         (E_OK)      -> the function done successfully
 *         (E_NOT_OK)  -> the function has issue to perform this action
 */
#if GPIO_PORT_PIN_CONFIGURATION == ENABLE
Std_ReturnType gpio_pin_get_direction_status(const pin_config_t *_pin_config, direction_t *direction_status){
    Std_ReturnType ret = E_OK;
    *direction_status = READ_BIT(*tris_registers[_pin_config->port], _pin_config->pin);
    if((direction_status != 0x00 && direction_status != 0x01) || _pin_config == NULL){
        ret = E_NOT_OK;
    }
   
    return ret;
}
#endif

/**
 * @brief Turn the pin of relay OFF
 * @param _pin_config
 * @param logic
 * @return 
 */
#if GPIO_PORT_PIN_CONFIGURATION == ENABLE
Std_ReturnType gpio_pin_write_logic(const pin_config_t *_pin_config, logic_t logic){
    Std_ReturnType ret = E_OK;
    if(logic){
        SET_BIT    (*lat_registers[_pin_config->port], _pin_config->pin);
    }
    else if(logic == 0){
        CLEAR_BIT  (*lat_registers[_pin_config->port], _pin_config->pin);
    }
    else{
        ret = E_NOT_OK;
    }
    return ret;
}
#endif

/**
 * 
 * @param _pin_config
 * @param logic
 * @return status of the function
 *         (E_OK)      -> the function done successfully
 *         (E_NOT_OK)  -> the function has issue to perform this action
 */
#if GPIO_PORT_PIN_CONFIGURATION == ENABLE
Std_ReturnType gpio_pin_read_logic(const pin_config_t *_pin_config, logic_t *logic){
    Std_ReturnType ret = E_OK;
    if(_pin_config == NULL || logic == NULL){
        ret = E_NOT_OK;
    }
    *logic = READ_BIT(*port_registers[_pin_config->port], _pin_config->pin);
    return ret;
}
#endif

/**
 * 
 * @param _pin_config
 * @return status of the function
 *         (E_OK)      -> the function done successfully
 *         (E_NOT_OK)  -> the function has issue to perform this action
 */
#if GPIO_PORT_PIN_CONFIGURATION == ENABLE
Std_ReturnType gpio_pin_pin_toggle(const pin_config_t *_pin_config){
    Std_ReturnType ret = E_OK;
    if(_pin_config == NULL){
        ret = E_NOT_OK;
    }
    TOGGLE_BIT    (*lat_registers[_pin_config->port], _pin_config->pin);
    return ret;
}
#endif

#if GPIO_PORT_PIN_CONFIGURATION == ENABLE
Std_ReturnType gpio_pin_intialize(const pin_config_t *_pin_config){
    Std_ReturnType ret = E_OK;
    gpio_pin_direction_intialize(_pin_config);
    gpio_pin_write_logic(_pin_config, GPIO_LOW);
    return ret;
}
#endif


/*PORTS INTIALIZATION*/
/**
 * 
 * @param port
 * @return status of the function
 *         (E_OK)      -> the function done successfully
 *         (E_NOT_OK)  -> the function has issue to perform this action
 */
#if GPIO_PORT_CONFIGURATION == ENABLE
Std_ReturnType gpio_port_direction_intialize(port_index_t port, uint8_t direction){
    Std_ReturnType ret = E_OK;
    if(direction){
        *tris_registers[port] = direction;
    }
    else{
        *tris_registers[port] = 0X00;
    }
    if(port > MAX_PORT_SIZE){
        ret = E_NOT_OK;
    }
    return ret;
}
#endif

/**
 * 
 * @param port
 * @param direction_status
 * @return status of the function
 *         (E_OK)      -> the function done successfully
 *         (E_NOT_OK)  -> the function has issue to perform this action
 */
#if GPIO_PORT_CONFIGURATION == ENABLE
Std_ReturnType gpio_port_get_direction_status(port_index_t port, uint8_t *direction_status){
    Std_ReturnType ret = E_OK;
    if(direction_status == NULL || port > MAX_PORT_SIZE){
        ret = E_NOT_OK;
    }
    *direction_status = *tris_registers[port];
    return ret;
}
#endif

/**
 * 
 * @param port
 * @param logic
 * @return status of the function
 *         (E_OK)      -> the function done successfully
 *         (E_NOT_OK)  -> the function has issue to perform this action
 */
#if GPIO_PORT_CONFIGURATION == ENABLE
Std_ReturnType gpio_port_write_logic(port_index_t port, logic_t logic){
    Std_ReturnType ret = E_OK;
    if(port > MAX_PORT_SIZE){
        ret = E_NOT_OK;
    }
    *lat_registers[port] = logic;
    return ret;
}
#endif

/**
 * 
 * @param port
 * @param logic
 * @return status of the function
 *         (E_OK)      -> the function done successfully
 *         (E_NOT_OK)  -> the function has issue to perform this action
 */
#if GPIO_PORT_CONFIGURATION == ENABLE
Std_ReturnType gpio_port_read_logic(port_index_t port, logic_t *logic){
    Std_ReturnType ret = E_OK;
    if(port > MAX_PORT_SIZE || logic == NULL){
        ret = E_NOT_OK;
    }
    *logic = *lat_registers[port];
    return ret;
}
#endif

/**
 * 
 * @param port
 * @return status of the function
 *         (E_OK)      -> the function done successfully
 *         (E_NOT_OK)  -> the function has issue to perform this action
 */
#if GPIO_PORT_CONFIGURATION == ENABLE
Std_ReturnType gpio_port_pin_toggle(port_index_t port){
    Std_ReturnType ret = E_OK;
    if(port > MAX_PORT_SIZE){
        ret = E_NOT_OK;
    }
    *lat_registers[port] = ~(*lat_registers[port]);
    // *lat_registers[port] = *lat_registers[port] ^ 0XFF;
    return ret;
}
#endif