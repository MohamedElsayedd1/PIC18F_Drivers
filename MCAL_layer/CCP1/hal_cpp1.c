/* 
 * File:   hal_ccp1.c
 * Author: Mohamed Elsayed 
 *
 * Created on February 21, 2024, 11:51 AM
 */

#include "hal_ccp1.h"

#if CCP1_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
static InterruptHandler CCP1_InterruptHandler = NULL;
#endif

static Std_ReturnType CCP1_CaptureTimerSelector(const ccp1_t *ccp1_obj);
static Std_ReturnType CCP1_CompareTimerSelector(const ccp1_t *ccp1_obj);

Std_ReturnType CCP1_DeInit(const ccp1_t *ccp1_obj){
    Std_ReturnType ret = E_OK;
    if(NULL == ccp1_obj){
        ret = E_NOT_OK;
    }
    else{
        /*Disable CCP1 Module*/
        CCP1_SET_MODE(CCP1_MODULE_DISABLE);
        /*Disable Interrupt*/
        #if CCP1_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
            INTERRUPT_CCP1_DISABLE();
        #endif
    }
    return ret;
}

#if (CCP1_CFG_SELECTED_MODE == CCP1_CFG_CAPTURE_MODE)
Std_ReturnType CCP1_Capture_Init(const ccp1_t *ccp1_obj){
    Std_ReturnType ret = E_OK;
    if(NULL == ccp1_obj){
        ret = E_NOT_OK;
    }
    else{
        /*Disable CCP1 Module*/
        CCP1_SET_MODE(CCP1_MODULE_DISABLE);
        /*Pin Configuration : Set RC2/CCP1x pin as input*/
        TRISCbits.RC2 = 1;
        /*Select Timer for CCP1 Capture mode*/
        CCP1_CaptureTimerSelector(ccp1_obj);
        /*Enable CCP1 Module interrupt source*/
        #if CCP1_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
        #if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
        INTERRUPT_InterruptPriorityEnable();
        INTERRUT_GlobalInterruptHighEnable();
        INTERRUT_GlobalInterruptLowEnable();
        /*set CCP1 Module interrupt priority levels*/
        if(ccp1_obj->ccp1_priority == HIGH_PRIORITY){
            INTERRUPT_CCP1_HIGH_PRIORITY();
        }
        else if(ccp1_obj->ccp1_priority == LOW_PRIORITY){
            INTERRUPT_CCP1_LOW_PRIORITY();
        }
        else{/*Nothing*/}
        INTERRUPT_CCP1_ENABLE();
        /*clear CCP1 Module external interrupt flag*/ 
        INTERRUPT_CCP1_CLEAR_FLAG();
        /*Intialize the callback function*/
        CCP1_InterruptHandler = ccp1_obj->CCP1_InterruptHandler;
        #else
        INTERRUPT_InterruptPriorityDisable();
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
        INTERRUPT_CCP1_ENABLE();
        /*clear CCP1 Module external interrupt flag*/
        INTERRUPT_CCP1_CLEAR_FLAG();
        /*Intialize the callback function*/
        CCP1_InterruptHandler = ccp1_obj->CCP1_InterruptHandler;
        #endif
        #endif
        /*CCP1 Module Capture Mode Intialization*/
        if(ccp1_obj->ccp1_mode == CCP1_CFG_CAPTURE_MODE){
            switch(ccp1_obj->ccp1_capture_mode){
                case CCP1_CAPTURE_MODE_1_FALLING_EDGE :
                    CCP1_SET_MODE(CCP1_CAPTURE_MODE_1_FALLING_EDGE);
                break;
                case CCP1_CAPTURE_MODE_1_RISING_EDGE :
                    CCP1_SET_MODE(CCP1_CAPTURE_MODE_1_RISING_EDGE);
                break;
                case CCP1_CAPTURE_MODE_4_RISING_EDGE :
                    CCP1_SET_MODE(CCP1_CAPTURE_MODE_4_RISING_EDGE);
                break;
                case CCP1_CAPTURE_MODE_16_RISING_EDGE :
                    CCP1_SET_MODE(CCP1_CAPTURE_MODE_16_RISING_EDGE);
                break;
                default: ret = E_NOT_OK;
            }
        }
        else{
            ret = E_NOT_OK;
        }
    }
    return ret;
}

Std_ReturnType CCP1_IsCaptureComplete(uint8_t *_capture_status){
    Std_ReturnType ret = E_OK;
    if(NULL == _capture_status){
        ret = E_NOT_OK;
    }
    else{
        if(PIR1bits.CCP1IF == CCP1_CAPTURE_READY){
            *_capture_status = CCP1_CAPTURE_READY;
            INTERRUPT_CCP1_CLEAR_FLAG();
        }
        else if(PIR1bits.CCP1IF == CCP1_CAPTURE_NOT_READY){
            *_capture_status = CCP1_CAPTURE_NOT_READY;
        }
    }
    return ret;
}

