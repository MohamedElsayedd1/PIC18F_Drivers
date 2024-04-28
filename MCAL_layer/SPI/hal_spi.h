/* 
 * File:   hal_spi.h
 * Author: Mohamed Elsayed
 *
 * Created on March 10, 2024, 6:11 PM
 */

#ifndef HAL_SPI_H
#define	HAL_SPI_H

/*SECTION : INCLUDES*/
#include "pic18f4620.h"
#include "../../MCAL_layer/std_types.h"
#include "../../MCAL_layer/device_config.h" 
#include "../../MCAL_layer/INTERRUPT/mcal_internal_interrupt.h"
#include "../../MCAL_layer/GPIO/hal_gpio.h"

/* SECTION : MACROS DECLARATION*/
// Define to indicate SPI master mode
#define SPI_MASTER_MODE                 1
// Define to indicate SPI slave mode
#define SPI_SLAVE_MODE                  0
// Clock is idle low
#define SPI_CLOCK_IDLE_LOW              0   
// Clock is idle high
#define SPI_CLOCK_IDLE_HIGH             1   
// Data is sampled on the leading edge of the clock
#define SPI_CLOCK_PHASE_EDGE_LEADING    0   
// Data is sampled on the trailing edge of the clock
#define SPI_CLOCK_PHASE_EDGE_TRAILING   1   
// SPI enable
#define SPI_ENABLE_BIT                  1 
// SPI Disable
#define SPI_DISABLE_BIT                 0
// SPI Slave mode, clock = SCK pin, SS pin control disabled
#define SPI_MODE_SLAVE_SCK_SS_DISABLED  0b0101  
// SPI Slave mode, clock = SCK pin, SS pin control enabled
#define SPI_MODE_SLAVE_SCK_SS_ENABLED   0b0100   
// SPI Master mode, clock = TMR2 output/2
#define SPI_MODE_MASTER_TMR2_2          0b0011 
// SPI Master mode, clock = FOSC/64
#define SPI_MODE_MASTER_FOSC_64         0b0010  
// SPI Master mode, clock = FOSC/16
#define SPI_MODE_MASTER_FOSC_16         0b0001
// SPI Master mode, clock = FOSC/4
#define SPI_MODE_MASTER_FOSC_4          0b0000   

/*SECTION : MACRO FUNCTIONS*/
#define SPI_INTERRUPT_ENABLE()          (PIE1bits.SSPIE = 1)  // Enable SPI interrupt
#define SPI_INTERRUPT_DISABLE()         (PIE1bits.SSPIE = 0)  // Disable SPI interrupt

#define SPI_INTERRUPT_FLAG_CLEAR()      (PIR1bits.SSPIF = 0)  // Clear SPI interrupt flag

#define SET_LOW_PRIORITY()              (IPR1bits.SSPIP = 1)  // Enable low priority interrupts
#define SET_HIGH_PRIORITY()             (IPR1bits.SSPIP = 0)  // Enable high priority interrupts
    
/*SECTION : DATA TYPE DECLARATION*/
typedef struct {
    void (*SPI_InterruptHandler)(void);
    pin_config_t SS_pin;          // The pin configuration that is connected to ss
    uint8_t clockSpeed;      // Clock speed in Hz
    uint8_t mode : 1;            // SPI mode (master/slave)
    uint8_t clockPolarity : 1;   // Clock polarity (idle low/high)
    uint8_t clockPhase : 1;      // Clock phase (leading/trailing edge)
    uint8_t interrupt_priority : 1; 
}SPI_t;

/*FUNCTIONS DECLARATIONS*/
Std_ReturnType SPI_Init(const SPI_t *spi_obj);
Std_ReturnType SPI_DeInit(const SPI_t *spi_obj);
Std_ReturnType SPI_Transfer_Blocking(uint8_t dataOut, uint8_t *dataIn);
Std_ReturnType SPI_Transfer_Non_Blocking(const SPI_t *spi_obj, uint8_t dataIn);
#endif	/* HAL_SPI_H */

