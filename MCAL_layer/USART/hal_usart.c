/* 
 * File:   hal_usart.h
 * Author: Mohamed Elsayed
 *
 * Created on March 4, 2024, 11:52 AM
 */

#include "hal_usart.h"

static volatile void (*TX_InterruptHandler)(void) = NULL;
static void (*RX_InterruptHandler)(void) = NULL;
static void (*FramingError_InterruptHandler)(void) = NULL;
static void (*OverrunError_InterruptHandler)(void) = NULL;

static Std_ReturnType USART_Set_BRG(const eusart_t *usart_obj);
static Std_ReturnType Tx_Interrupt_Configurations(const eusart_t *usart_obj);
static Std_ReturnType Rx_Interrupt_Configurations(const eusart_t *usart_obj);

Std_ReturnType USART_ASYNC_ENABLE(const eusart_t *usart_obj){
    Std_ReturnType ret = E_OK;
    if(NULL == usart_obj){
        ret = E_NOT_OK;
    }
    else{
        /*Disable USART module*/
        RCSTAbits.SPEN = EUSART_MODULE_DISABLE;
        /*Set BaudRate configurations and value*/
        ret = USART_Set_BRG(usart_obj);
        /*Enable ASYNC/SYNC mode*/
        TXSTAbits.SYNC = usart_obj->eusart_mode;
        /*Setting SPEN bit*/
        RCSTAbits.SPEN = EUSART_MODULE_ENABLE;
        /*Configure RC6 and RC7 as input pins*/
        TRISCbits.RC6 = 0x01;
        TRISCbits.RC6 = 0x01;
        /*Set the EUSART TX interrupt configurations*/
        ret = Tx_Interrupt_Configurations(usart_obj);
        /*Set the EUSART RX interrupt configurations*/
        ret = Rx_Interrupt_Configurations(usart_obj);
        /*Set the 9-bit transmit configurations*/
        TXSTAbits.TX9 = usart_obj->Tx_9bit_mode;
        /*Set the 9-bit receive configurations*/
        RCSTAbits.RX9 = usart_obj->Rx_9bit_mode;
        TX_InterruptHandler = usart_obj->EUSART_TxInterruptHandler;
    }
    return ret;
}

Std_ReturnType USART_ASYNC_Transmit_Data_Blocking(const eusart_t *usart_obj, uint16_t data){
    Std_ReturnType ret = E_OK;
    if(NULL == usart_obj){
        ret = E_NOT_OK;
    }
    else{
        /*Enable Transmitting Data*/
        TXSTAbits.TXEN = EUSART_TX_ENABLE;
        /*Check if the TSR is full or empty*/
        while(!(TXSTAbits.TRMT));
        /*check if it is 9-bit or 8-bit Transmission*/
        if(usart_obj->Tx_9bit_mode == EUSART_TX_9BIT_ENABLE){
            TXREG = (uint8_t)(data);
            TXSTAbits.TX9D = (data >> 8);
        }
        else if(usart_obj->Tx_9bit_mode == EUSART_TX_9BIT_DISABLE){
            TXREG = (uint8_t)(data);
        }
    }
    return ret;
    
}

Std_ReturnType USART_ASYNC_Transmit_String_Blocking(const eusart_t *usart_obj, uint8_t str[]){
    Std_ReturnType ret = E_OK;
    uint8_t str_counter = 0;
    if(NULL == usart_obj){
        ret = E_NOT_OK;
    }
    else{
        
        for(str_counter = 0; str_counter < strlen(str); str_counter++)
            USART_ASYNC_Transmit_Data_Blocking(usart_obj, str[str_counter]);
    }
    return ret;
}


