/* 
 * File:   hal_timer3.h
 * Author: Mohamed Elsayed
 *
 * Created on February 17, 2024, 8:22 PM
 */

#ifndef HAL_TIMER3_H
#define	HAL_TIMER3_H

/*SECTION : INCLUDES*/
#include "../../GPIO/hal_gpio.h"
#include "../../INTERRUPT/mcal_internal_interrupt.h"

/* SECTION : MACROS DECLARATION*/

/*
 * TMR3ON: Timer3 On bit
 * 1 = Enables Timer1
 * 0 = Stops Timer1
 */
#define TIMER3_ENABLE                            0X01
#define TIMER3_DISABLE                           0X00

/*
 * RD16: 16-Bit Read/Write Mode Enable bit
 * 1 = Enables register read/write of TImer1 in one 16-bit operation
 * 0 = Enables register read/write of Timer1 in two 8-bit operations
 */
#define TIMER3_8BIT_READ_WRITE_MODE              0X00
#define TIMER3_16BIT_READ_WRITE_MODE             0X01

/*
 * TMR3CS: Timer3 Clock Source Select bit
 * 1 = External clock from pin RC0/T1OSO/T13CKI (on the rising edge)
 * 0 = Internal clock (FOSC/4)
 */
#define TIMER3_EXTERNAL_CLK                      0X01  // clock source is connected to RC0/T1OSO/T13CKI (on the rising edge)
#define TIMER3_INTERNAL_CLK                      0X00

/*
 * T1SYNC: Timer3 External Clock Input Synchronization Select bit When TMR1CS = 1:
 * 1 = Do not synchronize external clock input
 * 0 = Synchronize external clock input
 * When TMR1CS = 0:
 * This bit is ignored. Timer1 uses the internal clock when TMR1CS = 0
 */
#define TIMER3_SYNC_COUNTER_MODE                 0X00
#define TIMER3_ASYNC_COUNTER_MODE                0X01


/*SECTION : MACRO FUNCTIONS*/
#define TIMER3_ENABLE_FEATURE()                   (T3CONbits.TMR3ON = TIMER3_ENABLE)
#define TIMER3_DISABLE_FEATURE()                  (T3CONbits.TMR3ON = TIMER3_DISABLE)

#define TIMER3_8BIT_READ_WRITE_ENABLE()           (T3CONbits.T3RD16 = TIMER3_8BIT_READ_WRITE_MODE)
#define TIMER3_16BIT_READ_WRITE_ENABLE()          (T3CONbits.T3RD16 = TIMER3_16BIT_READ_WRITE_MODE)

#define TIMER3_COUNTER_MODE()                     (T3CONbits.TMR3CS = TIMER3_EXTERNAL_CLK)
#define TIMER3_TIMER_MODE()                       (T3CONbits.TMR3CS = TIMER3_INTERNAL_CLK)

#define TIMER3_SYNC_COUNTER_ENABLE()              (T3CONbits.T3SYNC = TIMER3_SYNC_COUNTER_MODE)
#define TIMER3_ASYNC_COUNTER_ENABLE()             (T3CONbits.T3SYNC = TIMER3_ASYNC_COUNTER_MODE)

#define TIMER3_PRESCALER_SELECT(_PRESCALER)       (T3CONbits.T3CKPS = _PRESCALER)
/*SECTION : DATA TYPE DECLARATION*/

/*
 * T1CKPS1:T1CKPS0: Timer3 Input Clock Prescale Select bits
 * 11 = 1:8 Prescale value
 * 10 = 1:4 Prescale value
 * 01 = 1:2 Prescale value
 * 00 = 1:1 Prescale value
 */
typedef enum{
    TIMER3_PRESCALER_DIV_BY_1,
    TIMER3_PRESCALER_DIV_BY_2,
    TIMER3_PRESCALER_DIV_BY_4,
    TIMER3_PRESCALER_DIV_BY_8,         
}timer3_prescaler_t;

typedef struct{
    void (*Timer3_InterruptHandler)(void);
    uint16_t preloaded_value;
    timer3_prescaler_t prescaler_value;
    uint8_t timer3_priority : 1;
    uint8_t timer3_mode : 1;
    uint8_t timer3_rw_register_size : 1;
}timer3_t;

typedef void (*InterruptHandler)(void);

/*FUNCTIONS DECLARATIONS*/
Std_ReturnType Timer3_Init(const timer3_t *timer3);
Std_ReturnType Timer3_Deinit(const timer3_t *timer3);
Std_ReturnType Timer3_Write_Value(const timer3_t *timer3, uint16_t value);
Std_ReturnType Timer3_Read_Value(const timer3_t *timer3, uint16_t *value);

#endif	/* HAL_TIMER3_H */


