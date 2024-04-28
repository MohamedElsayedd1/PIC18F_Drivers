/* 
 * File:   hal_timer3.h
 * Author: Mohamed Elsayed
 *
 * Created on February 17, 2024, 8:22 PM
 */

#include "hal_timer3.h"

#if TIMER3_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
static InterruptHandler Timer3_InterruptHandler = NULL;
#endif

static volatile uint16_t timer3_preloaded = 0;

static void Timer3_RW_Register_Size_Config(const timer3_t *timer3);
static void Timer3_Mode_Select(const timer3_t *timer3);

Std_ReturnType Timer3_Init(const timer3_t *timer3){
    /*Disable Timer3 feature*/
    TIMER3_DISABLE_FEATURE();
    /*Timer3 Prescaler configurations*/
    TIMER3_PRESCALER_SELECT(timer3->prescaler_value);
    /*Select the timer/counter mode*/
    Timer3_Mode_Select(timer3);
    /*Timer3 8-Bit/16-Bit Control bit*/
    Timer3_RW_Register_Size_Config(timer3);
    /*Write the preloaded value into TMR0L/H*/
    TMR3H = (uint8_t)((timer3->preloaded_value) >> 8);
    TMR3L = (uint8_t)(timer3->preloaded_value);
    timer3_preloaded = timer3->preloaded_value;
    /*Enable Timer3 Module interrupt source*/
    #if TIMER3_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
    #if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
    INTERRUPT_InterruptPriorityEnable();
    INTERRUT_GlobalInterruptHighEnable();
    INTERRUT_GlobalInterruptLowEnable();
    /*set Timer3 Module interrupt priority levels*/
    if(timer3->timer3_priority == HIGH_PRIORITY){
        INTERRUPT_TIMER3_HIGH_PRIORITY();
    }
    else if(timer3->timer3_priority == LOW_PRIORITY){
        INTERRUPT_TIMER3_LOW_PRIORITY();
    }
    else{/*Nothing*/}
    INTERRUPT_TIMER3_ENABLE();
    /*clear Timer0 Module external interrupt flag*/ 
    INTERRUPT_TIMER3_CLEAR_FLAG();
    /*Intialize the callback function*/
    Timer3_InterruptHandler = timer3->Timer3_InterruptHandler;
    #else
    INTERRUPT_InterruptPriorityDisable();
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();
    INTERRUPT_TIMER3_ENABLE();
    /*clear Timer3 Module external interrupt flag*/
    INTERRUPT_TIMER3_CLEAR_FLAG();
    /*Intialize the callback function*/
    Timer3_InterruptHandler = timer3->Timer3_InterruptHandler;
    #endif
    #endif
    /*Enable Timer3 Feature*/
    TIMER3_ENABLE_FEATURE();
}

Std_ReturnType Timer3_Deinit(const timer3_t *timer3){
    Std_ReturnType ret = E_OK;
    if(timer3 == NULL){
        ret = E_NOT_OK;
    }
    else{
        /*Disable Timer3 feature*/
        TIMER3_DISABLE_FEATURE();
        #if TIMER3_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
        /*Disable Timer3 Module interrupt source*/
        INTERRUPT_TIMER3_DISABLE();
        #endif
    }
    return ret;
}

Std_ReturnType Timer3_Write_Value(const timer3_t *timer3, uint16_t value){
    Std_ReturnType ret = E_OK;
    if(timer3 == NULL){
        ret = E_NOT_OK;
    }
    else{
        TMR3H = (uint8_t)((value) >> 8);
        TMR3L = (uint8_t)(value);
    }
    return ret;
}

Std_ReturnType Timer3_Read_Value(const timer3_t *timer3, uint16_t *value){
    Std_ReturnType ret = E_OK;
    uint8_t timer3l = 0, timer3h = 0;
    if(timer3 == NULL){
        ret = E_NOT_OK;
    }
    else{
        timer3l = (uint8_t)(TMR3L);
        timer3h = (uint8_t)(TMR0H);
        *value = (uint16_t)(TMR3L +(TMR3H << 8));
    }
    return ret;
}

static void Timer3_RW_Register_Size_Config(const timer3_t *timer3){
    if(timer3->timer3_rw_register_size == TIMER3_8BIT_READ_WRITE_MODE){
        TIMER3_8BIT_READ_WRITE_ENABLE();
    }
    else if(timer3->timer3_rw_register_size == TIMER3_16BIT_READ_WRITE_MODE){
        TIMER3_16BIT_READ_WRITE_ENABLE();
    }
    else{/*Nothing*/}
}

static void Timer3_Mode_Select(const timer3_t *timer3){
    if(timer3->timer3_mode == TIMER3_EXTERNAL_CLK){
        TIMER3_COUNTER_MODE();
    }
    else if(timer3->timer3_mode == TIMER3_INTERNAL_CLK){
        TIMER3_TIMER_MODE();
        TIMER3_ASYNC_COUNTER_ENABLE();
    }
    else{/*Nothing*/}
}

void TMR3_ISR(void){
    INTERRUPT_TIMER3_CLEAR_FLAG();
    /*Write the preloaded value of timer0 into both timer3 registers in case of interrupt*/
    TMR3H = (uint8_t)(timer3_preloaded >> 8);
    TMR3L = (uint8_t)(timer3_preloaded);
    if(Timer3_InterruptHandler){
        Timer3_InterruptHandler();
    }
}