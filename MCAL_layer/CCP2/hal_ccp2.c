/* 
 * File:   hal_ccp2.c
 * Author: Mohamed Elsayed
 *
 * Created on February 24, 2024, 2:21 PM
 */

#include "hal_ccp2.h"

static void (*CCP2_InterruptHandler)(void) = NULL;

static Std_ReturnType CCP2_CaptureTimerSelector(const ccp2_t *ccp2_obj);
static Std_ReturnType CCP2_CompareTimerSelector(const ccp2_t *ccp2_obj);

Std_ReturnType CCP2_DeInit(const ccp2_t *ccp2_obj){
    Std_ReturnType ret = E_OK;
    if(NULL == ccp2_obj){
        ret = E_NOT_OK;
    }
    else{
        /*Disable CCP2 Module*/
        CCP2_SET_MODE(CCP2_MODULE_DISABLE);
        /*Disable CCP2 Interrupt*/
        #if CCP2_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
            INTERRUPT_CCP2_DISABLE();
        #endif
    }
    return ret;
}

#if (CCP2_CFG_SELECTED_MODE == CCP2_CFG_CAPTURE_MODE)
Std_ReturnType CCP2_Capture_Init(const ccp2_t *ccp2_obj){
    Std_ReturnType ret = E_OK;
    if(NULL == ccp2_obj){
        ret = E_NOT_OK;
    }
    else{
        /*Disable CCP2 Module*/
        CCP2_SET_MODE(CCP2_MODULE_DISABLE);
        /*Pin Configuration : Set RC1/CCP2x pin as input*/
        TRISCbits.RC1 = 1;
        /*Select Timer for CCP2 Capture mode*/
        CCP2_TIMER_SELCET(ccp2_obj->capture_timer_select);
        /*Select Timer for CCP1 Capture mode*/
        CCP2_CaptureTimerSelector(ccp2_obj);
        /*Enable CCP2 Module interrupt source*/
        #if CCP2_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
        #if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
        INTERRUPT_InterruptPriorityEnable();
        INTERRUT_GlobalInterruptHighEnable();
        INTERRUT_GlobalInterruptLowEnable();
        /*set CCP2 Module interrupt priority levels*/
        if(ccp2_obj->ccp2_priority == HIGH_PRIORITY){
            INTERRUPT_CCP2_HIGH_PRIORITY();
        }
        else if(ccp2_obj->ccp2_priority == LOW_PRIORITY){
            INTERRUPT_CCP2_LOW_PRIORITY();
        }
        else{/*Nothing*/}
        INTERRUPT_CCP2_ENABLE();
        /*clear CCP2 Module external interrupt flag*/ 
        INTERRUPT_CCP2_CLEAR_FLAG();
        /*Intialize the callback function*/
        CCP2_InterruptHandler = ccp2_obj->CCP2_InterruptHandler;
        #else
        INTERRUPT_InterruptPriorityDisable();
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
        INTERRUPT_CCP2_ENABLE();
        /*clear CCP2 Module external interrupt flag*/
        INTERRUPT_CCP2_CLEAR_FLAG();
        /*Intialize the callback function*/
        CCP2_InterruptHandler = ccp2_obj->CCP2_InterruptHandler;
        #endif
        #endif
        /*CCP1 Module Capture Mode Intialization*/
        if(ccp2_obj->ccp2_mode == CCP2_CFG_CAPTURE_MODE){
            switch(ccp2_obj->ccp2_capture_mode){
                case CCP2_CAPTURE_MODE_1_FALLING_EDGE :
                    CCP2_SET_MODE(CCP2_CAPTURE_MODE_1_FALLING_EDGE);
                break;
                case CCP2_CAPTURE_MODE_1_RISING_EDGE :
                    CCP2_SET_MODE(CCP2_CAPTURE_MODE_1_RISING_EDGE);
                break;
                case CCP2_CAPTURE_MODE_4_RISING_EDGE :
                    CCP2_SET_MODE(CCP2_CAPTURE_MODE_4_RISING_EDGE);
                break;
                case CCP2_CAPTURE_MODE_16_RISING_EDGE :
                    CCP2_SET_MODE(CCP2_CAPTURE_MODE_16_RISING_EDGE);
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

Std_ReturnType CCP2_IsCaptureComplete(uint8_t *_capture_status){
    Std_ReturnType ret = E_OK;
    if(NULL == _capture_status){
        ret = E_NOT_OK;
    }
    else{
        if(PIR2bits.CCP2IF == CCP2_CAPTURE_READY){
            *_capture_status = CCP2_CAPTURE_READY;
        }
        else if(PIR2bits.CCP2IF == CCP2_CAPTURE_NOT_READY){
            *_capture_status = CCP2_CAPTURE_NOT_READY;
        }
        else{ /*Nothing*/ }
    }
    return ret;
}

Std_ReturnType CCP2_Capture_Read_Value(uint16_t *_capture_value){
    Std_ReturnType ret = E_OK;
    CCP2_REG_T ccp2_temp_reg = {.ccpr2_l = 0, .ccpr2_h = 0};
    if(NULL == _capture_value){
        ret = E_NOT_OK;
    }
    else{
        ccp2_temp_reg.ccpr2_l = CCPR2L; 
        ccp2_temp_reg.ccpr2_h = CCPR2H; 
        *_capture_value = ccp2_temp_reg.ccpr2;
    }
    return ret;
}

static Std_ReturnType CCP2_CaptureTimerSelector(const ccp2_t *ccp2_obj){
    Std_ReturnType ret = E_OK;
    if(NULL == ccp2_obj){
        ret = E_NOT_OK;
    }
    else{
        if(CCP1_CCP2_TIMER1 == ccp2_obj->capture_timer_select){
            T3CONbits.T3CCP1 = 0;
            T3CONbits.T3CCP2 = 0;
        }
        else if(CCP1_TIMER1_CCP2_TIMER3 == ccp2_obj->capture_timer_select){
            T3CONbits.T3CCP1 = 1;
            T3CONbits.T3CCP2 = 0;
        }
        else if(CCP1_CCP2_TIMER3 == ccp2_obj->capture_timer_select){
            T3CONbits.T3CCP1 = 1;
            T3CONbits.T3CCP2 = 1;
        }
        else{/*Nothing*/}
    }
    return ret;
}
#endif

#if (CCP2_CFG_SELECTED_MODE == CCP2_CFG_COMPARE_MODE)
Std_ReturnType CCP2_Compare_Init(const ccp2_t *ccp2_obj){
    Std_ReturnType ret = E_OK;
    if(NULL == ccp2_obj){
        ret = E_NOT_OK;
    }
    else{
        /*Disable CCP2 Module*/
        CCP2_SET_MODE(CCP2_MODULE_DISABLE);
        /*Pin Configuration : Set RC1/CCP2x pin as output*/
        TRISCbits.RC1 = 0;
        /*Select Timer for CCP2 Capture mode*/
        CCP2_TIMER_SELCET(ccp2_obj->compare_timer_select);
        /*Select Timer for CCP1 Compare mode*/
        CCP2_CompareTimerSelector(ccp2_obj);
        /*Enable CCP2 Module interrupt source*/
        #if CCP2_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
        #if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
        INTERRUPT_InterruptPriorityEnable();
        INTERRUT_GlobalInterruptHighEnable();
        INTERRUT_GlobalInterruptLowEnable();
        /*set CCP2 Module interrupt priority levels*/
        if(ccp2_obj->ccp2_priority == HIGH_PRIORITY){
            INTERRUPT_CCP2_HIGH_PRIORITY();
        }
        else if(ccp2_obj->ccp2_priority == LOW_PRIORITY){
            INTERRUPT_CCP2_LOW_PRIORITY();
        }
        else{/*Nothing*/}
        INTERRUPT_CCP2_ENABLE();
        /*clear CCP2 Module external interrupt flag*/ 
        INTERRUPT_CCP2_CLEAR_FLAG();
        /*Intialize the callback function*/
        CCP2_InterruptHandler = ccp2_obj->CCP2_InterruptHandler;
        #else
        INTERRUPT_InterruptPriorityDisable();
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
        INTERRUPT_CCP2_ENABLE();
        /*clear CCP2 Module external interrupt flag*/
        INTERRUPT_CCP2_CLEAR_FLAG();
        /*Intialize the callback function*/
        CCP2_InterruptHandler = ccp2_obj->CCP2_InterruptHandler;
        #endif
        #endif
        /*Write Compare Value into CCPRL/H registers*/
        CCP2_Compare_Write_Value(ccp2_obj->Compare_Value);
        /*CCP1 Module Capture Mode Intialization*/
        if(ccp2_obj->ccp2_mode == CCP2_CFG_COMPARE_MODE){
            switch(ccp2_obj->ccp2_compare_mode){
                case CCP2_COMPARE_MODE_SET_PIN_HIGH :
                    CCP2_SET_MODE(CCP2_COMPARE_MODE_SET_PIN_HIGH);
                break;
                case CCP2_COMPARE_MODE_SET_PIN_LOW :
                    CCP2_SET_MODE(CCP2_COMPARE_MODE_SET_PIN_LOW);
                break;
                case CCP2_COMPARE_MODE_TOGGLE_PIN :
                    CCP2_SET_MODE(CCP2_COMPARE_MODE_TOGGLE_PIN);
                break;
                case CCP2_COMPARE_MODE_SW_INTERRUPT :
                    CCP2_SET_MODE(CCP2_COMPARE_MODE_SW_INTERRUPT);
                break;
                case CCP2_COMPARE_MODE_GEN_EVENT :
                    CCP2_SET_MODE(CCP2_COMPARE_MODE_GEN_EVENT);
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

Std_ReturnType CCP2_IsCompareComplete(uint8_t *_compare_status){
    Std_ReturnType ret = E_OK;
    if(NULL == _compare_status){
        ret = E_NOT_OK;
    }
    else{
        if(PIR2bits.CCP2IF == CCP2_COMPARE_READY){
            *_compare_status = CCP2_COMPARE_READY;
        }
        else if(PIR2bits.CCP2IF == CCP2_COMPARE_NOT_READY){
            *_compare_status = CCP2_COMPARE_NOT_READY;
        }
        else{ /*Nothing*/ }
    }
    return ret;
}

Std_ReturnType CCP2_Compare_Write_Value(uint16_t _compare_value){
    Std_ReturnType ret = E_OK;
    CCP2_REG_T ccp2_temp_reg = {.ccpr2_l = 0, .ccpr2_h = 0};
    ccp2_temp_reg.ccpr2 = _compare_value;
    CCPR2L = ccp2_temp_reg.ccpr2_l;
    CCPR2H = ccp2_temp_reg.ccpr2_h;
    return ret;
} 

static Std_ReturnType CCP2_CompareTimerSelector(const ccp2_t *ccp2_obj){
    Std_ReturnType ret = E_OK;
    if(NULL == ccp2_obj){
        ret = E_NOT_OK;
    }
    else{
        if(CCP1_CCP2_TIMER1 == ccp2_obj->compare_timer_select){
            T3CONbits.T3CCP1 = 0;
            T3CONbits.T3CCP2 = 0;
        }
        else if(CCP1_TIMER1_CCP2_TIMER3 == ccp2_obj->compare_timer_select){
            T3CONbits.T3CCP1 = 1;
            T3CONbits.T3CCP2 = 0;
        }
        else if(CCP1_CCP2_TIMER3 == ccp2_obj->compare_timer_select){
            T3CONbits.T3CCP1 = 1;
            T3CONbits.T3CCP2 = 1;
        }
        else{/*Nothing*/}
    }
    return ret;
}
#endif

#if (CCP2_CFG_SELECTED_MODE == CCP2_CFG_PWM_MODE)
Std_ReturnType CCP2_PWM_Init(const ccp2_t *ccp2_obj){
    Std_ReturnType ret = E_OK;
    uint16_t pwm_temp_dutycucle = 0;
    if(NULL == ccp2_obj){
        ret = E_NOT_OK;
    }
    else{
        /*Disable CCP2 Module*/
        CCP2_SET_MODE(CCP2_MODULE_DISABLE);
        /*Pin Configuration : Set RC2/CCP1x pin as output*/
        TRISCbits.RC1 = 0;
        /*Set the period of the generated pulse*/
        PR2 = (uint8_t)((_XTAL_FREQ)/(ccp2_obj->pwm_frequency * 4 * (ccp2_obj->timer2_prescaler_value + 1) * (ccp2_obj->timer2_postscaler_value + 1)) - 1);
        /*Set the DutyCycle of the generated pulse*/
        pwm_temp_dutycucle = (uint16_t)((PR2+1)*(ccp2_obj->pwm_DutyCycle/100.0)*4);
        CCP2CONbits.DC2B = (pwm_temp_dutycucle & 0x03);
        CCPR2L = (uint8_t)(ccp2_obj->pwm_DutyCycle);
        /*CCP2 Module PMW Mode Intialization*/
        if(ccp2_obj->ccp2_mode == CCP2_CFG_PWM_MODE){
            CCP2_SET_MODE(CCP2_PWM_MODE);
        }
        else{
            ret = E_NOT_OK;
        }
    }
    return ret;
}

Std_ReturnType CCP2_PWM_Change_DutyCyle(const uint8_t _duty){
    Std_ReturnType ret = E_OK;
    uint16_t pwm_temp_dutycucle = 0;
    pwm_temp_dutycucle = (uint16_t)((PR2+1)*(_duty/100.0)*4);
    CCP2CONbits.DC2B = (pwm_temp_dutycucle & 0x03);
    CCPR2L = (uint8_t)(pwm_temp_dutycucle >> 2);
    ret = E_NOT_OK;
    return ret;
}

Std_ReturnType CCP2_PWM_Start(void){
    Std_ReturnType ret = E_OK;
    /*Start the PWM Mode*/
    CCP2_SET_MODE(CCP2_PWM_MODE);
    ret = E_NOT_OK;
    return ret;
}

Std_ReturnType CCP2_PWM_Stop(void){
    Std_ReturnType ret = E_OK;
    /*Stop the PWM Mode*/
    CCP2_SET_MODE(CCP2_MODULE_DISABLE);
    ret = E_NOT_OK;
    return ret;
}
#endif

void CCP2_ISR(void){
    INTERRUPT_CCP2_CLEAR_FLAG();
    if(CCP2_InterruptHandler){
        CCP2_InterruptHandler();
    }
}





