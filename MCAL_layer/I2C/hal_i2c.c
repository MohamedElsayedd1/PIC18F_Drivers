/* 
 * File:   hal_i2c.h
 * Author: Mohamed Elsayed
 *
 * Created on April 15, 2024, 8:28 PM
 */

#include "hal_i2c.h"

#if MSSP_I2C_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
static void (*I2C_Report_Write_Collision)(void) = NULL;
static void (*I2C_Report_Read_OverFlow)(void) = NULL;
static void (*I2C_DefaultInterruptHandler)(void) = NULL;
#endif

static void MSSP_I2C_Interrupt_Configurations(const mssp_i2c_t *i2c_obj);

Std_ReturnType MSSP_I2C_Init(const mssp_i2c_t *i2c_obj){
    Std_ReturnType ret = E_OK;
    if(NULL == i2c_obj){
        ret = E_NOT_OK;
    }
    else{
        /* Disable MSSP I2C Module*/
        SSPCON1bits.SSPEN = MSSP_I2C_DISABLE;
        /* Master & Slave GPIO Configure as inputs */
        TRISCbits.RC3 = 1;
        TRISCbits.RC4 = 1;
        /* Clear the Bus Collision Detect Bit */
        SSPCON1bits.WCOL = 0;   /* No Collision */
        /* Clear the Receive Overflow Indicator bit */
        SSPCON1bits.SSPOV = 0; /* No Overflow */
        /* MSSP select mode*/
        if(i2c_obj->i2c_cfg.i2c_mode == I2C_MASTER_MODE){
            /* MSSP Master Mode Configurations */
            SSPCON1bits.SSPM = i2c_obj->i2c_cfg.i2c_mode_cfg;
            /* Master Mode Clock Configurations */
            SSPADD = (uint8_t)((_XTAL_FREQ / (4.0 * (i2c_obj->i2c_clock))) - 1);
        }
        else if(i2c_obj->i2c_cfg.i2c_mode == I2C_SLAVE_MODE){
            /* MSSP Slave Mode Configurations */
            SSPCON1bits.SSPM = i2c_obj->i2c_cfg.i2c_mode_cfg; 
            /* Slave General Call Configurations */
            SSPCON2bits.GCEN = i2c_obj->i2c_cfg.i2c_general_call;
            /* Release the clock */
            SSPCON1bits.CKP = 1;
            /* Assign the I2C Slave address */
            SSPADD = i2c_obj->i2c_cfg.i2c_slave_address;
        }
        /* I2C Slew Rate Configurations */
        SSPSTATbits.SMP = i2c_obj->i2c_cfg.i2c_slew_rate;
        /* I2C SMBus Configurations */
        SSPSTATbits.CKE = i2c_obj->i2c_cfg.i2c_SMBus_select;
        /* MSSP I2C interrupt configurations */
        MSSP_I2C_Interrupt_Configurations(i2c_obj);
        /* Enable MSSP I2C Module*/
        SSPCON1bits.SSPEN = MSSP_I2C_ENABLE;
    }
    return ret;
}

Std_ReturnType MSSP_I2C_DeInit(const mssp_i2c_t *i2c_obj){
    Std_ReturnType ret = E_OK;
    if(NULL == i2c_obj){
        ret = E_NOT_OK;
    }
    else{
        /* Disable MSSP I2C Module*/
        SSPCON1bits.SSPEN = MSSP_I2C_DISABLE;
        /* MSSP I2C interrupt configurations */
        #if MSSP_I2C_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
                INTERRUPT_MSSP_I2C_DISABLE();
                INTERRUPT_MSSP_I2C_CLEAR_FLAG();
                INTERRUPT_MSSP_I2C_BUS_COLLISION_DISABLE();
                INTERRUPT_MSSP_I2C_BUS_COLLISION_CLEAR_FLAG();
        #if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
                INTERRUT_GlobalInterruptHighDisable();
                INTERRUT_GlobalInterruptLowDisable();
                INTERRUPT_InterruptPriorityDisable();
        #elif INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_DISABLE
                INTERRUPT_InterruptPriorityDisable();
                INTERRUPT_GlobalInterruptDisable();
                INTERRUPT_PeripheralInterruptDisable();
        #endif
        #endif
    }
    return ret;
}