Std_ReturnType USART_ASYNC_Recieve_Data_Blocking(const eusart_t *usart_obj, uint16_t *data){
    Std_ReturnType ret = E_OK;
    if((NULL == usart_obj) || (NULL == data)){
        ret = E_NOT_OK;
    }
    else{
        /*Enable Receiving Data*/
        RCSTAbits.CREN = EUSART_RX_ENABLE;
        /*Check if the RCREG is full or empty*/
        while(!(PIR1bits.RCIF));
        /*Clear the EUSART Rx flag*/
        PIR1bits.RCIF = 0;
        /*check if it is 9-bit or 8-bit receiving*/
        if(usart_obj->Tx_9bit_mode == EUSART_TX_9BIT_ENABLE){
            *data = (uint16_t)((uint8_t)(RCREG) + (RCSTAbits.RX9D << 8));
        }
        else if(usart_obj->Tx_9bit_mode == EUSART_TX_9BIT_DISABLE){
            *data = (uint8_t)RCREG;
        }
        /*Clearing any error by resetting CREN bit*/
        if((OERR == 1) || (FERR == 1)){
            RCSTAbits.CREN = EUSART_RX_9BIT_DISABLE;
            RCSTAbits.CREN = EUSART_RX_9BIT_ENABLE;
        }
    }
    return ret;
    
}

Std_ReturnType USART_ASYNC_DISABLE(void){
    
}

static Std_ReturnType USART_Set_BRG(const eusart_t *usart_obj){
    Std_ReturnType ret = E_OK;
    uint32_t baudrate_value = usart_obj->baudrate_value;
    uint16_t baudrate_16bit_reg = 0;
    if(NULL == usart_obj){
        ret = E_NOT_OK;
    }
    else{
        if(usart_obj->baudrate_cfg == BRG_8BIT_ASYNCHRONOUS_LOW_SPEED){
            TXSTAbits.SYNC = EUSART_ASYNCHRONOUS_MODE;
            TXSTAbits.BRGH = EUSART_ASYNCHRONOUS_LOW_SPEED;
            BAUDCONbits.BRG16 = EUSART_BRG_8BIT_MODE;
            SPBRG = (_XTAL_FREQ/(64*baudrate_value)-1);
        }
        else if(usart_obj->baudrate_cfg == BRG_8BIT_ASYNCHRONOUS_HIGH_SPEED){
            TXSTAbits.SYNC = EUSART_ASYNCHRONOUS_MODE;
            TXSTAbits.BRGH = EUSART_ASYNCHRONOUS_HIGH_SPEED;
            BAUDCONbits.BRG16 = EUSART_BRG_8BIT_MODE;
            SPBRG = (_XTAL_FREQ/(16*baudrate_value)-1);
        }
        else if(usart_obj->baudrate_cfg == BRG_16BIT_ASYNCHRONOUS_LOW_SPEED){
            TXSTAbits.SYNC = EUSART_ASYNCHRONOUS_MODE;
            TXSTAbits.BRGH = EUSART_ASYNCHRONOUS_LOW_SPEED;
            BAUDCONbits.BRG16 = EUSART_BRG_16BIT_MODE;
            baudrate_16bit_reg = (_XTAL_FREQ/(16*baudrate_value)-1);
            SPBRG = (uint8_t)(baudrate_16bit_reg);
            SPBRGH = (uint8_t)(baudrate_16bit_reg >> 8);
        }
        else if(usart_obj->baudrate_cfg == BRG_16BIT_ASYNCHRONOUS_HIGH_SPEED){
            TXSTAbits.SYNC = EUSART_ASYNCHRONOUS_MODE;
            TXSTAbits.BRGH = EUSART_ASYNCHRONOUS_HIGH_SPEED;
            BAUDCONbits.BRG16 = EUSART_BRG_16BIT_MODE;
            baudrate_16bit_reg = (_XTAL_FREQ/(4*baudrate_value)-1);
            SPBRG = (uint8_t)(baudrate_16bit_reg);
            SPBRGH = (uint8_t)(baudrate_16bit_reg >> 8);
        }
        else if(usart_obj->baudrate_cfg == BRG_8BIT_SYNCHRONOUS){
            TXSTAbits.SYNC = EUSART_SYNCHRONOUS_MODE;
            BAUDCONbits.BRG16 = EUSART_BRG_8BIT_MODE;
            SPBRG = (_XTAL_FREQ/(4*baudrate_value)-1);
            baudrate_16bit_reg = (_XTAL_FREQ/(16*baudrate_value)-1);
            SPBRG = (uint8_t)(baudrate_16bit_reg);
            SPBRGH = (uint8_t)(baudrate_16bit_reg >> 8);
        }
        else if(usart_obj->baudrate_cfg == BRG_16BIT_SYNCHRONOUS){
            TXSTAbits.SYNC = EUSART_SYNCHRONOUS_MODE;
            BAUDCONbits.BRG16 = EUSART_BRG_16BIT_MODE;
            baudrate_16bit_reg = (_XTAL_FREQ/(4*baudrate_value)-1);
            SPBRG = (uint8_t)(baudrate_16bit_reg);
            SPBRGH = (uint8_t)(baudrate_16bit_reg >> 8);
        }
    }
    return ret;
}

