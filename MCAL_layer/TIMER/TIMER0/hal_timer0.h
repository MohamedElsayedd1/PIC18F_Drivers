/* 
 * File:   hal_timer0.h
 * Author: Mohamed Elsayed
 *
 * Created on February 12, 2024, 6:50 PM
 */

#ifndef HAL_TIMER0_H
#define	HAL_TIMER0_H

/*SECTION : INCLUDES*/
#include "../../GPIO/hal_gpio.h"
#include "../../INTERRUPT/mcal_internal_interrupt.h"

/* SECTION : MACROS DECLARATION*/

/*
 * TMR0ON: Timer0 On/Off Control bit
 * 1 = Enables Timer0
 * 0 = Stops Timer0
 */
#define TIMER0_ENABLE                            0X01
#define TIMER0_DISABLE                           0X00

/*
 * T08BIT: Timer0 8-Bit/16-Bit Control bit
 * 1 = Timer0 is configured as an 8-bit timer/counter
 * 0 = Timer0 is configured as a 16-bit timer/counter
 */
#define TIMER0_8BIT_MODE                         0X01
#define TIMER0_16BIT_MODE                        0X00

/*
 * T0CS: Timer0 Clock Source Select bit
 * 1 = Transition on T0CKI pin
 * 0 = Internal instruction cycle clock (CLKO)
 */
#define TIMER0_EXTERNAL_CLK                      0X01  // clock source is connected to T0CKI pin (Push Button - Sensor - Encoder)
#define TIMER0_INTERNAL_CLK                      0X00

/*
 * T0SE: Timer0 Source Edge Select bit
 * 1 = Increment on high-to-low transition on T0CKI pin
 * 0 = Increment on low-to-high transition on T0CKI pin
 */
#define TIMER0_FALLING_EDGE                      0X01
#define TIMER0_RISING_EDGE                       0X00

/*
 * PSA: Timer0 Prescaler Assignment bit
 * 1 = TImer0 prescaler is not assigned. Timer0 clock input bypasses prescaler.
 * 0 = Timer0 prescaler is assigned. Timer0 clock input comes from prescaler output.
 */
#define TIMER0_PRESCALER_VALUE_ENABLE            0X00
#define TIMER0_PRESCALER_VALUE_DISABLE           0X01

/*SECTION : MACRO FUNCTIONS*/
#define TIMER0_ENABLE_FEATURE()                   (T0CONbits.TMR0ON = TIMER0_ENABLE)
#define TIMER0_DISABLE_FEATURE()                  (T0CONbits.TMR0ON = TIMER0_DISABLE)

#define TIMER0_8BIT_FEATURE()                     (T0CONbits.T08BIT = TIMER0_8BIT_MODE)
#define TIMER0_16BIT_FEATURE()                    (T0CONbits.T08BIT = TIMER0_16BIT_MODE)

#define TIMER0_COUNTER_MODE()                     (T0CONbits.T0CS = TIMER0_EXTERNAL_CLK)
#define TIMER0_TIMER_MODE()                       (T0CONbits.T0CS = TIMER0_INTERNAL_CLK)

#define TIMER0_FALLING_EDGE_ENABLE()              (T0CONbits.T0SE = TIMER0_FALLING_EDGE)
#define TIMER0_RISING_EDGE_ENABLE()               (T0CONbits.T0SE = TIMER0_RISING_EDGE)

#define TIMER0_PRESCALER_ENABLE()                 (T0CONbits.PSA = TIMER0_PRESCALER_VALUE_ENABLE)
#define TIMER0_PRESCALER_DISABLE()                (T0CONbits.PSA = TIMER0_PRESCALER_VALUE_DISABLE)

/*SECTION : DATA TYPE DECLARATION*/
typedef enum{
    TIMER0_PRESCALER_DIV_BY_2,
    TIMER0_PRESCALER_DIV_BY_4,
    TIMER0_PRESCALER_DIV_BY_8,
    TIMER0_PRESCALER_DIV_BY_16,
    TIMER0_PRESCALER_DIV_BY_32,
    TIMER0_PRESCALER_DIV_BY_64,
    TIMER0_PRESCALER_DIV_BY_128,
    TIMER0_PRESCALER_DIV_BY_256,            
}timer0_prescaler_t;

typedef struct{
    void (*Timer0_InterruptHandler)(void);
    timer0_prescaler_t prescaler_value;
    uint16_t preloaded_value;
    uint8_t timer0_priority : 1;
    uint8_t timer0_mode : 1;
    uint8_t prescaler_status : 1;
    uint8_t timer0_register_size : 1;
    uint8_t timer0_counter_edge : 1;
}timer0_t;

typedef void (*InterruptHandler)(void);

/*FUNCTIONS DECLARATIONS*/
Std_ReturnType Timer0_Init(const timer0_t *timer0);
Std_ReturnType Timer0_Deinit(const timer0_t *timer0);
Std_ReturnType Timer0_Write_Value(const timer0_t *timer0, uint16_t value);
Std_ReturnType Timer0_Read_Value(const timer0_t *timer0, uint16_t *value);

#endif	/* HAL_TIMER0_H */

