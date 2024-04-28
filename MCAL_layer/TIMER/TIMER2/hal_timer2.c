/* 
 * File:   hal_timer2.h
 * Author: Mohamed Elsayed
 *
 * Created on February 16, 2024, 8:22 PM
 */

#include "hal_timer2.h"

#if TIMER2_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
static InterruptHandler Timer2_InterruptHandler = NULL;
#endif

static volatile uint8_t timer2_preloaded = 0;

Std_ReturnType Timer2_Init(const timer2_t *timer2){
    Std_ReturnType ret = E_OK;
    if(timer2 == NULL){
        ret = E_NOT_OK;
    }
    else{
        /*Disable Timer2 feature*/
        TIMER2_DISABLE_FEATURE();
        /*Timer2 Prescaler configurations*/
        TIMER2_PRESCALER_SELECT(timer2->prescaler_value);
        /*Timer2 Postscaler configurations*/
        TIMER2_POSTSCALER_SELECT(timer2->postscaler_value);
        TMR2 = timer2->preloaded_value;
        /*Write the preloaded value into TMR0L/H*/
        timer2_preloaded = timer2->preloaded_value;
        /*Enable Timer2 Module interrupt source*/
        #if TIMER2_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
        #if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
        INTERRUPT_InterruptPriorityEnable();
        INTERRUT_GlobalInterruptHighEnable();
        INTERRUT_GlobalInterruptLowEnable();
        /*set Timer0 Module interrupt priority levels*/
        if(timer2->timer2_priority == HIGH_PRIORITY){
            INTERRUPT_TIMER2_HIGH_PRIORITY();
        }
        else if(timer2->timer2_priority == LOW_PRIORITY){
            INTERRUPT_TIMER2_LOW_PRIORITY();
        }
        else{/*Nothing*/}
        INTERRUPT_TIMER2_ENABLE();
        /*clear Timer0 Module external interrupt flag*/ 
        INTERRUPT_TIMER2_CLEAR_FLAG();
        /*Intialize the callback function*/
        Timer2_InterruptHandler = timer2->Timer2_InterruptHandler;
        #else
        INTERRUPT_InterruptPriorityDisable();
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
        INTERRUPT_TIMER2_ENABLE();
        /*clear Timer2 Module external interrupt flag*/
        INTERRUPT_TIMER2_CLEAR_FLAG();
        /*Intialize the callback function*/
        Timer2_InterruptHandler = timer2->Timer2_InterruptHandler;
        #endif
        #endif
        TIMER2_ENABLE_FEATURE();
    }
    return ret;
}

Std_ReturnType Timer2_Deinit(const timer2_t *timer2){
    Std_ReturnType ret = E_OK;
    if(timer2 == NULL){
        ret = E_NOT_OK;
    }
    else{
        /*Disable Timer2 feature*/
        TIMER2_DISABLE_FEATURE();
        #if TIMER2_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
        /*Disable Timer2 Module interrupt source*/
        INTERRUPT_TIMER2_DISABLE();
        #endif
    }
    return ret;

}

Std_ReturnType Timer2_Write_Value(const timer2_t *timer2, uint16_t value){
    Std_ReturnType ret = E_OK;
    if(timer2 == NULL){
        ret = E_NOT_OK;
    }
    else{
        TMR2 = value;
    }
    return ret;

}

Std_ReturnType Timer2_Read_Value(const timer2_t *timer2, uint16_t *value){
    Std_ReturnType ret = E_OK;
    if((timer2 == NULL) || (value == NULL)){
        ret = E_NOT_OK;
    }
    else{
        *value = TMR2;
    }
    return ret;

}

void TMR2_ISR(void){
    INTERRUPT_TIMER2_CLEAR_FLAG();
    /*Write the preloaded value of timer2 into both timer0 registers in case of interrupt*/
    TMR2 = timer2_preloaded;
    if(Timer2_InterruptHandler){
        Timer2_InterruptHandler();
    }
}