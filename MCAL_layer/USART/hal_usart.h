/* 
 * File:   hal_usart.h
 * Author: Mohamed Elsayed
 *
 * Created on March 4, 2024, 11:52 AM
 */

#ifndef HAL_USART_H
#define	HAL_USART_H

/*SECTION : INCLUDES*/
#include <string.h>
#include "pic18f4620.h"
#include "../../MCAL_layer/std_types.h"
#include "../../MCAL_layer/device_config.h" 
#include "../../MCAL_layer/INTERRUPT/mcal_internal_interrupt.h"

/* SECTION : MACROS DECLARATION*/
/*EUSART Enable/Disable*/
#define EUSART_MODULE_ENABLE                    1
#define EUSART_MODULE_DISABLE                   0

/*EUSART Mode Select*/
#define EUSART_ASYNCHRONOUS_MODE                0
#define EUSART_SYNCHRONOUS_MODE                 1

/*16-Bit Baud Rate Register Enable*/
#define EUSART_BRG_8BIT_MODE                    0
#define EUSART_BRG_16BIT_MODE                   1

/*High Baud Rate Select*/
#define EUSART_ASYNCHRONOUS_LOW_SPEED           0
#define EUSART_ASYNCHRONOUS_HIGH_SPEED          1

/*9-Bit Transmit Enable*/
#define EUSART_TX_9BIT_ENABLE                   1
#define EUSART_TX_9BIT_DISABLE                  0

/*9-Bit Receive Enable*/
#define EUSART_RX_9BIT_ENABLE                   1
#define EUSART_RX_9BIT_DISABLE                  0

/*Transmit Enable*/
#define EUSART_TX_ENABLE                        1
#define EUSART_TX_DISABLE                       0

/*Reciever Enable*/
#define EUSART_RX_ENABLE                        1
#define EUSART_RX_DISABLE                       0   
/*SECTION : MACRO FUNCTIONS*/
    
/*SECTION : DATA TYPE DECLARATION*/
/*Baudrate Configuratons*/
typedef enum{
    BRG_8BIT_ASYNCHRONOUS_LOW_SPEED,
    BRG_8BIT_ASYNCHRONOUS_HIGH_SPEED,
    BRG_16BIT_ASYNCHRONOUS_LOW_SPEED,
    BRG_16BIT_ASYNCHRONOUS_HIGH_SPEED,
    BRG_8BIT_SYNCHRONOUS,
    BRG_16BIT_SYNCHRONOUS,
}baudrate_cfg_t;

typedef struct{
    void (*EUSART_TxInterruptHandler)(void);
    void (*EUSART_RxInterruptHandler)(void);
    void (*EUSART_FramingInterruptHandler)(void);
    void (*EUSART_OverrunInterruptHandler)(void);
    uint32_t baudrate_value;
    baudrate_cfg_t baudrate_cfg;
    uint8_t Tx_9bit_mode;
    uint8_t Rx_9bit_mode;
    uint8_t eusart_mode : 1;
    uint8_t interrupt_priority : 1;
}eusart_t;

/*FUNCTIONS DECLARATIONS*/
Std_ReturnType USART_ASYNC_ENABLE(const eusart_t *usart_obj);
Std_ReturnType USART_ASYNC_Transmit_Data_Blocking(const eusart_t *usart_obj, uint16_t data);
Std_ReturnType USART_ASYNC_Transmit_String_Blocking(const eusart_t *usart_obj, uint8_t str[]);
Std_ReturnType USART_ASYNC_Recieve_Data_Blocking(const eusart_t *usart_obj, uint16_t *data);
Std_ReturnType USART_ASYNC_DISABLE(void);
void EUSART_TX_ISR(void);
void EUSART_RX_ISR(void);
#endif	/* HAL_USART_H */

