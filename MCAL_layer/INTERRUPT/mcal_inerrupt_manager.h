/* 
 * File:   mcal_inerrupt_manager.h
 * Author: Mohamed Elsayed
 *
 * Created on January 5, 2024, 3:14 PM
 */

#ifndef MCAL_INERRUPT_MANAGER_H
#define	MCAL_INERRUPT_MANAGER_H

/*SECTION : INCLUDES*/
#include "mcal_interrupt_config.h"
#include "../../MCAL_layer/USART/hal_usart.h"

/* SECTION : MACROS DECLARATION*/

/*SECTION : MACRO FUNCTIONS*/

/*SECTION : DATA TYPE DECLARATION*/

/*FUNCTIONS DECLARATIONS*/
void INT0_ISR(void);
void INT1_ISR(void);
void INT2_ISR(void);
void RB4_ISR(uint8_t RBx_source);
void RB5_ISR(uint8_t RBx_source);
void RB6_ISR(uint8_t RBx_source);
void RB7_ISR(uint8_t RBx_source);
void ADC_ISR(void);
void TMR0_ISR(void);
void TMR1_ISR(void);
void TMR2_ISR(void);
void TMR3_ISR(void);
void CCP1_ISR(void);
void CCP2_ISR(void);
void EUSART_TX_ISR(void);
void EUSART_RX_ISR(void);
void SPI_ISR(void);
void MSSP_I2C_ISR(void);
void MSSP_I2C_BUS_COLLISION_ISR(void);
#endif	/* MCAL_INERRUPT_MANAGER_H */