Std_ReturnType CCP1_Capture_Read_Value(uint16_t *_capture_value){
    Std_ReturnType ret = E_OK;
    CCP1_REG_T ccp1_temp_reg = {.ccpr1_l = 0, .ccpr1_h = 0}; 
    if(NULL == _capture_value){
        ret = E_NOT_OK;
    }
    else{
        ccp1_temp_reg.ccpr1_l = CCPR1L;
        ccp1_temp_reg.ccpr1_h = CCPR1H;
        *_capture_value = ccp1_temp_reg.ccpr1;
    }
    return ret;
}

static Std_ReturnType CCP1_CaptureTimerSelector(const ccp1_t *ccp1_obj){
    Std_ReturnType ret = E_OK;
    if(NULL == ccp1_obj){
        ret = E_NOT_OK;
    }
    else{
        if(CCP1_CCP2_TIMER1 == ccp1_obj->capture_timer_select){
            T3CONbits.T3CCP1 = 0;
            T3CONbits.T3CCP2 = 0;
        }
        else if(CCP1_TIMER1_CCP2_TIMER3 == ccp1_obj->capture_timer_select){
            T3CONbits.T3CCP1 = 1;
            T3CONbits.T3CCP2 = 0;
        }
        else if(CCP1_CCP2_TIMER3 == ccp1_obj->capture_timer_select){
            T3CONbits.T3CCP1 = 1;
            T3CONbits.T3CCP2 = 1;
        }
        else{/*Nothing*/}
    }
    return ret;
}

#endif
#if (CCP1_CFG_SELECTED_MODE == CCP1_CFG_COMPARE_MODE)
Std_ReturnType CCP1_Compare_Init(const ccp1_t *ccp1_obj){
    Std_ReturnType ret = E_OK;
    if(NULL == ccp1_obj){
        ret = E_NOT_OK;
    }
    else{
        /*Disable CCP1 Module*/
        CCP1_SET_MODE(CCP1_MODULE_DISABLE);
        /*Pin Configuration : Set RC2/CCP1x pin as output*/
        TRISCbits.RC2 = 0;
        /*Select Timer for CCP1 Compare mode*/
        CCP1_CompareTimerSelector(ccp1_obj);
        /*Enable CCP1 Module interrupt source*/
        #if CCP1_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
        #if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
        INTERRUPT_InterruptPriorityEnable();
        INTERRUT_GlobalInterruptHighEnable();
        INTERRUT_GlobalInterruptLowEnable();
        /*set CCP1 Module interrupt priority levels*/
        if(ccp1_obj->ccp1_priority == HIGH_PRIORITY){
            INTERRUPT_CCP1_HIGH_PRIORITY();
        }
        else if(ccp1_obj->ccp1_priority == LOW_PRIORITY){
            INTERRUPT_CCP1_LOW_PRIORITY();
        }
        else{/*Nothing*/}
        INTERRUPT_CCP1_ENABLE();
        /*clear CCP1 Module external interrupt flag*/ 
        INTERRUPT_CCP1_CLEAR_FLAG();
        /*Intialize the callback function*/
        CCP1_InterruptHandler = ccp1_obj->CCP1_InterruptHandler;
        #else
        INTERRUPT_InterruptPriorityDisable();
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
        INTERRUPT_CCP1_ENABLE();
        /*clear CCP1 Module external interrupt flag*/
        INTERRUPT_CCP1_CLEAR_FLAG();
        /*Intialize the callback function*/
        CCP1_InterruptHandler = ccp1_obj->CCP1_InterruptHandler;
        #endif
        #endif
        /*Write Compare Value into CCPRL/H registers*/
        CCP1_Compare_Write_Value(ccp1_obj->Compare_Value);
        /*CCP1 Module Compare Mode Intialization*/
        if(ccp1_obj->ccp1_mode == CCP1_CFG_COMPARE_MODE){
            switch(ccp1_obj->ccp1_compare_mode){
                case CCP1_COMPARE_MODE_SET_PIN_HIGH :
                    CCP1_SET_MODE(CCP1_COMPARE_MODE_SET_PIN_HIGH);
                break;
                case CCP1_COMPARE_MODE_SET_PIN_LOW :
                    CCP1_SET_MODE(CCP1_COMPARE_MODE_SET_PIN_LOW);
                break;
                case CCP1_COMPARE_MODE_TOGGLE_PIN :
                    CCP1_SET_MODE(CCP1_COMPARE_MODE_TOGGLE_PIN);
                break;
                case CCP1_COMPARE_MODE_SW_INTERRUPT :
                    CCP1_SET_MODE(CCP1_COMPARE_MODE_SW_INTERRUPT);
                break;
                case CCP1_COMPARE_MODE_GEN_EVENT :
                    CCP1_SET_MODE(CCP1_COMPARE_MODE_GEN_EVENT);
                break;
                default: ret = E_NOT_OK;
            }
        }
        else{
            ret = E_NOT_OK;
        }
    }
    return ret;
}

