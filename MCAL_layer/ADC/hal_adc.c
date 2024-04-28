/* 
 * File:   hal_adc.c
 * Author: Mohamed Elsayed
 *
 * Created on February 2, 2024, 3:23 PM
 */

#include "hal_adc.h"

#if ADC_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
static InterruptHandler ADC_InterruptHandler = NULL;
#endif

static Std_ReturnType ADC_SetPinDirection(const adc_t *adc);
static Std_ReturnType ADC_AnalogSelect(const adc_channel_select_t *adc_channel);

/**
 * @summary Intializes the ADC
 * @param adc "pointer to the adc configurations"
 * @return status of the function
 *         (E_OK)      -> the function done successfully
 *         (E_NOT_OK)  -> the function has issue to perform this action
 */
 Std_ReturnType ADC_Init(const adc_t *adc){
    Std_ReturnType ret = E_OK;
    if(adc == NULL){
        ret = E_NOT_OK;
    }
    else{
        /* Set the pin as input */
        ret = ADC_SetPinDirection(adc);
        /* Disable ADC module */
        ADC_DISABLE();
        /* Select ADC input channel */
        ADC_CHANNEL_SELECT(adc->channel);
        /* Select ADC conversion clock */
        ADC_CONVERSION_CLOCK_SELECT(adc->conversion_clock);
        /* Select acquisition time */
        ADC_ACQUISITION_TIME_SELECT(adc->acquisition_time);
        /* Select Result format */
        ADC_RESULT_FORMAT(adc->adc_format); 
        /* Configure the interrupt */
        #if ADC_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
        /* Enable the global interrupt bit */      
        /* Enable the ADC Module interrupt source */
        INTERRUPT_ADC_ENABLE();
        /* Clear the ADC Module interrupt flag */
        INTERRUPT_ADC_CLEAR_FLAG();
        /* Set the callback function */
        ADC_InterruptHandler = adc->ADC_InterruptHandler;
        /* Set the ADC Module interrupt priority */
        #if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
        INTERRUPT_InterruptPriorityEnable();
        INTERRUT_GlobalInterruptHighEnable();
        INTERRUT_GlobalInterruptLowEnable();       
        if(HIGH_PRIORITY == adc->adc_priority){
            INTERRUPT_ADC_HIGH_PRIORITY();
        }
        else if(LOW_PRIORITY == adc->adc_priority){
            INTERRUPT_ADC_LOW_PRIORITY();
        }
        #else
        INTERRUPT_InterruptPriorityDisable();
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
        #endif
        #endif
        /* Configure voltage reference */
        VOLTAGE_REF_CONFIG1(adc->adc_voltage_ref);
        VOLTAGE_REF_CONFIG2(adc->adc_voltage_ref);
        /* Enable the ADC */
        ADC_ENABLE();
    }
    return ret;
}
 
 /**
 * @summary Un intialize the ADC
 * @param adc "pointer to the adc configurations"
 * @return status of the function
 *         (E_OK)      -> the function done successfully
 *         (E_NOT_OK)  -> the function has issue to perform this action
 */
Std_ReturnType ADC_Deinit(const adc_t *adc){
    Std_ReturnType ret = E_OK;
    if(adc == NULL){
        ret = E_NOT_OK;
    }
    else{
        /* Disable the interrupt*/
        #if ADC_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
        /* Disable the global interrupt sources */
        #if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
        INTERRUT_GlobalInterruptHighDisable();
        INTERRUT_GlobalInterruptLowDisable();
        #else
        INTERRUPT_GlobalInterruptDisable();
        INTERRUPT_PeripheralInterruptDisable();
        #endif
        /* Disable the ADC MODULE interrupt source */
        INTERRUPT_ADC_DISABLE();
        #endif
        /* Disable ADC module */
        ADC_DISABLE();
    }
    return ret;
}

/**
 * @summary Start the conversion process
 * @param adc "pointer to the adc configurations"
 * @return status of the function
 *         (E_OK)      -> the function done successfully
 *         (E_NOT_OK)  -> the function has issue to perform this action
 */
Std_ReturnType ADC_StartConversion(const adc_t *adc){
    Std_ReturnType ret = E_OK;
    if(adc == NULL){
        ret = E_NOT_OK;
    }
    ADC_START_CONVERSION();
    return ret;
}

/**
 * @summary Check if the conversion process has ended or not
 * @param adc "pointer to the adc configurations"
 * @return status of the function
 *         (E_OK)      -> the function done successfully
 *         (E_NOT_OK)  -> the function has issue to perform this action
 */
Std_ReturnType ADC_IsConversionDone(const adc_t *adc, uint8_t *conversion_status){
    Std_ReturnType ret = E_OK;
    if((adc == NULL) || (conversion_status == NULL)){
        ret = E_NOT_OK;
    }
    else{
        *conversion_status = (uint8_t)(!ADC_CONVERSION_STATUS());
    }
    return ret;
}

/**
 * @summary Return the conversion Result
 * @param adc "pointer to the adc configurations"
 * @return status of the function
 *         (E_OK)      -> the function done successfully
 *         (E_NOT_OK)  -> the function has issue to perform this action
 */
