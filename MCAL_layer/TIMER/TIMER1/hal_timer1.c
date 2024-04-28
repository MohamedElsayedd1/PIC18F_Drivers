/* 
 * File:   hal_timer1.c
 * Author: Mohamed Elsayed
 *
 * Created on February 15, 2024, 7:39 PM
 */
#include "hal_timer1.h"

#if TIMER1_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
static InterruptHandler Timer1_InterruptHandler = NULL;
#endif

static volatile uint16_t timer1_preloaded = 0;

static void Timer1_RW_Register_Size_Config(const timer1_t *timer1);
static void Timer1_Mode_Select(const timer1_t *timer1);

Std_ReturnType Timer1_Init(const timer1_t *timer1){
    Std_ReturnType ret = E_OK;
    if(timer1 == NULL){
        ret = E_NOT_OK;
    }
    else{
        /*Disable Timer1 feature*/
        TIMER1_DISABLE_FEATURE();
        /*Timer1 Prescaler configurations*/
        TIMER1_PRESCALER_SELECT(timer1->prescaler_value);
        /*Select the timer/counter mode*/
        Timer1_Mode_Select(timer1);
        /*Timer0 8-Bit/16-Bit Control bit*/
        Timer1_RW_Register_Size_Config(timer1);
        /*Write the preloaded value into TMR0L/H*/
        TMR1H = (uint8_t)((timer1->preloaded_value) >> 8);
        TMR1L = (uint8_t)(timer1->preloaded_value);
        timer1_preloaded = timer1->preloaded_value;
        /*Enable Timer0 Module interrupt source*/
        #if TIMER1_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
        #if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
        INTERRUPT_InterruptPriorityEnable();
        INTERRUT_GlobalInterruptHighEnable();
        INTERRUT_GlobalInterruptLowEnable();
        /*set Timer0 Module interrupt priority levels*/
        if(timer1->timer1_priority == HIGH_PRIORITY){
            INTERRUPT_TIMER1_HIGH_PRIORITY();
        }
        else if(timer1->timer1_priority == LOW_PRIORITY){
            INTERRUPT_TIMER1_LOW_PRIORITY();
        }
        else{/*Nothing*/}
        INTERRUPT_TIMER1_ENABLE();
        /*clear Timer0 Module external interrupt flag*/ 
        INTERRUPT_TIMER1_CLEAR_FLAG();
        /*Intialize the callback function*/
        Timer1_InterruptHandler = timer1->Timer1_InterruptHandler;
        #else
        INTERRUPT_InterruptPriorityDisable();
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
        INTERRUPT_TIMER1_ENABLE();
        /*clear Timer1 Module external interrupt flag*/
        INTERRUPT_TIMER1_CLEAR_FLAG();
        /*Intialize the callback function*/
        Timer1_InterruptHandler = timer1->Timer1_InterruptHandler;
        #endif
        #endif
        /*Enable Timer0 Feature*/
        TIMER1_ENABLE_FEATURE();
    }
    return ret;
}

Std_ReturnType Timer1_Deinit(const timer1_t *timer1){
    Std_ReturnType ret = E_OK;
    if(timer1 == NULL){
        ret = E_NOT_OK;
    }
    else{
        /*Disable Timer1 feature*/
        TIMER1_DISABLE_FEATURE();
        #if TIMER1_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
        /*Disable Timer1 Module interrupt source*/
        INTERRUPT_TIMER1_DISABLE();
        #endif
    }
    return ret;
}

Std_ReturnType Timer1_Write_Value(const timer1_t *timer1, uint16_t value){
    Std_ReturnType ret = E_OK;
    if(timer1 == NULL){
        ret = E_NOT_OK;
    }
    else{
        TMR1H = (uint8_t)((value) >> 8);
        TMR1L = (uint8_t)(value);
    }
    return ret;
}

Std_ReturnType Timer1_Read_Value(const timer1_t *timer1, uint16_t *value){
    Std_ReturnType ret = E_OK;
    uint8_t timer1l = 0, timer1h = 0;
    if((timer1 == NULL) || (value == NULL)){
        ret = E_NOT_OK;
    }
    else{
        timer1l = (uint8_t)(TMR1L);
        timer1h = (uint8_t)(TMR1H);
        *value = (uint16_t)((timer1l) + (timer1h << 8));
    }
    return ret;
}



static void Timer1_RW_Register_Size_Config(const timer1_t *timer1){
    if(timer1->timer1_rw_register_size == TIMER1_8BIT_READ_WRITE_MODE){
        TIMER1_8BIT_READ_WRITE_ENABLE();
    }
    else if(timer1->timer1_rw_register_size == TIMER1_16BIT_READ_WRITE_MODE){
        TIMER1_16BIT_READ_WRITE_ENABLE();
    }
    else{/*Nothing*/}
}

static void Timer1_Mode_Select(const timer1_t *timer1){
    if(timer1->timer1_mode == TIMER1_EXTERNAL_CLK){
        TIMER1_COUNTER_MODE();
    }
    else if(timer1->timer1_mode == TIMER1_INTERNAL_CLK){
        TIMER1_TIMER_MODE();
        TIMER1_ASYNC_COUNTER_ENABLE();
    }
    else{/*Nothing*/}
}

void TMR1_ISR(void){
    INTERRUPT_TIMER1_CLEAR_FLAG();
    /*Write the preloaded value of timer0 into both timer0 registers in case of interrupt*/
    TMR1H = (uint8_t)(timer1_preloaded >> 8);
    TMR1L = (uint8_t)(timer1_preloaded);
    if(Timer1_InterruptHandler){
        Timer1_InterruptHandler();
    }
}