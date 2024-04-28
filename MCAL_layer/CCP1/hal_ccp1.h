/* 
 * File:   hal_ccp1.h
 * Author: Mohamed Elsayed 
 *
 * Created on February 21, 2024, 11:51 AM
 */

#ifndef HAL_CCP1_H
#define	HAL_CCP1_H

/*SECTION : INCLUDES*/
#include "hal_ccp1_cfg.h"
#include "../../MCAL_layer/GPIO/hal_gpio.h"
#include "../../MCAL_layer/INTERRUPT/mcal_internal_interrupt.h"

/* SECTION : MACROS DECLARATION*/
#define CCP1_MODULE_DISABLE                 ((uint8_t)0X00)
#define CCP1_CAPTURE_MODE_1_FALLING_EDGE    ((uint8_t)0X04)
#define CCP1_CAPTURE_MODE_1_RISING_EDGE     ((uint8_t)0X05)
#define CCP1_CAPTURE_MODE_4_RISING_EDGE     ((uint8_t)0X06)
#define CCP1_CAPTURE_MODE_16_RISING_EDGE    ((uint8_t)0X07)
#define CCP1_COMPARE_MODE_SET_PIN_HIGH      ((uint8_t)0X08)
#define CCP1_COMPARE_MODE_SET_PIN_LOW       ((uint8_t)0X09)
#define CCP1_COMPARE_MODE_TOGGLE_PIN        ((uint8_t)0X02) 
#define CCP1_COMPARE_MODE_SW_INTERRUPT      ((uint8_t)0X0A)
#define CCP1_COMPARE_MODE_GEN_EVENT         ((uint8_t)0X0B)
#define CCP1_PWM_MODE                       ((uint8_t)0X0C)

#define CCP1_CCP2_TIMER1                    0X00
#define CCP1_TIMER1_CCP2_TIMER3             0x01
#define CCP1_CCP2_TIMER3                    0X10

/*CCP1 Capture Mode State*/
#define CCP1_CAPTURE_READY                  0X01
#define CCP1_CAPTURE_NOT_READY              0X00

/*CCP1 Capture Mode State*/
#define CCP1_COMPARE_READY                  0X01
#define CCP1_COMPARE_NOT_READY              0X00

/*SECTION : MACRO FUNCTIONS*/
#define CCP1_SET_MODE(_MODE)                (CCP1CONbits.CCP1M = _MODE)


/*SECTION : DATA TYPE DECLARATION*/
typedef enum{
    CCP1_CAPTURE_MODE_SELECT = 0,
    CCP1_COMPARE_MODE_SELECT,
    CCP1_PWM_MODE_SELECT
}ccp1_mode_t;

typedef union{
    struct{
        uint8_t ccpr1_l;
        uint8_t ccpr1_h;
    };
    struct{
        uint16_t ccpr1;
    };
}CCP1_REG_T;

typedef struct{
    void (*CCP1_InterruptHandler)(void);
    uint8_t ccp1_priority : 1;
    ccp1_mode_t ccp1_mode;
    #if (CCP1_CFG_SELECTED_MODE == CCP1_CFG_CAPTURE_MODE)
        uint8_t ccp1_capture_mode;
        uint8_t capture_timer_select;
    #endif
    #if (CCP1_CFG_SELECTED_MODE == CCP1_CFG_COMPARE_MODE)
        uint16_t Compare_Value;
        uint8_t ccp1_compare_mode;
        uint8_t compare_timer_select;
    #endif
    #if (CCP1_CFG_SELECTED_MODE == CCP1_CFG_PWM_MODE)
        uint32_t pwm_frequency;
        uint8_t pwm_DutyCycle;
        uint8_t timer2_postscaler_value : 4;
        uint8_t timer2_prescaler_value : 2;
    #endif
}ccp1_t;

typedef void (*InterruptHandler)(void);
/*FUNCTIONS DECLARATIONS*/

#if (CCP1_CFG_SELECTED_MODE == CCP1_CFG_CAPTURE_MODE)
Std_ReturnType CCP1_Capture_Init(const ccp1_t *ccp1_obj);
Std_ReturnType CCP1_IsCaptureComplete(uint8_t *_capture_status);
Std_ReturnType CCP1_Capture_Read_Value(uint16_t *_capture_value);
#endif
#if (CCP1_CFG_SELECTED_MODE == CCP1_CFG_COMPARE_MODE)
Std_ReturnType CCP1_Compare_Init(const ccp1_t *ccp1_obj);
Std_ReturnType CCP1_IsCompareComplete(uint8_t *_compare_status);
Std_ReturnType CCP1_Compare_Write_Value(uint16_t _compare_value); 
#endif
#if (CCP1_CFG_SELECTED_MODE == CCP1_CFG_PWM_MODE)
Std_ReturnType CCP1_PWM_Init(const ccp1_t *ccp1_obj);
Std_ReturnType CCP1_PWM_Change_DutyCyle(const uint8_t _duty);
Std_ReturnType CCP1_PWM_Start(void);
Std_ReturnType CCP1_PWM_Stop(void);
#endif

Std_ReturnType CCP1_DeInit(const ccp1_t *ccp1_obj);

void CCP1_ISR(void);

#endif	/* HAL_CCP1_H */

