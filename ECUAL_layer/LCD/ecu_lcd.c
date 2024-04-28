#include "ecu_lcd.h"
#include <string.h>

static Std_ReturnType lcd_4bit_send(const lcd_4bit_t *lcd, uint8_t command);
static Std_ReturnType lcd_4bit_send_enable_signal(const lcd_4bit_t *lcd);
static Std_ReturnType lcd_8bit_send_enable_signal(const lcd_8bit_t *lcd);
static Std_ReturnType lcd_8bit_set_cursor(const lcd_8bit_t *lcd, uint8_t row, uint8_t column);
static Std_ReturnType lcd_4bit_set_cursor(const lcd_8bit_t *lcd, uint8_t row, uint8_t column);

Std_ReturnType lcd_4bit_intialize(const lcd_4bit_t *lcd){
    Std_ReturnType ret = E_OK;
    if(NULL == lcd){
        ret = E_NOT_OK;
    }
    else{
        ret = gpio_pin_intialize(&(lcd->rs_pin));
        ret = gpio_pin_intialize(&(lcd->en_pin));
        for(int pin_counter = 0; pin_counter < _LCD_4BIT_PINS ; pin_counter++){
            ret = gpio_pin_intialize(&(lcd->lcd_data[pin_counter]));
        }
        __delay_ms(20);
        ret = lcd_4bit_send_command(lcd, _LCD_8BIT_MODE_2LINE );
        __delay_ms(5);
        ret = lcd_4bit_send_command(lcd, _LCD_8BIT_MODE_2LINE );
        __delay_us(180);
        ret = lcd_4bit_send_command(lcd, _LCD_8BIT_MODE_2LINE );
        
        ret = lcd_4bit_send_command(lcd, _LCD_CLEAR );
        ret = lcd_4bit_send_command(lcd, _LCD_RETURN_HOME );
        ret = lcd_4bit_send_command(lcd, _LCD_INC_SHIFT_OFF );
        ret = lcd_4bit_send_command(lcd, _LCD_CURSOR_OFF_DISPLAY_ON );
        ret = lcd_4bit_send_command(lcd, _LCD_4BIT_MODE_2LINE );
        ret = lcd_4bit_send_command(lcd, _LCD_DDRAM_START );
    }
    return ret;
}

Std_ReturnType lcd_4bit_send_command(const lcd_4bit_t *lcd, uint8_t command){
    Std_ReturnType ret = E_OK;
    if(NULL == lcd){
        ret = E_NOT_OK;
    }
    else{
        ret = gpio_pin_write_logic(&(lcd->rs_pin), GPIO_LOW);
        
        ret = lcd_4bit_send(lcd, (command >> 4));
        ret = lcd_4bit_send_enable_signal(lcd);
        
        ret = lcd_4bit_send(lcd, (command >> 0));
        ret = lcd_4bit_send_enable_signal(lcd);
    }
    return ret;
}

Std_ReturnType lcd_4bit_send_char_data(const lcd_4bit_t *lcd, uint8_t data){
    Std_ReturnType ret = E_OK;
    if(NULL == lcd){
        ret = E_NOT_OK;
    }
    else{
        ret = gpio_pin_write_logic(&(lcd->rs_pin), GPIO_HIGH);
        
        ret = lcd_4bit_send(lcd, (data >> 4));
        ret = lcd_4bit_send_enable_signal(lcd);
        
        ret = lcd_4bit_send(lcd, (data >> 0));
        ret = lcd_4bit_send_enable_signal(lcd);
    }
    return ret;
}

Std_ReturnType lcd_4bit_send_char_data_pos(const lcd_4bit_t *lcd, uint8_t data, uint8_t row, uint8_t column){
    Std_ReturnType ret = E_OK;
    if(NULL == lcd){
        ret = E_NOT_OK;
    }
    else{
        ret = lcd_4bit_set_cursor(lcd, row, column);
        ret = lcd_4bit_send_char_data(lcd, data);
    }
    return ret;
}

Std_ReturnType lcd_4bit_send_string(const lcd_4bit_t *lcd, uint8_t str[]){
    Std_ReturnType ret = E_OK;
    if(NULL == lcd){
        ret = E_NOT_OK;
    }
    else{
        for(uint8_t chr_counter = 0; chr_counter < strlen(str); chr_counter++){
            ret = lcd_4bit_send_char_data(lcd, str[chr_counter]);
        }
    }
    return ret;
}

Std_ReturnType lcd_4bit_send_string_pos(const lcd_4bit_t *lcd, uint8_t str[], uint8_t row, uint8_t column){
    Std_ReturnType ret = E_OK;
    if(NULL == lcd){
        ret = E_NOT_OK;
    }
    else{
        ret = lcd_4bit_set_cursor(lcd, row, column);
        while(*str){
            ret = lcd_4bit_send_char_data(lcd, *str++);
        }
    }
    return ret;
}

