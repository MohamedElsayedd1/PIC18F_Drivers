/* 
 * File:   hal_ccp2.h
 * Author: Mohamed Elsayed
 *
 * Created on February 24, 2024, 2:21 PM
 */

#ifndef HAL_CCP2_H
#define	HAL_CCP2_H

/*SECTION : INCLUDES*/
#include "hal_ccp2_cfg.h"
#include "../INTERRUPT/mcal_internal_interrupt.h"

/* SECTION : MACROS DECLARATION*/
#define CCP2_MODULE_DISABLE                         ((uint8_t)0X00)
#define CCP2_CAPTURE_MODE_1_FALLING_EDGE            ((uint8_t)0X04)
#define CCP2_CAPTURE_MODE_1_RISING_EDGE             ((uint8_t)0X05)
#define CCP2_CAPTURE_MODE_4_RISING_EDGE             ((uint8_t)0X06)
#define CCP2_CAPTURE_MODE_16_RISING_EDGE            ((uint8_t)0X07)
#define CCP2_COMPARE_MODE_SET_PIN_HIGH              ((uint8_t)0X08)
#define CCP2_COMPARE_MODE_SET_PIN_LOW               ((uint8_t)0X09)
#define CCP2_COMPARE_MODE_TOGGLE_PIN                ((uint8_t)0X02) 
#define CCP2_COMPARE_MODE_SW_INTERRUPT              ((uint8_t)0X0A)
#define CCP2_COMPARE_MODE_GEN_EVENT                 ((uint8_t)0X0B)
#define CCP2_PWM_MODE                               ((uint8_t)0X0C)

#define CCP1_CCP2_TIMER1                    0X00
#define CCP1_TIMER1_CCP2_TIMER3             0x01
#define CCP1_CCP2_TIMER3                    0X10

/*CCP1 Capture Mode State*/
#define CCP2_CAPTURE_READY                  0X01
#define CCP2_CAPTURE_NOT_READY              0X00

/*CCP1 Capture Mode State*/
#define CCP2_COMPARE_READY                  0X01
#define CCP2_COMPARE_NOT_READY              0X00

/*SECTION : MACRO FUNCTIONS*/
#define CCP2_SET_MODE(_MODE)                (CCP2CONbits.CCP2M = _MODE)

#define CCP2_TIMER_SELCET(_TIMER)           do{T3CONbits.T3CCP1 = _TIMER & 0x01; T3CONbits.T3CCP2 = (_TIMER >> 1) & 0x01;}while(0);

/*SECTION : DATA TYPE DECLARATION*/
typedef enum{
    CCP2_CAPTURE_MODE_SELECT = 0,
    CCP2_COMPARE_MODE_SELECT,
    CCP2_PWM_MODE_SELECT
}ccp2_mode_t;

typedef union{
    struct{
        uint8_t ccpr2_l;
        uint8_t ccpr2_h;
    };
    struct{
        uint8_t ccpr2;
    };
}CCP2_REG_T;

typedef struct{
    void (*CCP2_InterruptHandler)(void);
    uint8_t ccp2_priority : 1;
    ccp2_mode_t ccp2_mode;
    #if (CCP2_CFG_SELECTED_MODE == CCP2_CFG_CAPTURE_MODE)
        uint8_t ccp2_capture_mode;
        uint8_t capture_timer_select;
    
    #endif
    #if (CCP2_CFG_SELECTED_MODE == CCP2_CFG_COMPARE_MODE)
        uint16_t Compare_Value;
        uint8_t ccp2_compare_mode;
        uint8_t compare_timer_select;
    #endif
    #if (CCP2_CFG_SELECTED_MODE == CCP2_CFG_PWM_MODE)
        uint8_t pwm_frequency;
        uint8_t pwm_DutyCycle;
        uint8_t timer2_postscaler_value : 4;
        uint8_t timer2_prescaler_value : 2;
    #endif
}ccp2_t;

/*FUNCTIONS DECLARATIONS*/
Std_ReturnType CCP2_DeInit(const ccp2_t *ccp2_obj);
#if (CCP2_CFG_SELECTED_MODE == CCP2_CFG_CAPTURE_MODE)
Std_ReturnType CCP2_Capture_Init(const ccp2_t *ccp2_obj);
Std_ReturnType CCP2_IsCaptureComplete(uint8_t *_capture_status);
Std_ReturnType CCP2_Capture_Read_Value(uint16_t *_capture_value);
#endif
#if (CCP2_CFG_SELECTED_MODE == CCP2_CFG_COMPARE_MODE)
Std_ReturnType CCP2_Compare_Init(const ccp2_t *ccp2_obj);
Std_ReturnType CCP2_IsCompareComplete(uint8_t *_compare_status);
Std_ReturnType CCP2_Compare_Write_Value(uint16_t _compare_value); 
#endif
#if (CCP2_CFG_SELECTED_MODE == CCP2_CFG_PWM_MODE)
Std_ReturnType CCP2_PWM_Init(const ccp2_t *ccp2_obj);
Std_ReturnType CCP2_PWM_Change_DutyCyle(const uint8_t _duty);
Std_ReturnType CCP2_PWM_Start(void);
Std_ReturnType CCP2_PWM_Stop(void);
#endif


#endif	/* HAL_CCP2_H */

