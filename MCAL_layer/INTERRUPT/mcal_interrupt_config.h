/* 
 * File:   mcal_interrupt_config.h
 * Author: Mohamed Elsayed
 *
 * Created on January 5, 2024, 3:12 PM
 */

#ifndef MCAL_INTERRUPT_CONFIG_H
#define	MCAL_INTERRUPT_CONFIG_H

/*SECTION : INCLUDES*/
#include "pic18f4620.h"
#include "../std_types.h"
#include "../device_config.h"
#include "mcal_interrupt_gen_cfg.h"
#include "../../MCAL_layer/GPIO/hal_gpio.h"

/* SECTION : MACROS DECLARATION*/
#define INTERRUPT_ENABLE           1
#define INTERRUPT_DISABLE          0
#define INTERRUPT_OCCUR            1
#define INTERRUPT_NOT_OCCUR        0
#define INTERRUPT_PRIORITY_ENABLE  1
#define INTERRUPT_PRIORITY_DISABLE 0

/*SECTION : MACRO FUNCTIONS*/
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_PRIORITY_ENABLE
/*This macro enables priority feature for interrupts*/
#define INTERRUPT_InterruptPriorityEnable()   (RCONbits.IPEN = 1)

/*This macro enable all high-priority interrupts*/
#define INTERRUT_GlobalInterruptHighEnable()  (INTCONbits.GIEH = 1)
/*This macro disables all high-priority interrupts*/
#define INTERRUT_GlobalInterruptHighDisable() (INTCONbits.GIEH = 0)

/*This macro enable all low-priority interrupts*/
#define INTERRUT_GlobalInterruptLowEnable()   (INTCONbits.GIEL = 1)
/*This macro disables all low-priority interrupts*/
#define INTERRUT_GlobalInterruptLowDisable()  (INTCONbits.GIEL = 0)

#elif INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_PRIORITY_DISABLE
/*This macro disables priority feature for interrupts*/
#define INTERRUPT_InterruptPriorityDisable()  (RCONbits.IPEN = 0)

/*This macro enables all interrupt sources (peripherals and external pins)*/
#define INTERRUPT_GlobalInterruptEnable()     (INTCONbits.GIE  = 1)
/*This macro disables all interrupt sources (peripherals and external pins)*/
#define INTERRUPT_GlobalInterruptDisable()     (INTCONbits.GIE  = 0)

/*This macro enables all peripheral interrupt sources*/
#define INTERRUPT_PeripheralInterruptEnable()     (INTCONbits.PEIE = 1)
/*This macro disables all peripheral interrupt sources*/
#define INTERRUPT_PeripheralInterruptDisable()     (INTCONbits.PEIE = 0)
#endif

/*SECTION : DATA TYPE DECLARATION*/
/*INTx_priority_t struct to define which priority level from (LOW _ HIGH)*/
typedef enum{
    LOW_PRIORITY,
    HIGH_PRIORITY
}interrupt_prioity_t;

/*FUNCTIONS DECLARATIONS*/


#endif	/* MCAL_INTERRUPT_CONFIG_H */

