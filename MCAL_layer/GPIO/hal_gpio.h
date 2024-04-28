/* 
 * File:   hal_gpio.h
 * Author: Mohamed Elsayed
 *
 * Created on September 30, 2023, 11:53 AM
 */

#ifndef HAL_GPIO_H
#define	HAL_GPIO_H

/*SECTION : INCLUDES*/

#include "pic18f4620.h"
#include "../std_types.h"
#include "../device_config.h"
#include "hal_gpio_cfg.h"

/* SECTION : MACROS DECLARATION*/
#define MAX_PINS_SIZE 7
#define MAX_PORT_SIZE 4

/*SECTION : MACRO FUNCTIONS*/
#define HWREG8(_X)                   *((volatile uint8_t*)(_X))

#define SET_BIT(REG, BIT_POS)        (REG |= (1 << BIT_POS))
#define CLEAR_BIT(REG, BIT_POS)      (REG &= ~(1 << BIT_POS))
#define TOGGLE_BIT(REG, BIT_POS)     (REG ^= (1 << BIT_POS))
#define READ_BIT(REG, BIT_POS)       ((REG >> BIT_POS) & (0x01))

#define GPIO_PORT_PIN_CONFIGURATION  CONFIG_ENABLE
#define GPIO_PORT_CONFIGURATION      CONFIG_ENABLE




/*SECTION : DATA TYPE DECLARATION*/
/*PORTS ENUM*/
typedef enum{
    PORTA_INDEX = 0,
    PORTB_INDEX,
    PORTC_INDEX,
    PORTD_INDEX,
    PORTE_INDEX 
}port_index_t;

/*PINS ENUM*/
typedef enum{
    GPIO_PIN0 = 0,
    GPIO_PIN1,
    GPIO_PIN2,
    GPIO_PIN3,
    GPIO_PIN4,
    GPIO_PIN5,
    GPIO_PIN6,
    GPIO_PIN7         
}pin_index_t;

/*(INPUT_OUTPUT) DIRECTION ENUM*/
typedef enum{
    GPIO_OUTPUT = 0,
    GPIO_INPUT
}direction_t;

/*(0_1) LOGIC ENUM*/
typedef enum{
    GPIO_LOW = 0,
    GPIO_HIGH
}logic_t;

/*STRUCT OF COMINATIONAL ENUMS*/
typedef struct{
    uint8_t port        : 3;    /* @ref : port_t */
    uint8_t pin         : 3;    /* @ref : pin_index_t */
    uint8_t direction   : 1;    /* @ref : direction_t */
    uint8_t logic       : 1;    /* @ref : logic_t */
}pin_config_t;

/*FUNCTIONS DECLARATIONS*/
Std_ReturnType gpio_pin_direction_intialize(const pin_config_t *_pin_config);
Std_ReturnType gpio_pin_get_direction_status(const pin_config_t *_pin_config, direction_t *direction_status);
Std_ReturnType gpio_pin_write_logic(const pin_config_t *_pin_config, logic_t logic);
Std_ReturnType gpio_pin_read_logic(const pin_config_t *_pin_config, logic_t *logic);
Std_ReturnType gpio_pin_pin_toggle(const pin_config_t *_pin_config);
Std_ReturnType gpio_pin_intialize(const pin_config_t *_pin_config);

Std_ReturnType gpio_port_direction_intialize(port_index_t port, uint8_t direction);
Std_ReturnType gpio_port_get_direction_status(port_index_t port, uint8_t *direction_status);
Std_ReturnType gpio_port_write_logic(port_index_t port, logic_t logic);
Std_ReturnType gpio_port_read_logic(port_index_t port, logic_t *logic);
Std_ReturnType gpio_port_pin_toggle(port_index_t port);

#endif	/* HAL_GPIO_H */
