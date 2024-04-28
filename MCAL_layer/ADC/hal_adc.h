/* 
 * File:   hal_adc.h
 * Author: Mohamed Elsayed
 *
 * Created on February 2, 2024, 3:19 PM
 */

#ifndef HAL_ADC_H
#define	HAL_ADC_H

/*SECTION : INCLUDES*/
#include "../GPIO/hal_gpio.h"
#include "hal_adc_cfg.h"
#include "../INTERRUPT/mcal_internal_interrupt.h"

/* SECTION : MACROS DECLARATION*/

/*
 * @brief : A/D Port Configuration Control bits
 * @note : when ADC_AN4_ANALOG_FUNCTIONALITY is configured, this means 
 * AN4, AN3, AN2, AN1, AN0 are analog functionality
 */
#define ADC_ALL_DIGITAL_FUCTIONALITY    0x0F
#define ADC_AN0_ANALOG_FUNCTIONALITY    0x0E
#define ADC_AN1_ANALOG_FUNCTIONALITY    0x0D
#define ADC_AN2_ANALOG_FUNCTIONALITY    0x0C
#define ADC_AN3_ANALOG_FUNCTIONALITY    0x0B
#define ADC_AN4_ANALOG_FUNCTIONALITY    0x0A
#define ADC_AN5_ANALOG_FUNCTIONALITY    0x09
#define ADC_AN6_ANALOG_FUNCTIONALITY    0x08
#define ADC_AN7_ANALOG_FUNCTIONALITY    0x07
#define ADC_AN8_ANALOG_FUNCTIONALITY    0x06
#define ADC_AN9_ANALOG_FUNCTIONALITY    0x05
#define ADC_AN10_ANALOG_FUNCTIONALITY   0x04
#define ADC_AN11_ANALOG_FUNCTIONALITY   0x03
#define ADC_AN12_ANALOG_FUNCTIONALITY   0x02
#define ADC_ALL_ANALOG_FUNCTIONALITY    0x01

#define ADC_CONVERSION_IS_COMPLATED     0x01
#define ADC_CONVERSION_IN_PROGRESS      0x00



/*SECTION : MACRO FUNCTIONS*/

/*
 * ADC Enable bit
 * 0 = ADC is Disabled
 * 1 = ADC is Enabled
 */
#define ADC_ENABLE()                         ADCON0bits.ADON = 1
#define ADC_DISABLE()                        ADCON0bits.ADON = 0

/*
 * A/D Conversion Status bit
 * 0 = A/D conversion completed/not in progress
 * 1 = starts an A/D conversion cycle
 */
#define ADC_START_CONVERSION()               ADCON0bits.GODONE = 1
#define ADC_CONVERSION_STATUS()             (ADCON0bits.GODONE)

/*
 * VCFG1: Voltage Reference Configuration bit (VREF - source)
 * 1 = VREF - (AN2)
 * 0 = VSS "Default"
 * VCFG0: Voltage Reference Configuration bit (VREF + source)
 * 1 = VREF + (AN3)
 * 0 = VDD "Default"
 */
#define VOLTAGE_REF_CONFIG1(_REF)          (ADCON1bits.VCFG0 = _REF)    
#define VOLTAGE_REF_CONFIG2(_REF)          (ADCON1bits.VCFG1 = _REF)

/*
 * A/D Result Format Select
 * 1 = Right justified
 * 0 = Left justified
 */
#define ADC_RESULT_FORMAT(_FORMAT)          (ADCON2bits.ADFM = _FORMAT) 

/*
 * A/D Port Configuration Control bits
 */
#define ADC_CONFIGURE_ANALOG_BIT(_ANx)     (ADCON1bits.PCFG = _ANx)

/*
 * Analog Channel Select bits
 */
#define ADC_CHANNEL_SELECT(_CHANNEL)       (ADCON0bits.CHS  = _CHANNEL)

/*
 * A/D Acquisition Time Select bits
 */
#define ADC_ACQUISITION_TIME_SELECT(_TAD)  (ADCON2bits.ACQT  = _TAD)

/*
 * A/D Conversion Clock Select bits
 */