Std_ReturnType MSSP_I2C_Master_Send_Start(const mssp_i2c_t *i2c_obj){
    Std_ReturnType ret = E_OK;
    if(NULL == i2c_obj){
        ret = E_NOT_OK;
    }
    else{
        /* Initiate Start Condition */
        SSPCON2bits.SEN = 1;            /* Initiates Start condition on SDA and SCL pins. Automatically cleared by hardware. */
        /* Check if Start Condition has been sent */
        while(!(SSPSTATbits.S));
        /* Clear The SSPIF Interrupt flag Due To Start Condition */
        PIR1bits.SSPIF = 0;
        /* Report The Start Condition Detection */
        if(I2C_START_BIT_DETECTED != SSPSTATbits.S){
            ret = E_NOT_OK;
        }
        else{
            /* Nothing */
        }
    }
    return ret;
}

Std_ReturnType MSSP_I2C_Master_Send_Repeated_Start(const mssp_i2c_t *i2c_obj){
    Std_ReturnType ret = E_OK;
    if(NULL == i2c_obj){
        ret = E_NOT_OK;
    }
    else{
        /* Initiate Repeated START Condition */
        SSPCON2bits.RSEN = 1;            /* Initiates Start condition on SDA and SCL pins. Automatically cleared by hardware. */
        /* Check if Repeated START Condition has been sent */
        while(SSPCON2bits.RSEN);
        /* Clear The SSPIF Interrupt flag Due To Start Condition */
        PIR1bits.SSPIF = 0;
    }
    return ret;
}

Std_ReturnType MSSP_I2C_Master_Send_Stop(const mssp_i2c_t *i2c_obj){
    Std_ReturnType ret = E_OK;
    if(NULL == i2c_obj){
        ret = E_NOT_OK;
    }
    else{
        /* Initiate STOP Condition */
        SSPCON2bits.PEN = 1;            /* Initiates Start condition on SDA and SCL pins. Automatically cleared by hardware. */
        /* Check if STOP Condition has been sent */
        while(!(SSPSTATbits.P));
        /* Clear The SSPIF Interrupt flag Due To Start Condition */
        PIR1bits.SSPIF = 0;
        /* Report The STOP Condition Detection */
        if(I2C_STOP_BIT_DETECTED != SSPSTATbits.P){
            ret = E_NOT_OK;
        }
        else{
            /* Nothing */
        }
    }
    return ret;
}

Std_ReturnType MSSP_I2C_Master_Read_Blocking(const mssp_i2c_t *i2c_obj, uint8_t *Data, uint8_t ack){
    Std_ReturnType ret = E_OK;
    if(NULL == i2c_obj || NULL == Data){
        ret = E_NOT_OK;
    }
    else{
        /* Master Mode Receive enable bit*/
        SSPCON2bits.RSEN = I2C_MASTER_RECEIVE_ENABLE;
        /* Check if a complete byte has been received */
        while(!(SSPSTATbits.BF));      /* Wait till the flag "SSPIF" is set */
        /* Copy the data stored in SSPBUF */
        *Data = SSPBUF;
        /* Send ACK or NOT-ACK*/
        SSPCON2bits.ACKEN = 1;
        SSPCON2bits.ACKDT = ack;
    }
    return ret;
}

Std_ReturnType MSSP_I2C_Master_Write_Blocking(const mssp_i2c_t *i2c_obj, uint8_t Data, uint8_t *ack){
    Std_ReturnType ret = E_OK;
    if(NULL == i2c_obj || NULL == ack){
        ret = E_NOT_OK;
    }
    else{
        /* Write Data To The SSPBUF Register */
        SSPBUF = Data;
        /* Check If The Data Has Been Written */
        while(!(PIR1bits.SSPIF));       /* Wait till the flag "SSPIF" is set */
        /* Clear The SSPIF Interrupt Flag */
        PIR1bits.SSPIF = 0;
        /* Report The Acknowledge Received By Slave-Receiver */
        if(I2C_ACK_RECEIVED == SSPCON2bits.ACKSTAT){
            *ack = I2C_ACK_RECEIVED;
        }
        else{
            *ack = I2C_NOT_ACK_RECEIVED;
        }
    }
    return ret;
}

