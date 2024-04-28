/* 
 * File:   hal_spi.h
 * Author: Mohamed Elsayed
 *
 * Created on March 10, 2024, 6:11 PM
 */

#include "hal_spi.h"

static void (*SPI_InterruptHandler)(void) = NULL;

Std_ReturnType SPI_Init(const SPI_t *spi_obj){
    Std_ReturnType ret = E_OK;
    if(NULL == spi_obj){
        ret = E_NOT_OK;
    }
    else{
        // Disable SPI Module
        SSPCON1bits.SSPEN = SPI_DISABLE_BIT;
        // Set SS pin as output an low-level
        ret = gpio_pin_direction_intialize(&(spi_obj->SS_pin));
        ret = gpio_pin_write_logic(&(spi_obj->SS_pin), GPIO_LOW);
        if(SPI_MASTER_MODE == spi_obj->mode){
            // Set the appropriate TRIS bits for SPI pins
            TRISCbits.RC3 = 0;  // SCK (Serial Clock) as input
            TRISCbits.RC5 = 1;  // SDI (Serial Data Input) as input
            TRISCbits.RC4 = 0;  // SDO (Serial Data Output) as output
        }
        else if(SPI_SLAVE_MODE == spi_obj->mode){
            // Set the appropriate TRIS bits for SPI pins
            TRISCbits.RC3 = 1;  // SCK (Serial Clock) as input
            TRISCbits.RC5 = 1;  // SDI (Serial Data Input) as input
            TRISCbits.RC4 = 0;  // SDO (Serial Data Output) as output
        }
        else{/*Nothing*/}
        // Input data sampled at middle of data output time
        SSPSTATbits.SMP = 0;  
        // Transmit occurs on idle -> active or active -> idle transition 
        SSPSTATbits.CKE = spi_obj->clockPhase; 
        // Idle state for clock is a low-level or high_level
        SSPCON1bits.CKP = spi_obj->clockPolarity; 
        // SPI Slave or Master mode
        SSPCON1bits.SSPM = spi_obj->clockSpeed;  
        // Disable SPI Interrupt
        SPI_INTERRUPT_DISABLE();
        // Enable SPI module
        SSPCON1bits.SSPEN = SPI_ENABLE_BIT; 
        
    }
    return ret;
}

Std_ReturnType SPI_DeInit(const SPI_t *spi_obj){
    Std_ReturnType ret = E_OK;
    if(NULL == spi_obj){
        ret = E_NOT_OK;
    }
    else{
        // Disable SPI Module
        SSPCON1bits.SSPEN = SPI_DISABLE_BIT;
    }
    return ret;
}

Std_ReturnType SPI_Transfer_Blocking(uint8_t dataOut, uint8_t *dataIn){
    Std_ReturnType ret = E_OK;
    if(NULL == dataIn){
        ret = E_NOT_OK;
    }
    else{
        SSPBUF = dataOut;  // Write data to SPI buffer
        while (!SSPSTATbits.BF);  // Wait for the data transmission to complete
        *dataIn = SSPBUF;
    }
    return ret;
}

Std_ReturnType SPI_Transfer_Non_Blocking(const SPI_t *spi_obj, uint8_t dataOut){
    Std_ReturnType ret = E_OK;
    if(NULL == spi_obj){
        ret = E_NOT_OK;
    }
    else{
        #if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
        INTERRUPT_InterruptPriorityEnable();
        INTERRUT_GlobalInterruptHighEnable();
        INTERRUT_GlobalInterruptLowEnable();
        /*set SPI Module interrupt priority levels*/
        if(spi_obj->interrupt_priority == HIGH_PRIORITY){
            SET_HIGH_PRIORITY();
        }
        else if(spi_obj->interrupt_priority == LOW_PRIORITY){
            SET_LOW_PRIORITY();
        }
        SPI_INTERRUPT_ENABLE();
        /*clear SPI Module external interrupt flag*/ 
        SPI_INTERRUPT_FLAG_CLEAR();
        /*Intialize the callback function*/
        SPI_InterruptHandler = spi_obj->SPI_InterruptHandler;
        #else
        INTERRUPT_InterruptPriorityDisable();
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
        SPI_INTERRUPT_ENABLE();
        /*clear SPI Module external interrupt flag*/ 
        SPI_INTERRUPT_FLAG_CLEAR();
        /*Intialize the callback function*/
        SPI_InterruptHandler = spi_obj->SPI_InterruptHandler;
        #endif
        SSPBUF = dataOut;
    }
    return ret;
}

void SPI_ISR(void){
    if(SPI_InterruptHandler){
        SPI_InterruptHandler();
    }
}