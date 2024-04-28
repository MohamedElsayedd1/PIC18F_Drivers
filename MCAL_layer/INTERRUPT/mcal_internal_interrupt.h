/* 
 * File:   mcal_internal_interrupt.h
 * Author: Mohamed Elsayed
 *
 * Created on January 5, 2024, 3:13 PM
 */

#ifndef MCAL_INTERNAL_INTERRUPT_H
#define	MCAL_INTERNAL_INTERRUPT_H

/*SECTION : INCLUDES*/
#include "mcal_interrupt_config.h"

/* SECTION : MACROS DECLARATION*/

/*
 *@brief : ADC interrupt configurations
 */
#if ADC_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
/*Enable ADC Module interrupt source*/
#define INTERRUPT_ADC_ENABLE()         (PIE1bits.ADIE = 1)
/*Disable ADC Module interrupt source*/
#define INTERRUPT_ADC_DISABLE()        (PIE1bits.ADIE = 0)
/*clear ADC Module external interrupt flag*/
#define INTERRUPT_ADC_CLEAR_FLAG()     (PIR1bits.ADIF  = 0)
/*set ADC Module interrupt priority levels*/
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
#define INTERRUPT_ADC_HIGH_PRIORITY()  (IPR1bits.ADIP = 1)
#define INTERRUPT_ADC_LOW_PRIORITY()   (IPR1bits.ADIP = 0)
#endif
#endif

/*
 *@brief : Timer0 interrupt configurations
 */

#if TIMER0_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
/*Enable Timer0 Module interrupt source*/
#define INTERRUPT_TIMER0_ENABLE()         (INTCONbits.TMR0IE = 1)
/*Disable Timer0 Module interrupt source*/
#define INTERRUPT_TIMER0_DISABLE()        (INTCONbits.TMR0IE = 0)
/*clear Timer0 Module external interrupt flag*/
#define INTERRUPT_TIMER0_CLEAR_FLAG()     (INTCONbits.TMR0IF  = 0)
/*set Timer0 Module interrupt priority levels*/
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
#define INTERRUPT_TIMER0_HIGH_PRIORITY()  (INTCON2bits.TMR0IP = 1)
#define INTERRUPT_TIMER0_LOW_PRIORITY()   (INTCON2bits.TMR0IP = 0)
#endif
#endif

/*
 *@brief : Timer1 interrupt configurations
 */

#if TIMER1_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
/*Enable Timer1 Module interrupt source*/
#define INTERRUPT_TIMER1_ENABLE()         (PIE1bits.TMR1IE = 1)
/*Disable Timer1 Module interrupt source*/
#define INTERRUPT_TIMER1_DISABLE()        (PIE1bits.TMR1IE = 0)
/*clear Timer1 Module interrupt flag*/
#define INTERRUPT_TIMER1_CLEAR_FLAG()     (PIR1bits.TMR1IF = 0)
/*set Timer1 Module interrupt priority levels*/
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
#define INTERRUPT_TIMER1_HIGH_PRIORITY()  (IPR1bits.TMR1IP = 1)
#define INTERRUPT_TIMER1_LOW_PRIORITY()   (IPR1bits.TMR1IP = 0)
#endif
#endif

/*
 *@brief : Timer2 interrupt configurations
 */

#if TIMER2_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
/*Enable Timer2 Module interrupt source*/
#define INTERRUPT_TIMER2_ENABLE()         (PIE1bits.TMR2IE = 1)
/*Disable Timer2 Module interrupt source*/
#define INTERRUPT_TIMER2_DISABLE()        (PIE1bits.TMR2IE = 0)
/*clear Timer2 Module interrupt flag*/
#define INTERRUPT_TIMER2_CLEAR_FLAG()     (PIR1bits.TMR2IF = 0)
/*set Timer2 Module interrupt priority levels*/
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
#define INTERRUPT_TIMER2_HIGH_PRIORITY()  (IPR1bits.TMR2IP = 1)
#define INTERRUPT_TIMER2_LOW_PRIORITY()   (IPR1bits.TMR2IP = 0)
#endif
#endif

/*
 *@brief : Timer3 interrupt configurations
 */

#if TIMER2_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
/*Enable Timer3 Module interrupt source*/
#define INTERRUPT_TIMER3_ENABLE()         (PIE2bits.TMR3IE = 1)
/*Disable Timer3 Module interrupt source*/
#define INTERRUPT_TIMER3_DISABLE()        (PIE2bits.TMR3IE = 0)
/*clear Timer3 Module interrupt flag*/
#define INTERRUPT_TIMER3_CLEAR_FLAG()     (PIR2bits.TMR3IF = 0)
/*set Timer3 Module interrupt priority levels*/
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
#define INTERRUPT_TIMER3_HIGH_PRIORITY()  (IPR2bits.TMR3IP = 1)
#define INTERRUPT_TIMER3_LOW_PRIORITY()   (IPR2bits.TMR3IP = 0)
#endif
#endif

/*
 *@brief : CCP1 interrupt configurations
 */