Std_ReturnType CCP1_IsCompareComplete(uint8_t *_compare_status){
    Std_ReturnType ret = E_OK;
    if(NULL == _compare_status){
        ret = E_NOT_OK;
    }
    else{
        if(PIR1bits.CCP1IF == CCP1_COMPARE_READY){
            *_compare_status = CCP1_COMPARE_READY;
            INTERRUPT_CCP1_CLEAR_FLAG();
        }
        else if(PIR1bits.CCP1IF == CCP1_COMPARE_NOT_READY){
            *_compare_status = CCP1_COMPARE_NOT_READY;
        }
    }
    return ret;
}

Std_ReturnType CCP1_Compare_Write_Value(uint16_t _compare_value){
    Std_ReturnType ret = E_OK;
    CCP1_REG_T ccp1_temp_reg = {.ccpr1_l = 0, .ccpr1_h = 0}; 
    ccp1_temp_reg.ccpr1 = _compare_value;
    CCPR1L = ccp1_temp_reg.ccpr1_l;
    CCPR1H = ccp1_temp_reg.ccpr1_h;
    ret = E_NOT_OK;
    return ret;
}

Std_ReturnType CCP1_CompareTimerSelector(const ccp1_t *ccp1_obj){
    Std_ReturnType ret = E_OK;
    if(NULL == ccp1_obj){
        ret = E_NOT_OK;
    }
    else{
        if(CCP1_CCP2_TIMER1 == ccp1_obj->compare_timer_select){
            T3CONbits.T3CCP1 = 0;
            T3CONbits.T3CCP2 = 0;
        }
        else if(CCP1_TIMER1_CCP2_TIMER3 == ccp1_obj->compare_timer_select){
            T3CONbits.T3CCP1 = 1;
            T3CONbits.T3CCP2 = 0;
        }
        else if(CCP1_CCP2_TIMER3 == ccp1_obj->compare_timer_select){
            T3CONbits.T3CCP1 = 1;
            T3CONbits.T3CCP2 = 1;
        }
        else{/*Nothing*/}
    }
    return ret;
}

#endif
#if (CCP1_CFG_SELECTED_MODE == CCP1_CFG_PWM_MODE)
Std_ReturnType CCP1_PWM_Init(const ccp1_t *ccp1_obj){
    Std_ReturnType ret = E_OK;
    uint16_t pwm_temp_dutycucle = 0;
    if(NULL == ccp1_obj){
        ret = E_NOT_OK;
    }
    else{
        /*Disable CCP1 Module*/
        CCP1_SET_MODE(CCP1_MODULE_DISABLE);
        /*Pin Configuration : Set RC2/CCP1x pin as output*/
        TRISCbits.RC2 = 0;
        /*Set the period of the generated pulse*/
        PR2 = (uint8_t)((_XTAL_FREQ)/(ccp1_obj->pwm_frequency * 4 * (ccp1_obj->timer2_prescaler_value + 1) * (ccp1_obj->timer2_postscaler_value + 1)) - 1);
        /*Set the DutyCycle of the generated pulse*/
        pwm_temp_dutycucle = (uint16_t)((PR2+1)*(ccp1_obj->pwm_DutyCycle/100.0)*4);
        CCP1CONbits.DC1B = (pwm_temp_dutycucle & 0x0003);
        CCPR1L = (uint8_t)(ccp1_obj->pwm_DutyCycle);
        /*CCP1 Module PMW Mode Intialization*/
        if(ccp1_obj->ccp1_mode == CCP1_CFG_PWM_MODE){
            CCP1_SET_MODE(CCP1_PWM_MODE);
        }
        else{
            ret = E_NOT_OK;
        }
    }
    return ret;
}

Std_ReturnType CCP1_PWM_Change_DutyCyle(const uint8_t _duty){
    Std_ReturnType ret = E_OK;
    uint16_t pwm_temp_dutycucle = 0;
    pwm_temp_dutycucle = (uint16_t)((PR2+1)*(_duty/100.0)*4);
    CCP1CONbits.DC1B = (pwm_temp_dutycucle & 0x03);
    CCPR1L = (uint8_t)(pwm_temp_dutycucle >> 2);
    ret = E_NOT_OK;
    return ret;
}

Std_ReturnType CCP1_PWM_Start(void){
    Std_ReturnType ret = E_OK;
    /*Start the PWM Mode*/
    CCP1_SET_MODE(CCP1_PWM_MODE);
    ret = E_NOT_OK;
    return ret;
}

Std_ReturnType CCP1_PWM_Stop(void){
    Std_ReturnType ret = E_OK;
    /*Stop the PWM Mode*/
    CCP1_SET_MODE(CCP1_MODULE_DISABLE);
    ret = E_NOT_OK;
    return ret;
}
#endif

void CCP1_ISR(void){
    INTERRUPT_CCP1_CLEAR_FLAG();
    if(CCP1_InterruptHandler){
        CCP1_InterruptHandler();
    }
}