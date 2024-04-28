/* 
 * File:   mcal_interrupt_gen_cfg.h
 * Author: Dubai Store
 *
 * Created on January 5, 2024, 9:01 PM
 */

#ifndef MCAL_INTERRUPT_GEN_CFG_H
#define	MCAL_INTERRUPT_GEN_CFG_H

#define INTERRUPT_FEATURE_ENABLE  1
#define INTERRUPT_FEATURE_DISABLE 0

/*Enables or disables priority levels feature*/
#define INTERRUPT_PRIORITY_LEVELS_ENABLE           INTERRUPT_FEATURE_DISABLE

/*Enables or disables both external interrupts (INTx _ On Change)*/
#define EXTERNAL_INTERRUPT_INTx_FEATURE_ENABLE     INTERRUPT_FEATURE_ENABLE
#define EXTERNAL_INTERRUPT_OnChange_FEATURE_ENABLE INTERRUPT_FEATURE_ENABLE

/*Enables or disables ADC interrupt source*/
#define ADC_INTERRUPT_FEATURE                      INTERRUPT_FEATURE_ENABLE

/*Enables or disables Timer0 interrupt source*/
#define TIMER0_INTERRUPT_FEATURE                   INTERRUPT_FEATURE_ENABLE

/*Enables or disables Timer1 interrupt source*/
#define TIMER1_INTERRUPT_FEATURE                   INTERRUPT_FEATURE_ENABLE

/*Enables or disables Timer2 interrupt source*/
#define TIMER2_INTERRUPT_FEATURE                   INTERRUPT_FEATURE_ENABLE

/*Enables or disables Timer3 interrupt source*/
#define TIMER3_INTERRUPT_FEATURE                   INTERRUPT_FEATURE_ENABLE

/*Enables or disables CCP1 interrupt source*/
#define CCP1_INTERRUPT_FEATURE                     INTERRUPT_FEATURE_ENABLE

/*Enables or disables CCP1 interrupt source*/
#define CCP2_INTERRUPT_FEATURE                     INTERRUPT_FEATURE_ENABLE

/*Enables or disables USART Transmitting interrupt source*/
#define USART_TX_INTERRUPT_FEATURE                 INTERRUPT_FEATURE_DISABLE

/*Enables or disables USART Receiving interrupt source*/
#define USART_RX_INTERRUPT_FEATURE                 INTERRUPT_FEATURE_DISABLE

/*Enables or disables I2C Receiving interrupt source*/
#define MSSP_I2C_INTERRUPT_FEATURE                      INTERRUPT_FEATURE_DISABLE

#endif	/* MCAL_INTERRUPT_GEN_CFG_H */

