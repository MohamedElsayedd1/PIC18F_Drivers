/* 
 * File:   ecu_lcd.h
 * Author: Dubai Store
 *
 * Created on December 8, 2023, 1:44 PM
 */

#ifndef ECU_LCD_H
#define	ECU_LCD_H

/*SECTION : INCLUDES*/
#include "../../MCAL_layer/GPIO/hal_gpio.h"

/* SECTION : MACROS DECLARATION*/
#define _LCD_CLEAR                      0X01
#define _LCD_RETURN_HOME                0X02
#define _LCD_INC_SHIFT_OFF              0X06
#define _LCD_DEC_SHIFT_OFF              0X04
#define _LCD_INC_SHIFT_ON               0X07
#define _LCD_DEC_SHIFT_ON               0X05
#define _LCD_CURSOR_SHIFT_LEFT          0X10
#define _LCD_CURSOR_SHIFT_RIGHT         0X14
#define _LCD_DISPLAY_SHIFT_LEFT         0X18
#define _LCD_DISPLAY_SHIFT_RIGHT        0X1C
#define _LCD_CURSOR_OFF_DISPLAY_ON      0X0C
#define _LCD_CURSOR_OFF_DISPLAY_OFF     0X08
#define _LCD_CURSOR_ON_BLINK_ON         0X0F
#define _LCD_CURSOR_ON_BLINK_OFF        0X0E
#define _LCD_8BIT_MODE_2LINE            0X38
#define _LCD_4BIT_MODE_2LINE            0X28
#define _LCD_DDRAM_START                0X80
#define _LCD_CGRAM_START                0X40
#define _LCD_4BIT_PINS                   4
#define _LCD_8BIT_PINS                   8

#define ROW1  1
#define ROW2  2
#define ROW3  3
#define ROW4  4

/*SECTION : DATA TYPE DECLARATION*/
typedef struct{
    pin_config_t lcd_data[4];
    pin_config_t rs_pin;
    pin_config_t en_pin;
}lcd_4bit_t;

typedef struct{
    pin_config_t lcd_data[8];
    pin_config_t rs_pin;
    pin_config_t en_pin;
}lcd_8bit_t;

/*FUNCTIONS DECLARATIONS*/
Std_ReturnType lcd_4bit_intialize(const lcd_4bit_t *lcd);
Std_ReturnType lcd_4bit_send_command(const lcd_4bit_t *lcd, uint8_t command);
Std_ReturnType lcd_4bit_send_char_data(const lcd_4bit_t *lcd, uint8_t data);
Std_ReturnType lcd_4bit_send_char_data_pos(const lcd_4bit_t *lcd, uint8_t data, uint8_t row, uint8_t column);
Std_ReturnType lcd_4bit_send_string(const lcd_4bit_t *lcd, uint8_t str[]);
Std_ReturnType lcd_4bit_send_string_pos(const lcd_4bit_t *lcd, uint8_t str[], uint8_t row, uint8_t column);
Std_ReturnType lcd_4bit_send_custom_char(const lcd_4bit_t *lcd, uint8_t cutom_chr[], uint8_t row, uint8_t column, uint8_t memory_pos);

Std_ReturnType lcd_8bit_intialize(const lcd_8bit_t *lcd);
Std_ReturnType lcd_8bit_send_command(const lcd_8bit_t *lcd, uint8_t command);
Std_ReturnType lcd_8bit_send_char_data(const lcd_8bit_t *lcd, uint8_t data);
Std_ReturnType lcd_8bit_send_char_data_pos(const lcd_8bit_t *lcd, uint8_t data, uint8_t row, uint8_t column);
Std_ReturnType lcd_8bit_send_string(const lcd_8bit_t *lcd, uint8_t str[]);
Std_ReturnType lcd_8bit_send_string_pos(const lcd_8bit_t *lcd, uint8_t str[], uint8_t row, uint8_t column);
Std_ReturnType lcd_8bit_send_custom_char(const lcd_8bit_t *lcd, uint8_t cutom_chr[], uint8_t row, uint8_t column, uint8_t memory_pos);

Std_ReturnType covert_byte_to_str(uint8_t value, uint8_t *str);
Std_ReturnType covert_short_to_str(uint16_t value, uint8_t *str);
Std_ReturnType covert_int_to_str(uint32_t value, uint8_t *str);

#endif	/* ECU_LCD_H */

