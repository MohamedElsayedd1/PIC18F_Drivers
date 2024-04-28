/* 
 * File:   hal_timer1.h
 * Author: Mohamed Elsayed
 *
 * Created on February 15, 2024, 7:39 PM
 */

#ifndef HAL_TIMER1_H
#define	HAL_TIMER1_H

/*SECTION : INCLUDES*/
#include "../../GPIO/hal_gpio.h"
#include "../../INTERRUPT/mcal_internal_interrupt.h"

/* SECTION : MACROS DECLARATION*/

/*
 * TMR1ON: Timer1 On bit
 * 1 = Enables Timer1
 * 0 = Stops Timer1
 */
#define TIMER1_ENABLE                            0X01
#define TIMER1_DISABLE                           0X00

/*
 * RD16: 16-Bit Read/Write Mode Enable bit
 * 1 = Enables register read/write of TImer1 in one 16-bit operation
 * 0 = Enables register read/write of Timer1 in two 8-bit operations
 */
#define TIMER1_8BIT_READ_WRITE_MODE              0X00
#define TIMER1_16BIT_READ_WRITE_MODE             0X01

/*
 * TMR1CS: Timer1 Clock Source Select bit
 * 1 = External clock from pin RC0/T1OSO/T13CKI (on the rising edge)
 * 0 = Internal clock (FOSC/4)
 */
#define TIMER1_EXTERNAL_CLK                      0X01  // clock source is connected to RC0/T1OSO/T13CKI (on the rising edge)
#define TIMER1_INTERNAL_CLK                      0X00

/*
 * T1SYNC: Timer1 External Clock Input Synchronization Select bit When TMR1CS = 1:
 * 1 = Do not synchronize external clock input
 * 0 = Synchronize external clock input
 * When TMR1CS = 0:
 * This bit is ignored. Timer1 uses the internal clock when TMR1CS = 0
 */
#define TIMER1_SYNC_COUNTER_MODE                 0X00
#define TIMER1_ASYNC_COUNTER_MODE                0X01

/*
 * T1OSCEN: Timer1 Oscillator Enable bit
 * 1 = Timer1 oscillator is enabled
 * 0 = Timer1 oscillator is shut off
 * The oscillator inverter and feedback resistor are turned off to eliminate power drain.
 */
#define TIMER1_OSCILLATOR_ENABLE                  0X01
#define TIMER1_OSCILLATOR_DISABLE                 0X00

/*
 * Check if the Timer1 clock source is enabled or not
 */
#define TIMER1_CLK_SOURCE_ENABLED                 0X01
#define TIMER1_CLK_SOURCE_ENABLED                 0X00

/*SECTION : MACRO FUNCTIONS*/
#define TIMER1_ENABLE_FEATURE()                   (T1CONbits.TMR1ON = TIMER1_ENABLE)
#define TIMER1_DISABLE_FEATURE()                  (T1CONbits.TMR1ON = TIMER1_DISABLE)

#define TIMER1_8BIT_READ_WRITE_ENABLE()           (T1CONbits.T1RD16 = TIMER1_8BIT_READ_WRITE_MODE)
#define TIMER1_16BIT_READ_WRITE_ENABLE()          (T1CONbits.T1RD16 = TIMER1_16BIT_READ_WRITE_MODE)

#define TIMER1_COUNTER_MODE()                     (T1CONbits.TMR1CS = TIMER1_EXTERNAL_CLK)
#define TIMER1_TIMER_MODE()                       (T1CONbits.TMR1CS = TIMER1_INTERNAL_CLK)

#define TIMER1_OSCILLATOR_ENABLE()                (T1CONbits.T1OSCEN = TIMER1_OSCILLATOR_ENABLE)
#define TIMER1_OSCILLATOR_DISABLE()               (T1CONbits.T1OSCEN = TIMER1_OSCILLATOR_DISABLE)

#define TIMER1_SYNC_COUNTER_ENABLE()              (T1CONbits.T1SYNC = TIMER1_SYNC_COUNTER_MODE)
#define TIMER1_ASYNC_COUNTER_ENABLE()             (T1CONbits.T1SYNC = TIMER1_ASYNC_COUNTER_MODE)

#define TIMER1_SYSTEM_CLK_STATUS()                (T1CONbits.T1RUN)

#define TIMER1_PRESCALER_SELECT(_PRESCALER)       (T1CONbits.T1CKPS = _PRESCALER)

/*SECTION : DATA TYPE DECLARATION*/

/*
 * T1CKPS1:T1CKPS0: Timer1 Input Clock Prescale Select bits
 * 11 = 1:8 Prescale value
 * 10 = 1:4 Prescale value
 * 01 = 1:2 Prescale value
 * 00 = 1:1 Prescale value
 */
typedef enum{
    TIMER1_PRESCALER_DIV_BY_1,
    TIMER1_PRESCALER_DIV_BY_2,
    TIMER1_PRESCALER_DIV_BY_4,
    TIMER1_PRESCALER_DIV_BY_8,         
}timer1_prescaler_t;

typedef struct{
    void (*Timer1_InterruptHandler)(void);
    uint16_t preloaded_value;
    timer1_prescaler_t prescaler_value;
    uint8_t timer1_priority : 1;
    uint8_t timer1_mode : 1;
    uint8_t timer1_rw_register_size : 1;
    uint8_t timer1_osc_enable : 1;
}timer1_t;

typedef void (*InterruptHandler)(void);

/*FUNCTIONS DECLARATIONS*/
Std_ReturnType Timer1_Init(const timer1_t *timer1);
Std_ReturnType Timer1_Deinit(const timer1_t *timer1);
Std_ReturnType Timer1_Write_Value(const timer1_t *timer1, uint16_t value);
Std_ReturnType Timer1_Read_Value(const timer1_t *timer1, uint16_t *value);


#endif	/* HAL_TIMER1_H */