#if CCP1_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
/*Enable CCP1 Module interrupt source*/
#define INTERRUPT_CCP1_ENABLE()         (PIE1bits.CCP1IE = 1)
/*Disable CCP1 Module interrupt source*/
#define INTERRUPT_CCP1_DISABLE()        (PIE1bits.CCP1IE = 0)
/*clear CCP1 Module interrupt flag*/
#define INTERRUPT_CCP1_CLEAR_FLAG()     (PIR1bits.CCP1IF = 0)
/*set CCP1 Module interrupt priority levels*/
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
#define INTERRUPT_CCP1_HIGH_PRIORITY()  (IPR1bits.CCP1IP = 1)
#define INTERRUPT_CCP1_LOW_PRIORITY()   (IPR1bits.CCP1IP = 0)
#endif
#endif

/*
 *@brief : CCP2 interrupt configurations
 */

#if CCP2_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
/*Enable CCP2 Module interrupt source*/
#define INTERRUPT_CCP2_ENABLE()         (PIE2bits.CCP2IE = 1)
/*Disable CCP2 Module interrupt source*/
#define INTERRUPT_CCP2_DISABLE()        (PIE2bits.CCP2IE = 0)
/*clear CCP2 Module interrupt flag*/
#define INTERRUPT_CCP2_CLEAR_FLAG()     (PIR2bits.CCP2IF = 0)
/*set CCP2 Module interrupt priority levels*/
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
#define INTERRUPT_CCP2_HIGH_PRIORITY()  (IPR2bits.CCP2IP = 1)
#define INTERRUPT_CCP2_LOW_PRIORITY()   (IPR2bits.CCP2IP = 0)
#endif
#endif

/*
 *@brief : EUSART TX interrupt configurations
 */

#if USART_TX_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
/*Enable EUSART TX interrupt source*/
#define INTERRUPT_USART_TX_ENABLE()         (PIE1bits.TXIE = 1)
/*Disable EUSART TX interrupt source*/
#define INTERRUPT_USART_TX_DISABLE()        (PIE1bits.TXIE = 0)
/*clear EUSART TX interrupt flag*/
#define INTERRUPT_USART_TX_CLEAR_FLAG()     (PIR1bits.TXIF = 0)
/*set EUSART TX interrupt priority levels*/
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
#define INTERRUPT_USART_TX_HIGH_PRIORITY()  (IPR1bits.TXIP = 1)
#define INTERRUPT_USART_TX_LOW_PRIORITY()   (IPR1bits.TXIP = 0)
#endif
#endif

/*
 *@brief : EUSART RX interrupt configurations
 */

#if USART_RX_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
/*Enable EUSART RX interrupt source*/
#define INTERRUPT_USART_RX_ENABLE()         (PIE1bits.RCIE = 1)
/*Disable EUSART RX interrupt source*/
#define INTERRUPT_USART_RX_DISABLE()        (PIE1bits.RCIE = 0)
/*clear EUSART RX interrupt flag*/
#define INTERRUPT_USART_RX_CLEAR_FLAG()     (PIR1bits.RCIF = 0)
/*set EUSART RX interrupt priority levels*/
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
#define INTERRUPT_USART_RX_HIGH_PRIORITY()  (IPR1bits.RCIP = 1)
#define INTERRUPT_USART_RX_LOW_PRIORITY()   (IPR1bits.RCIP = 0)
#endif
#endif

/*
 *@brief : MSSP I2C interrupt configurations
 */

#if MSSP_I2C_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
/*Enable MSSP I2C Transmit/Receive interrupt source*/
#define INTERRUPT_MSSP_I2C_ENABLE()         (PIE1bits.SSPIE = 1)
#define INTERRUPT_MSSP_I2C_BUS_COLLISION_ENABLE()          (PIE2bits.BCLIE = 1)
/*Disable MSSP I2C Transmit/Receive interrupt source*/
#define INTERRUPT_MSSP_I2C_DISABLE()        (PIE1bits.SSPIE = 0)
#define INTERRUPT_MSSP_I2C_BUS_COLLISION_DISABLE()         (PIE2bits.BCLIE = 0)
/*clear MSSP I2C Transmit/Receive interrupt flag*/
#define INTERRUPT_MSSP_I2C_CLEAR_FLAG()     (PIR1bits.SSPIF = 0)
#define INTERRUPT_MSSP_I2C_BUS_COLLISION_CLEAR_FLAG()      (PIR2bits.BCLIF = 0)
/*set MSSP I2C Transmit/Receive interrupt priority levels*/
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
#define INTERRUPT_MSSP_I2C_HIGH_PRIORITY()  (IPR1bits.SSPIP = 1)
#define INTERRUPT_MSSP_I2C_LOW_PRIORITY()   (IPR1bits.SSPIP = 0)

#define INTERRUPT_MSSP_I2C_BUS_COLLISION_HIGH_PRIORITY()  (IPR2bits.BCLIP = 1)
#define INTERRUPT_MSSP_I2C_BUS_COLLISION_LOW_PRIORITY()   (IPR2bits.BCLIP = 0)

#endif
#endif

/*SECTION : MACRO FUNCTIONS*/
    
/*SECTION : DATA TYPE DECLARATION*/

/*FUNCTIONS DECLARATIONS*/


#endif	/* MCAL_INTERNAL_INTERRUPT_H */