Std_ReturnType MSSP_I2C_Read(const mssp_i2c_t *i2c_obj, uint8_t *Data){
    
}

Std_ReturnType MSSP_I2C_Write(const mssp_i2c_t *i2c_obj, uint8_t Data, uint8_t ack){
    
}
/* Slave mode interrupt service routine 
void MSSP_I2C_ISR(void){
    INTERRUPT_MSSP_I2C_CLEAR_FLAG();
    if(I2C_DefaultInterruptHandler){
        I2C_DefaultInterruptHandler();
    }
}

void MSSP_I2C_BUS_COLLISION_ISR(void){
    INTERRUPT_MSSP_I2C_BUS_COLLISION_CLEAR_FLAG();
    if(I2C_Report_Write_Collision){
        I2C_Report_Write_Collision();
    }
}
 */ 

static void MSSP_I2C_Interrupt_Configurations(const mssp_i2c_t *i2c_obj){
    #if MSSP_I2C_INTERRUPT_FEATURE == INTERRUPT_FEATURE_ENABLE
            INTERRUPT_MSSP_I2C_ENABLE();
            INTERRUPT_MSSP_I2C_CLEAR_FLAG();
            INTERRUPT_MSSP_I2C_BUS_COLLISION_ENABLE();
            INTERRUPT_MSSP_I2C_BUS_COLLISION_CLEAR_FLAG();
            I2C_DefaultInterruptHandler = i2c_obj->I2C_DefaultInterruptHandler;
            I2C_Report_Read_OverFlow = i2c_obj->I2C_Report_Read_OverFlow;
            I2C_Report_Write_Collision = i2c_obj->I2C_Report_Write_Collision;
    #if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
            INTERRUT_GlobalInterruptHighEnable();
            INTERRUT_GlobalInterruptLowEnable();
            INTERRUPT_InterruptPriorityEnable();
            INTERRUPT_MSSP_I2C_LOW_PRIORITY();
            /*set MSSP I2C Module interrupt priority levels*/
            if(i2c_obj->i2c_cfg.interrupt_prority == HIGH_PRIORITY){
                INTERRUPT_MSSP_I2C_HIGH_PRIORITY();
                INTERRUPT_MSSP_I2C_BUS_COLLISION_HIGH_PRIORITY();
            }
            else if(i2c_obj->i2c_cfg.interrupt_prority == LOW_PRIORITY){
                INTERRUPT_MSSP_I2C_LOW_PRIORITY();
                INTERRUPT_MSSP_I2C_BUS_COLLISION_LOW_PRIORITY()
            }
    #elif INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_DISABLE
            INTERRUPT_InterruptPriorityDisable();
            INTERRUPT_GlobalInterruptEnable();
            INTERRUPT_PeripheralInterruptEnable();
    #endif
    #endif
}

Std_ReturnType MSSP_I2C_Master_Read_1byte_register_Blocking(const mssp_i2c_t *i2c_obj, uint8_t address, uint8_t reg, uint8_t *Data){
    Std_ReturnType ret = E_OK;
    uint8_t rec_ack = 0;
    if(NULL == i2c_obj || NULL == Data){
        ret = E_NOT_OK;
    }
    else{
        ret =  MSSP_I2C_Master_Send_Start(i2c_obj);
        ret = MSSP_I2C_Master_Write_Blocking(i2c_obj, address, &rec_ack);
        ret = MSSP_I2C_Master_Write_Blocking(i2c_obj, reg, &rec_ack);
        ret = MSSP_I2C_Master_Send_Repeated_Start(i2c_obj);
        ret = MSSP_I2C_Master_Write_Blocking(i2c_obj, address+1, &rec_ack);
        ret = MSSP_I2C_Master_Read_Blocking(i2c_obj, Data, 0);
        ret = MSSP_I2C_Master_Send_Stop(i2c_obj);
    }
    return ret;
}