#define ADC_CONVERSION_CLOCK_SELECT(_FOSC) (ADCON2bits.ADCS = _FOSC)

/*SECTION : DATA TYPE DECLARATION*/

/*
 * @brief : Analog Channel
 */
typedef enum{
    ADC_CHANNAL_AN0 = 0,
    ADC_CHANNAL_AN1,
    ADC_CHANNAL_AN2,
    ADC_CHANNAL_AN3,
    ADC_CHANNAL_AN4,
    ADC_CHANNAL_AN5,
    ADC_CHANNAL_AN6,
    ADC_CHANNAL_AN7,
    ADC_CHANNAL_AN8,
    ADC_CHANNAL_AN9,
    ADC_CHANNAL_AN10,
    ADC_CHANNAL_AN11,
    ADC_CHANNAL_AN12,
}adc_channel_select_t;

/*
 * @brief : A/D Acquisition Time 
 */
typedef enum{
    ADC_0_TAD = 0,
    ADC_2_TAD,
    ADC_4_TAD,
    ADC_6_TAD,
    ADC_8_TAD,
    ADC_12_TAD,
    ADC_16_TAD,
    ADC_20_TAD   
}adc_acquisition_time_t;

/*
 * @brief : A/D Conversion Clock 
 * @note : If the A/D FRC clock source is selected, a delay of one TCY (instruction cycle) is added before the A/D
           clock starts. This allows the SLEEP instruction to be executed before starting a conversion.
 */
typedef enum{
    ADC_CONVERSION_CLOCK_FOCS_DIV_2 = 0,
    ADC_CONVERSION_CLOCK_FOCS_DIV_8,
    ADC_CONVERSION_CLOCK_FOCS_DIV_32,
    ADC_CONVERSION_CLOCK_FOCS_DIV_FRC,
    ADC_CONVERSION_CLOCK_FOCS_DIV_4,
    ADC_CONVERSION_CLOCK_FOCS_DIV_16,
    ADC_CONVERSION_CLOCK_FOCS_DIV_64
}adc_conversion_clock_t;

/*
 * @brief : A/D Result Format Select
 */
typedef enum{
    ADC_LEFT_FORMAT = 0,
    ADC_RIGHT_FORMAT                               // Recommended 
}adc_result_format_t;

/*
 * @brief : Voltage Reference Configuration
 */
typedef enum{
    ADC_INTERNAL_REF = 0,                          // Recommended 
    ADC_EXTERNAL_REF       
}adc_voltage_ref_t;

/*
 * @brief : Structure of all ADC configurations
 */
typedef struct{
    void (*ADC_InterruptHandler)(void);            /* CallBack function */
    interrupt_prioity_t adc_priority;              /*interrupt priority*/
    adc_channel_select_t channel;                  /* @ref : adc_channel_select_t */
    adc_conversion_clock_t conversion_clock;       /* @ref : adc_conversion_clock_t */
    adc_acquisition_time_t acquisition_time;       /* @ref : adc_acquisition_time_t */
    adc_result_format_t adc_format;                /* @ref : adc_result_format_t */
    adc_voltage_ref_t adc_voltage_ref;             /* @ref : adc_voltage_ref_t*/
}adc_t;

typedef uint16_t adc_result_t;

/*Pointer to callback function declaration*/
typedef void (*InterruptHandler)(void);

/*FUNCTIONS DECLARATIONS*/
Std_ReturnType ADC_Init(const adc_t *adc);
Std_ReturnType ADC_Deinit(const adc_t *adc);
Std_ReturnType ADC_StartConversion(const adc_t *adc);
Std_ReturnType ADC_IsConversionDone(const adc_t *adc, uint8_t *conversion_status);
Std_ReturnType ADC_GetConversionResult(const adc_t *adc, adc_result_t *conversion_result);
Std_ReturnType ADC_GetConversion_Blocking(const adc_t *adc, adc_result_t *conversion_result);
#if ADC_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
Std_ReturnType ADC_GetConversion_interrupt(const adc_t *adc);
#endif

#endif	/* HAL_ADC_H */