Std_ReturnType lcd_4bit_send_custom_char(const lcd_4bit_t *lcd, uint8_t cutom_chr[], uint8_t row, uint8_t column, uint8_t memory_pos){
    Std_ReturnType ret = E_OK;
    if(NULL == lcd){
        ret = E_NOT_OK;
    }
    else{
        
    }
    return ret;
}

/////////////////////////

Std_ReturnType lcd_8bit_intialize(const lcd_8bit_t *lcd){
    Std_ReturnType ret = E_OK;
    if(NULL == lcd){
        ret = E_NOT_OK;
    }
    else{
        ret = gpio_pin_intialize(&(lcd->rs_pin));
        ret = gpio_pin_intialize(&(lcd->en_pin));
        for(int pin_counter = 0; pin_counter < _LCD_8BIT_PINS ; pin_counter++){
            ret = gpio_pin_intialize(&(lcd->lcd_data[pin_counter]));
        }
        __delay_ms(20);
        ret = lcd_8bit_send_command(lcd, _LCD_8BIT_MODE_2LINE );
        __delay_ms(5);
        ret = lcd_8bit_send_command(lcd, _LCD_8BIT_MODE_2LINE );
        __delay_us(180);
        ret = lcd_8bit_send_command(lcd, _LCD_8BIT_MODE_2LINE );
        
        ret = lcd_8bit_send_command(lcd, _LCD_CLEAR );
        ret = lcd_8bit_send_command(lcd, _LCD_RETURN_HOME );
        ret = lcd_8bit_send_command(lcd, _LCD_INC_SHIFT_OFF );
        ret = lcd_8bit_send_command(lcd, _LCD_CURSOR_OFF_DISPLAY_ON );
        ret = lcd_8bit_send_command(lcd, _LCD_8BIT_MODE_2LINE );
        ret = lcd_8bit_send_command(lcd, _LCD_DDRAM_START );
    }
    return ret;
}

Std_ReturnType lcd_8bit_send_command(const lcd_8bit_t *lcd, uint8_t command){
    Std_ReturnType ret = E_OK;
    if(NULL == lcd){
        ret = E_NOT_OK;
    }
    else{
        ret = gpio_pin_write_logic(&(lcd->rs_pin), GPIO_LOW);
        for(uint8_t pin_counter = 0; pin_counter < _LCD_8BIT_PINS; pin_counter++){
            ret = gpio_pin_write_logic(&(lcd->lcd_data[pin_counter]), (command >> pin_counter) & 0X01);
        }
        ret = lcd_8bit_send_enable_signal(lcd);
    }
    return ret;
}

Std_ReturnType lcd_8bit_send_char_data(const lcd_8bit_t *lcd, uint8_t data){
    Std_ReturnType ret = E_OK;
    if(NULL == lcd){
        ret = E_NOT_OK;
    }
    else{
        ret = gpio_pin_write_logic(&(lcd->rs_pin), GPIO_HIGH);
        for(uint8_t pin_counter = 0; pin_counter < _LCD_8BIT_PINS; pin_counter++){
            ret = gpio_pin_write_logic(&(lcd->lcd_data[pin_counter]), (data >> pin_counter) & 0X01);
        }
        ret = lcd_8bit_send_enable_signal(lcd);
    }
    return ret;
}

Std_ReturnType lcd_8bit_send_char_data_pos(const lcd_8bit_t *lcd, uint8_t data, uint8_t row, uint8_t column){
    Std_ReturnType ret = E_OK;
    if(NULL == lcd){
        ret = E_NOT_OK;
    }
    else{
        lcd_8bit_set_cursor(lcd, row, column);
        lcd_8bit_send_char_data(lcd, data);
    }
    return ret;
}

Std_ReturnType lcd_8bit_send_string(const lcd_8bit_t *lcd, uint8_t str[]){
    Std_ReturnType ret = E_OK;
    if(NULL == lcd){
        ret = E_NOT_OK;
    }
    else{
        for(uint8_t chr_counter = 0; chr_counter < strlen(str); chr_counter++){
            ret = lcd_8bit_send_char_data(lcd, str[chr_counter]);
        }
    }
    return ret;
}

Std_ReturnType lcd_8bit_send_string_pos(const lcd_8bit_t *lcd, uint8_t str[], uint8_t row, uint8_t column){
    Std_ReturnType ret = E_OK;
    if(NULL == lcd){
        ret = E_NOT_OK;
    }
    else{
        ret = lcd_8bit_set_cursor(lcd, row, column);
        ret = lcd_8bit_send_string(lcd, str);
    }
    return ret;
}