static Std_ReturnType Tx_Interrupt_Configurations(const eusart_t *usart_obj){
    /*Enable EUSART TX interrupt source*/
    #if USART_TX_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
    #if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
    INTERRUPT_InterruptPriorityEnable();
    INTERRUT_GlobalInterruptHighEnable();
    INTERRUT_GlobalInterruptLowEnable();
    /*set EUSART TX interrupt priority levels*/
    if(usart_obj->interrupt_priority == HIGH_PRIORITY){
        INTERRUPT_USART_TX_HIGH_PRIORITY();
    }
    else if(usart_obj->interrupt_priority == LOW_PRIORITY){
        INTERRUPT_USART_TX_LOW_PRIORITY();
    }
    else{/*Nothing*/}
    INTERRUPT_USART_TX_ENABLE();
    /*clear EUSART TX external interrupt flag*/ 
    INTERRUPT_USART_TX_CLEAR_FLAG();
    /*Intialize the callback function*/
    TX_InterruptHandler = usart_obj->EUSART_TxInterruptHandler;
    #else
    INTERRUPT_InterruptPriorityDisable();
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();
    INTERRUPT_USART_TX_ENABLE();
    /*clear EUSART TX external interrupt flag*/
    INTERRUPT_USART_TX_CLEAR_FLAG();
    /*Intialize the callback function*/
    TX_InterruptHandler = usart_obj->EUSART_TxInterruptHandler;
    #endif
    #endif
}

static Std_ReturnType Rx_Interrupt_Configurations(const eusart_t *usart_obj){
    /*Enable EUSART RX interrupt source*/
    #if USART_RX_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
    FramingError_InterruptHandler = usart_obj->EUSART_FramingInterruptHandler;
    OverrunError_InterruptHandler = usart_obj->EUSART_OverrunInterruptHandler;
    #if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
    INTERRUPT_InterruptPriorityEnable();
    INTERRUT_GlobalInterruptHighEnable();
    INTERRUT_GlobalInterruptLowEnable();
    /*set EUSART RX interrupt priority levels*/
    if(usart_obj->interrupt_priority == HIGH_PRIORITY){
        INTERRUPT_USART_RX_HIGH_PRIORITY();
    }
    else if(usart_obj->interrupt_priority == LOW_PRIORITY){
        INTERRUPT_USART_RX_LOW_PRIORITY();
    }
    else{/*Nothing*/}
    INTERRUPT_USART_RX_ENABLE();
    /*clear EUSART RX external interrupt flag*/ 
    INTERRUPT_USART_RX_CLEAR_FLAG();
    /*Intialize the callback function*/
    RX_InterruptHandler = usart_obj->EUSART_RxInterruptHandler;
    #else
    INTERRUPT_InterruptPriorityDisable();
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();
    INTERRUPT_USART_RX_ENABLE();
    /*clear EUSART RX external interrupt flag*/
    INTERRUPT_USART_RX_CLEAR_FLAG();
    /*Intialize the callback function*/
    RX_InterruptHandler = usart_obj->EUSART_RxInterruptHandler;
    #endif
    #endif
}

void EUSART_TX_ISR(void){
    if(TX_InterruptHandler == NULL){
        /*Nothing*/
    }
    else{
        TX_InterruptHandler();
    }
}

void EUSART_RX_ISR(void){
    if(RX_InterruptHandler == NULL){
        /*Nothing*/
    }
    else{
        RX_InterruptHandler();
    }
    if(FramingError_InterruptHandler){
        FramingError_InterruptHandler();
    }
    else{/*Nothing*/}
    if(OverrunError_InterruptHandler){
        OverrunError_InterruptHandler();
    }
    else{/*Nothing*/}
}