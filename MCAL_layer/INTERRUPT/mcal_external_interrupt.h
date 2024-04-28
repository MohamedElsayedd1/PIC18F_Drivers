/* 
 * File:   mcal_external_interrupt.h
 * Author: Mohamed Elsayed
 *
 * Created on January 5, 2024, 3:14 PM
 */

#ifndef MCAL_EXTERNAL_INTERRUPT_H
#define	MCAL_EXTERNAL_INTERRUPT_H

/*SECTION : INCLUDES*/
#include "mcal_interrupt_config.h"

/* SECTION : MACROS DECLARATION*/
#if EXTERNAL_INTERRUPT_INTx_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE

/*Enable (INT0 _ INT1 _ INT2) external interrupt sources*/
#define INTERRUPT_INT0_ENABLE()        (INTCONbits.INT0IE = 1)
#define INTERRUPT_INT1_ENABLE()        (INTCON3bits.INT1IE = 1)
#define INTERRUPT_INT2_ENABLE()        (INTCON3bits.INT2IE = 1)

/*Disable (INT0 _ INT1 _ INT2) external interrupt sources*/
#define INTERRUPT_INT0_DISABLE()       (INTCONbits.INT0IE = 0)
#define INTERRUPT_INT1_DISABLE()       (INTCON3bits.INT1IE = 0)
#define INTERRUPT_INT2_DISABLE()       (INTCON3bits.INT2IE = 0)

/*clear (INT0 _ INT1 _ INT2) external interrupt flags*/
#define INTERRUPT_INT0_CLEAR_FLAG()    (INTCONbits.INT0F = 0)
#define INTERRUPT_INT1_CLEAR_FLAG()    (INTCON3bits.INT1F = 0)
#define INTERRUPT_INT2_CLEAR_FLAG()    (INTCON3bits.INT2F = 0)

/*set (INT0 _ INT1 _ INT2) external interrupt priority levels*/
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
#define INTERRUPT_INT1_HIGH_PRIORITY() (INTCON3bits.INT1IP = 1)
#define INTERRUPT_INT2_HIGH_PRIORITY() (INTCON3bits.INT2IP = 1)

#define INTERRUPT_INT1_LOW_PRIORITY()  (INTCON3bits.INT1IP = 0)
#define INTERRUPT_INT2_LOW_PRIORITY()  (INTCON3bits.INT2IP = 0)
#endif

/*set (INT0 _ INT1 _ INT2) external interrupt edge (Falling _ rising)*/
#define INTERRUPT_INT0_RISING_EDGE()   (INTCON2bits.INTEDG0 = 1)
#define INTERRUPT_INT0_FALLING_EDGE()  (INTCON2bits.INTEDG0 = 0)

#define INTERRUPT_INT1_RISING_EDGE()   (INTCON2bits.INTEDG1 = 1)
#define INTERRUPT_INT1_FALLING_EDGE()  (INTCON2bits.INTEDG1 = 0)

#define INTERRUPT_INT2_RISING_EDGE()   (INTCON2bits.INTEDG2 = 1)
#define INTERRUPT_INT2_FALLING_EDGE()  (INTCON2bits.INTEDG2 = 0)

#endif

#if EXTERNAL_INTERRUPT_OnChange_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
/*Enable (RB "On Change") external interrupt sources*/
#define INTERRUPT_RBx_ENABLE()         (INTCONbits.RBIE = 1)

/*Disable (RB "On Change") external interrupt sources*/
#define INTERRUPT_RBx_DISABLE()        (INTCONbits.RBIE = 0)

/*clear (RB "On Change") external interrupt flag*/
#define INTERRUPT_RBx_CLEAR_FLAG()     (INTCONbits.RBIF = 0)

#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
#define INTERRUPT_RBx_HIGH_PRIORITY()  (INTCON2bits.RBIP = 1)
#define INTERRUPT_RBx_LOW_PRIORITY()   (INTCON2bits.RBIP = 0)
#endif

#endif

/*SECTION : MACRO FUNCTIONS*/

/*SECTION : DATA TYPE DECLARATION*/
/*Pointer to function declaration*/
typedef void (*InterruptHandler)(void);

/*INTx_t struct to define which interrupt source from (INT0 _ INT1 _ INT2)*/
typedef enum{
    EXT_INT0,
    EXT_INT1,
    EXT_INT2
}INTx_t;

/*INTx_t struct to define which interrupt trigering edge from (FALLING _ RISING)*/
typedef enum{
    FALLING_EDGE,
    RISING_EDGE
}INTx_edge_t;

/*interrupt_INTx_t struct to configure INTx pins specifications*/
typedef struct{
    void (*EXT_InterruptHandler)(void);
    pin_config_t pin;
    INTx_t INTx_source;
    interrupt_prioity_t INTx_priority;
    INTx_edge_t INTx_edge;
}interrupt_INTx_t;

/*interrupt_RBx_t struct to configure RBx pins specifications*/
typedef struct{
    void (*EXT_high_InterruptHandler)(void);
    void (*EXT_low_InterruptHandler)(void);
    pin_config_t pin;
    interrupt_prioity_t RBx_priority;
}interrupt_RBx_t;

/*FUNCTIONS DECLARATIONS*/
Std_ReturnType Interrupt_INTx_init(const interrupt_INTx_t *int_obj);
Std_ReturnType Interrupt_INTx_Dinit(const interrupt_INTx_t *int_obj);
Std_ReturnType Interrupt_RBx_init(const interrupt_RBx_t *int_obj);
Std_ReturnType Interrupt_RBx_Dinit(const interrupt_RBx_t *int_obj);

void INT0_ISR(void);
void INT1_ISR(void);
void INT2_ISR(void);

void RB4_ISR(uint8_t RBx_source);
void RB5_ISR(uint8_t RBx_source);
void RB6_ISR(uint8_t RBx_source);
void RB7_ISR(uint8_t RBx_source);


#endif	/* MCAL_EXTERNAL_INTERRUPT_H */

