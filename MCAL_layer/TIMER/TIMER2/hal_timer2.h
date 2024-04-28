/* 
 * File:   hal_timer2.h
 * Author: Mohamed Elsayed
 *
 * Created on February 16, 2024, 8:22 PM
 */

#ifndef HAL_TIMER2_H
#define	HAL_TIMER2_H

/*SECTION : INCLUDES*/
#include "../../GPIO/hal_gpio.h"
#include "../../INTERRUPT/mcal_internal_interrupt.h"

/* SECTION : MACROS DECLARATION*/

/*
 * TMR2ON: Timer2 On bit
 * 1 = Timer2 is on
 * 0 = Timer2 is off
 */
#define TIMER2_ENABLE                            0X01
#define TIMER2_DISABLE                           0X00

/*
 * T2OUTPS3:T2OUTPS0: Timer2 Output Postscale Select bits
 * 0000 = 1:1 Postscale
 * 0001 = 1:2 Postscale
 * ?
 * ?
 * 1111 = 1:16 Postscale
 */
#define TIMER2_POSTSCALER_DIV_BY_1                1
#define TIMER2_POSTSCALER_DIV_BY_2                2
#define TIMER2_POSTSCALER_DIV_BY_3                3
#define TIMER2_POSTSCALER_DIV_BY_4                4
#define TIMER2_POSTSCALER_DIV_BY_5                5
#define TIMER2_POSTSCALER_DIV_BY_6                6
#define TIMER2_POSTSCALER_DIV_BY_7                7
#define TIMER2_POSTSCALER_DIV_BY_8                8
#define TIMER2_POSTSCALER_DIV_BY_9                9
#define TIMER2_POSTSCALER_DIV_BY_10               10
#define TIMER2_POSTSCALER_DIV_BY_11               11
#define TIMER2_POSTSCALER_DIV_BY_12               12
#define TIMER2_POSTSCALER_DIV_BY_13               13
#define TIMER2_POSTSCALER_DIV_BY_14               14
#define TIMER2_POSTSCALER_DIV_BY_15               15
#define TIMER2_POSTSCALER_DIV_BY_16               16

/*
 * T2CKPS1:T2CKPS0: Timer2 Clock Prescale Select bits
 * 00 = Prescaler is 1
 * 01 = Prescaler is 4
 * 1x = Prescaler is 16
 */
#define TIMER2_PRESCALER_DIV_BY_1                 0
#define TIMER2_PRESCALER_DIV_BY_4                 1
#define TIMER2_PRESCALER_DIV_BY_16                2

/*SECTION : MACRO FUNCTIONS*/
#define TIMER2_ENABLE_FEATURE()                   (T2CONbits.TMR2ON = TIMER2_ENABLE)
#define TIMER2_DISABLE_FEATURE()                  (T2CONbits.TMR2ON = TIMER2_DISABLE)

#define TIMER2_PRESCALER_SELECT(_PRESCALER)       (T2CONbits.T2CKPS = _PRESCALER)

#define TIMER2_POSTSCALER_SELECT(_POSTSCALER)     (T2CONbits.TOUTPS = _POSTSCALER)

/*SECTION : DATA TYPE DECLARATION*/
typedef struct{
    void (*Timer2_InterruptHandler)(void);
    uint16_t preloaded_value;
    uint8_t postscaler_value : 4;
    uint8_t prescaler_value : 2;
    uint8_t timer2_priority : 1;
}timer2_t;

typedef void (*InterruptHandler)(void);

/*FUNCTIONS DECLARATIONS*/
Std_ReturnType Timer2_Init(const timer2_t *timer2);
Std_ReturnType Timer2_Deinit(const timer2_t *timer2);
Std_ReturnType Timer2_Write_Value(const timer2_t *timer2, uint16_t value);
Std_ReturnType Timer2_Read_Value(const timer2_t *timer2, uint16_t *value);

#endif	/* HAL_TIMER2_H */