Std_ReturnType lcd_8bit_send_custom_char(const lcd_8bit_t *lcd, uint8_t cutom_chr[], uint8_t row, uint8_t column, uint8_t memory_pos){
    Std_ReturnType ret = E_OK;
    uint8_t cgram_counter = 0;
    if(NULL == lcd){
        ret = E_NOT_OK;
    }
    else{
        ret = lcd_8bit_send_command(lcd, (_LCD_CGRAM_START+(8*memory_pos)));
        for(cgram_counter = 0; cgram_counter <= 7; cgram_counter++){
            ret = lcd_8bit_send_char_data(lcd, cutom_chr[cgram_counter]);
        }
        ret = lcd_8bit_send_char_data_pos(lcd, memory_pos, row, column);
    }
    return ret;
}

Std_ReturnType covert_byte_to_str(uint8_t value, uint8_t *str){
    Std_ReturnType ret = E_OK;
    if(NULL == str){
        ret = E_NOT_OK;
    }
    else{
        memset(str, '\0', 4);
        sprintf(str, "%d", value);
    }
    return ret;
}

Std_ReturnType covert_short_to_str(uint16_t value, uint8_t *str){
    Std_ReturnType ret = E_OK;
    if(NULL == str){
        ret = E_NOT_OK;
    }
    else{
        memset(str, '\0', 6);
        sprintf(str, "%d", value);
    }
    return ret;
}

Std_ReturnType covert_int_to_str(uint32_t value, uint8_t *str){
    Std_ReturnType ret = E_OK;
    if(NULL == str){
        ret = E_NOT_OK;
    }
    else{
        memset(str, '\0', 11);
        sprintf(str, "%d", value);
    }
    return ret;
}


static Std_ReturnType lcd_4bit_send(const lcd_4bit_t *lcd, uint8_t command){
    Std_ReturnType ret = E_OK;
    if(NULL == lcd){
        ret = E_NOT_OK;
    }
    else{
        ret = gpio_pin_write_logic(&(lcd->lcd_data[0]), (command >> 0) & 0X01);
        ret = gpio_pin_write_logic(&(lcd->lcd_data[1]), (command >> 1) & 0X01);
        ret = gpio_pin_write_logic(&(lcd->lcd_data[2]), (command >> 2) & 0X01);
        ret = gpio_pin_write_logic(&(lcd->lcd_data[3]), (command >> 3) & 0X01);
    }
    return ret;
}

static Std_ReturnType lcd_4bit_send_enable_signal(const lcd_4bit_t *lcd){
    Std_ReturnType ret = E_OK;
    if(NULL == lcd){
        ret = E_NOT_OK;
    }
    else{
        ret = gpio_pin_write_logic(&(lcd->en_pin), GPIO_HIGH);
        __delay_us(50);
        ret = gpio_pin_write_logic(&(lcd->en_pin), GPIO_LOW);
    }
    return ret;
}

static Std_ReturnType lcd_8bit_send_enable_signal(const lcd_8bit_t *lcd){
    Std_ReturnType ret = E_OK;
    if(NULL == lcd){
        ret = E_NOT_OK;
    }
    else{
        ret = gpio_pin_write_logic(&(lcd->en_pin), GPIO_HIGH);
        __delay_us(50);
        ret = gpio_pin_write_logic(&(lcd->en_pin), GPIO_LOW);
    }
    return ret;
}

static Std_ReturnType lcd_8bit_set_cursor(const lcd_8bit_t *lcd, uint8_t row, uint8_t column){
    Std_ReturnType ret = E_OK;
    if(NULL == lcd){
        ret = E_NOT_OK;
    }
    else{
        column--;
        switch(row){
            case ROW1: 
                ret = lcd_8bit_send_command(lcd,(0X80 + column));
                break;
            case ROW2: 
                ret = lcd_8bit_send_command(lcd,(0XC0 + column));
                break;
            case ROW3: 
                ret = lcd_8bit_send_command(lcd,(0X94 + column));
                break;    
            case ROW4: 
                ret = lcd_8bit_send_command(lcd,(0XD4 + column));
                break;
        }
    }
    return ret;
}
static Std_ReturnType lcd_4bit_set_cursor(const lcd_8bit_t *lcd, uint8_t row, uint8_t column){
    Std_ReturnType ret = E_OK;
    if(NULL == lcd){
        ret = E_NOT_OK;
    }
    else{
        column--;
        switch(row){
            case ROW1: 
                ret = lcd_4bit_send_command(lcd,(0X80 + column));
                break;
            case ROW2: 
                ret = lcd_4bit_send_command(lcd,(0XC0 + column));
                break;
            case ROW3: 
                ret = lcd_4bit_send_command(lcd,(0X94 + column));
                break;    
            case ROW4: 
                ret = lcd_4bit_send_command(lcd,(0XD4 + column));
                break;
        }
    }
    return ret;
}