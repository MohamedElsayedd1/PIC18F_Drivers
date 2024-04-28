/* 
 * File:   hal_timer0.c
 * Author: Mohamed Elsayed
 *
 * Created on February 12, 2024, 6:50 PM
 */

#include "hal_timer0.h"

#if TIMER0_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
static InterruptHandler Timer0_InterruptHandler = NULL;
#endif

static volatile uint16_t timer0_preloaded = 0;

static void Timer0_Prescaler_Config(const timer0_t *timer0);
static void Timer0_Register_Size_Config(const timer0_t *timer0);
static void Timer0_Mode_Select(const timer0_t *timer0);

Std_ReturnType Timer0_Init(const timer0_t *timer0){
    Std_ReturnType ret = E_OK;
    if(timer0 == NULL){
        ret = E_NOT_OK;
    }
    else{
        /*Disable Timer0 feature*/
        TIMER0_DISABLE_FEATURE();
        /*Timer0 Prescaler configurations*/
        Timer0_Prescaler_Config(timer0);
        /*Select the timer/counter mode*/
        Timer0_Mode_Select(timer0);
        /*Timer0 8-Bit/16-Bit Control bit*/
        Timer0_Register_Size_Config(timer0);
        /*Writ the preloaded value into TMR0L/H*/
        TMR0H = (uint8_t)((timer0->preloaded_value) >> 8);
        TMR0L = (uint8_t)(timer0->preloaded_value);
        timer0_preloaded = timer0->preloaded_value;
        /*Enable Timer0 Module interrupt source*/
        #if TIMER0_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
        #if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
        INTERRUPT_InterruptPriorityEnable();
        INTERRUT_GlobalInterruptHighEnable();
        INTERRUT_GlobalInterruptLowEnable();
        /*set Timer0 Module interrupt priority levels*/
        if(timer0->timer0_priority == HIGH_PRIORITY){
            INTERRUPT_TIMER0_HIGH_PRIORITY();
        }
        else if(timer0->timer0_priority == LOW_PRIORITY){
            INTERRUPT_TIMER0_LOW_PRIORITY();
        }
        else{/*Nothing*/}
        INTERRUPT_TIMER0_ENABLE();
        /*clear Timer0 Module external interrupt flag*/ 
        INTERRUPT_TIMER0_CLEAR_FLAG();
        /*Intialize the callback function*/
        Timer0_InterruptHandler = timer0->Timer0_InterruptHandler;
        #else
        INTERRUPT_InterruptPriorityDisable();
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
        INTERRUPT_TIMER0_ENABLE();
        /*clear Timer0 Module external interrupt flag*/
        INTERRUPT_TIMER0_CLEAR_FLAG();
        /*Intialize the callback function*/
        Timer0_InterruptHandler = timer0->Timer0_InterruptHandler;
        #endif
        #endif
        /*Enable Timer0 Feature*/
        TIMER0_ENABLE_FEATURE();
    }
    return ret;
}

Std_ReturnType Timer0_Deinit(const timer0_t *timer0){
    Std_ReturnType ret = E_OK;
    if(timer0 == NULL){
        ret = E_NOT_OK;
    }
    else{
        /*Disable Timer0 feature*/
        TIMER0_DISABLE_FEATURE();
        #if TIMER0_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
        /*Disable Timer0 Module interrupt source*/
        INTERRUPT_TIMER0_DISABLE();
        #endif
    }
    return ret;  
}

Std_ReturnType Timer0_Write_Value(const timer0_t *timer0, uint16_t value){
    Std_ReturnType ret = E_OK;
    if(timer0 == NULL){
        ret = E_NOT_OK;
    }
    else{
        TMR0H = (uint8_t)((value) >> 8);
        TMR0L = (uint8_t)(value);
    }
    return ret;
}

Std_ReturnType Timer0_Read_Value(const timer0_t *timer0, uint16_t *value){
    Std_ReturnType ret = E_OK;
    uint8_t timer0l = 0, timer0h = 0;
    if((timer0 == NULL) || (value == NULL)){
        ret = E_NOT_OK;
    }
    else{
        timer0l = (uint8_t)(TMR0L);
        timer0h = (uint8_t)(TMR0H);
        *value = (uint16_t)((timer0l) + (timer0h << 8));
    }
    return ret;
}

void TMR0_ISR(void){
    INTERRUPT_TIMER0_CLEAR_FLAG();
    /*Write the preloaded value of timer0 into both timer0 registers in case of interrupt*/
    TMR0H = (uint8_t)(timer0_preloaded >> 8);
    TMR0L = (uint8_t)(timer0_preloaded);
    if(Timer0_InterruptHandler){
        Timer0_InterruptHandler();
    }
}

static void Timer0_Prescaler_Config(const timer0_t *timer0){
    if(timer0->prescaler_status == TIMER0_PRESCALER_VALUE_ENABLE){
        TIMER0_PRESCALER_ENABLE();
        T0CONbits.T0PS = timer0->prescaler_value;
    }
    else if(timer0->prescaler_status == TIMER0_PRESCALER_VALUE_DISABLE){
        TIMER0_PRESCALER_DISABLE();
    }
    else{/*Nothing*/}
}

static void Timer0_Register_Size_Config(const timer0_t *timer0){
    if(timer0->timer0_register_size == TIMER0_8BIT_MODE){
        TIMER0_8BIT_FEATURE();
    }
    else if(timer0->timer0_register_size == TIMER0_16BIT_MODE){
        TIMER0_16BIT_FEATURE();
    }
    else{/*Nothing*/}
}

static void Timer0_Mode_Select(const timer0_t *timer0){
    if(timer0->timer0_mode == TIMER0_EXTERNAL_CLK){
        TIMER0_COUNTER_MODE();
    }
    else if(timer0->timer0_mode == TIMER0_INTERNAL_CLK){
        TIMER0_TIMER_MODE();
        if(timer0->timer0_counter_edge == TIMER0_RISING_EDGE){
            TIMER0_RISING_EDGE_ENABLE();
        }
        else if(timer0->timer0_counter_edge == TIMER0_FALLING_EDGE){
            TIMER0_FALLING_EDGE_ENABLE();
        }
        else{/*Nothing*/}
    }
    else{/*Nothing*/}
}