Std_ReturnType ADC_GetConversionResult(const adc_t *adc, adc_result_t *conversion_result){
    Std_ReturnType ret = E_OK;
    if((adc == NULL) || (conversion_result == NULL)){
        ret = E_NOT_OK;
    }
    else{
        if(adc->adc_format == ADC_RIGHT_FORMAT){
            *conversion_result = (uint16_t)(ADRESL + ((ADRESH & 0x03) << 8));
        }
        else if(adc->adc_format == ADC_LEFT_FORMAT){
           *conversion_result = (uint16_t)((ADRESL + ((ADRESH & 0x03) << 8)) >> 6); 
        }
        else{
           *conversion_result = (uint16_t)(ADRESL + ((ADRESH & 0x03) << 8));
        }
    }
    return ret;
}

/**
 * @summary Enable the ADC module, Start the conversion, and check if
 *          conversion process is done or not and Return the conversion Result
 * @param adc "pointer to the adc configurations"
 * @param conversion_result
 * @return status of the function
 *         (E_OK)      -> the function done successfully
 *         (E_NOT_OK)  -> the function has issue to perform this action 
 */
Std_ReturnType ADC_GetConversion_Blocking(const adc_t *adc, adc_result_t *conversion_result){
    Std_ReturnType ret = E_OK;
    uint8_t conversion_status = -1;
    if((adc == NULL) || (conversion_result == NULL)){
        ret = E_NOT_OK;
    }
    else{
        /* Intialize and Enable the ADC module*/
        ret = ADC_Init(adc);
        /* Start the conversion process */
        ret = ADC_StartConversion(adc);
        /*Check is the conversion process is over, if it's done, it returns the digital output*/
        while(conversion_status != ADC_CONVERSION_IS_COMPLATED){
            ret = ADC_IsConversionDone(adc, &conversion_status);
        }
        /* Another option : while(ADCON0bits.GODONE); 
         * ret = ADC_GetConversionResult(adc, conversion_result); */  
        /* Return the final result */
        if(ADC_CONVERSION_IS_COMPLATED == conversion_status){
            ret = ADC_GetConversionResult(adc, conversion_result);
        }
        else{
            ret = E_NOT_OK; 
        }
    }
    return ret;
}

#if ADC_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
/**
 * @summary Enable the ADC module, Start the conversion, and check if
 *          conversion process is done or not and Return the conversion Result
 * @param adc "pointer to the adc configurations"
 * @param conversion_result
 * @return status of the function
 *         (E_OK)      -> the function done successfully
 *         (E_NOT_OK)  -> the function has issue to perform this action 
 */
Std_ReturnType ADC_GetConversion_interrupt(const adc_t *adc){
    Std_ReturnType ret = E_OK;
    if(adc == NULL){
        ret = E_NOT_OK;
    }
    else{
        /* Intialize and Enable the ADC module*/
        ret = ADC_Init(adc);
        /* Start the conversion process */
        ret = ADC_StartConversion(adc);
    }
    return ret;
}
#endif


/**
 * @summary Set the analog pin as input "to recieve the analog signal"
 * @param adc
 * @return status of the function
 *         (E_OK)      -> the function done successfully
 *         (E_NOT_OK)  -> the function has issue to perform this action
 */
static Std_ReturnType ADC_SetPinDirection(const adc_t *adc){
    Std_ReturnType ret = E_OK;
    if(adc == NULL){
        ret = E_NOT_OK;
    }
    else{
        switch(adc->channel){
            case ADC_CHANNAL_AN0 : SET_BIT(TRISA, _TRISA_TRISA0_POSN); break;
            case ADC_CHANNAL_AN1 : SET_BIT(TRISA, _TRISA_TRISA1_POSN); break;
            case ADC_CHANNAL_AN2 : SET_BIT(TRISA, _TRISA_TRISA2_POSN); break;
            case ADC_CHANNAL_AN3 : SET_BIT(TRISA, _TRISA_TRISA3_POSN); break;
            case ADC_CHANNAL_AN4 : SET_BIT(TRISA, _TRISA_TRISA5_POSN); break;
            case ADC_CHANNAL_AN5 : SET_BIT(TRISE, _TRISE_TRISE0_POSN); break;
            case ADC_CHANNAL_AN6 : SET_BIT(TRISE, _TRISE_TRISE1_POSN); break;
            case ADC_CHANNAL_AN7 : SET_BIT(TRISE, _TRISE_TRISE2_POSN); break;
            case ADC_CHANNAL_AN8 : SET_BIT(TRISB, _TRISB_TRISB2_POSN); break;
            case ADC_CHANNAL_AN9 : SET_BIT(TRISB, _TRISB_TRISB3_POSN); break;
            case ADC_CHANNAL_AN10 : SET_BIT(TRISB, _TRISB_TRISB1_POSN); break;
            case ADC_CHANNAL_AN11 : SET_BIT(TRISB, _TRISB_TRISB4_POSN); break;
            case ADC_CHANNAL_AN12 : SET_BIT(TRISB, _TRISB_TRISB0_POSN); break;
        }
    }
    return ret;
}

void ADC_ISR(void){
    #if ADC_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
    /* Clear the adc interrupt flag*/
    INTERRUPT_ADC_CLEAR_FLAG();
    /* Execute the callback function*/
    if(ADC_InterruptHandler){
        ADC_InterruptHandler();
    }
    else{/*Nothing*/}
    #endif
}