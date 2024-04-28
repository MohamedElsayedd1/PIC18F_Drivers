/* 
 * File:   hal_i2c.h
 * Author: Mohamed Elsayed
 *
 * Created on April 15, 2024, 8:28 PM
 */

#ifndef HAL_I2C_H
#define	HAL_I2C_H

/*SECTION : INCLUDES*/
#include "hal_i2c.h"
#include "../GPIO/hal_gpio.h"
#include "../INTERRUPT/mcal_internal_interrupt.h"

/* SECTION : MACROS DECLARATION*/

/* Slew Rate Enable/Disable */
#define MSSP_I2C_ENABLE                        1
#define MSSP_I2C_DISABLE                       0

/* Slew Rate Enable/Disable */
#define I2C_SLEW_RATE_ENABLE                   0
#define I2C_SLEW_RATE_DISABLE                  1

/* SMBus Enable/Disable */
#define I2C_SMBUS_ENABLE                       1
#define I2C_SMBUS_DISABLE                      0

/* Slave Mode Data/Address Indication */
#define I2C_LAST_BYTE_DATA                     1
#define I2C_LAST_BYTE_ADDRESS                  0

/* START bit detection */
#define I2C_START_BIT_DETECTED                 1
#define I2C_START_BIT_NOT_DETECTED             0

/* STOP bit detection */
#define I2C_STOP_BIT_DETECTED                  1
#define I2C_STOP_BIT_NOT_DETECTED              0

/* Receive Enable bit (Master mode only) */
#define I2C_MASTER_RECEIVE_ENABLE              1
#define I2C_MASTER_RECEIVE_DISABLE             0

/* Master Synchronous Serial Port Mode Select bits */
#define I2C_SLAVE_MODE_7BIT                         0x06U
#define I2C_SLAVE_MODE_10BIT                        0x07U
#define I2C_SLAVE_MODE_7BIT_SS_INTERRUPT_ENABLE     0x0EU
#define I2C_SLAVE_MODE_10BIT_SS_INTERRUPT_ENABLE    0x0FU
#define I2C_MASTER_MODE_DEFINED_CLOCK               0x08U
#define I2C_MASTER_MODE_FIRMWARE_CONTROLLED         0x0BU

/* I2C Mode : Master or Slave */
#define I2C_MASTER_MODE                             1
#define I2C_SLAVE_MODE                              0

/* General Call Enable bit (Slave mode only)    */
#define I2C_SLAVE_GENERAL_CALL_ENABLE               1
#define I2C_SLAVE_GENERAL_CALL_DISABLE              0

/* Acknowledge Status bit (Master Transmit mode only) */
#define I2C_ACK_RECEIVED                            0
#define I2C_NOT_ACK_RECEIVED                        1

/* Acknowledge Data bit (Master Receive mode only)( */
#define I2C_MASTER_SEND_ACK                         0
#define I2C_MASTER_SEND_NOT_ACK                     1

/*SECTION : MACRO FUNCTIONS*/

/*SECTION : DATA TYPE DECLARATION*/
typedef struct{
    uint8_t i2c_slave_address;                              /* I2C Slave Address */
    uint8_t i2c_mode_cfg : 4;                               /* Master Synchronous Serial Port Mode Select bits */
    uint8_t i2c_mode : 1;                                   /* I2C Mode : Master or Slave */
    uint8_t i2c_SMBus_select : 1;                           /* SMBus Enable/Disable */
    uint8_t i2c_slew_rate : 1;                              /* Slew Rate Enable/Disable */
    uint8_t i2c_general_call : 1;                           /* General Call Enable/Disable */
    uint8_t i2c_master_receive_mode : 1;                    /* Master Receive mode Enable/Disable */
    interrupt_prioity_t interrupt_prority;
    uint8_t i2c_reserved : 6;
}i2c_config_t;

typedef struct{
    uint32_t i2c_clock;                                 /* Master clock frequency */
    void (*I2C_Report_Write_Collision)(void);           /* Write collision call-back function */
    void (*I2C_Report_Read_OverFlow)(void);             /* Read Overflow call-back function */
    void (*I2C_DefaultInterruptHandler)(void);          /* Default Interrupt Handler */
    i2c_config_t i2c_cfg;
}mssp_i2c_t;


/*FUNCTIONS DECLARATIONS*/
Std_ReturnType MSSP_I2C_Init(const mssp_i2c_t *i2c_obj);
Std_ReturnType MSSP_I2C_DeInit(const mssp_i2c_t *i2c_obj);
Std_ReturnType MSSP_I2C_Master_Send_Start(const mssp_i2c_t *i2c_obj);
Std_ReturnType MSSP_I2C_Master_Send_Repeated_Start(const mssp_i2c_t *i2c_obj);
Std_ReturnType MSSP_I2C_Master_Send_Stop(const mssp_i2c_t *i2c_obj);
Std_ReturnType MSSP_I2C_Read(const mssp_i2c_t *i2c_obj, uint8_t *Data);
Std_ReturnType MSSP_I2C_Write(const mssp_i2c_t *i2c_obj, uint8_t Data, uint8_t ack);
Std_ReturnType MSSP_I2C_Master_Read_Blocking(const mssp_i2c_t *i2c_obj, uint8_t *Data, uint8_t ack);
Std_ReturnType MSSP_I2C_Master_Write_Blocking(const mssp_i2c_t *i2c_obj, uint8_t Data, uint8_t *ack);
Std_ReturnType MSSP_I2C_Master_Read_1byte_register_Blocking(const mssp_i2c_t *i2c_obj, uint8_t address, uint8_t reg, uint8_t *Data);

#endif	/* HAL_I2C_H */

 