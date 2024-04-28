/* 
 * File:   mcal_interrupt_manager.c
 * Author: Mohamed Elsayed
 *
 * Created on January 5, 2024, 3:13 PM
 */
#include "mcal_inerrupt_manager.h"

static uint8_t volatile RB4_flag = 1, RB5_flag = 1, RB6_flag = 1, RB7_flag = 1;

#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
void __interrupt() HighPriority_ISR(void){
    if((INTCONbits.INT0IE == INTERRUPT_ENABLE) && (INTCONbits.INT0F == INTERRUPT_OCCUR)){
        INT0_ISR();
    }  
    /*PORTB external on-change interrupt*/
    if((INTCONbits.RBIE == INTERRUPT_ENABLE) && (INTCONbits.RBIF == INTERRUPT_OCCUR) && (PORTBbits.RB4 == GPIO_HIGH) && (RB4_flag == 1)){
        RB4_flag = 0;
        RB4_ISR(1);
    }
    else{/*Nothing*/}
    if((INTCONbits.RBIE == INTERRUPT_ENABLE) && (INTCONbits.RBIF == INTERRUPT_OCCUR) && (PORTBbits.RB4 == GPIO_LOW)&& (RB4_flag == 0)){
        RB4_flag = 1;
        RB4_ISR(0);
    }
    else{/*Nothing*/}
    
    if((INTCONbits.RBIE == INTERRUPT_ENABLE) && (INTCONbits.RBIF == INTERRUPT_OCCUR) && (PORTBbits.RB5 == GPIO_HIGH) && (RB5_flag == 1)){
        RB5_flag = 0;
        RB5_ISR(1);
    }
    else{/*Nothing*/}
    if((INTCONbits.RBIE == INTERRUPT_ENABLE) && (INTCONbits.RBIF == INTERRUPT_OCCUR) && (PORTBbits.RB5 == GPIO_LOW)&& (RB5_flag == 0)){
        RB5_flag = 1;
        RB5_ISR(0);
    }
    else{/*Nothing*/}
    
    if((INTCONbits.RBIE == INTERRUPT_ENABLE) && (INTCONbits.RBIF == INTERRUPT_OCCUR) && (PORTBbits.RB6 == GPIO_HIGH) && (RB6_flag == 1)){
        RB6_flag = 0;
        RB6_ISR(1);
    }
    else{/*Nothing*/}
    if((INTCONbits.RBIE == INTERRUPT_ENABLE) && (INTCONbits.RBIF == INTERRUPT_OCCUR) && (PORTBbits.RB6 == GPIO_LOW)&& (RB6_flag == 0)){
        RB6_flag = 1;
        RB6_ISR(0);
    }
    else{/*Nothing*/}
    
    if((INTCONbits.RBIE == INTERRUPT_ENABLE) && (INTCONbits.RBIF == INTERRUPT_OCCUR) && (PORTBbits.RB7 == GPIO_HIGH) && (RB7_flag == 1)){
        RB7_flag = 0;
        RB7_ISR(1);
    }
    else{/*Nothing*/}
   
    /*PORTB external on-change interrupt*/
    if((PIR1bits.ADIF == INTERRUPT_ENABLE) && (PIE1bits.ADIE == INTERRUPT_OCCUR)){
        ADC_ISR();
    }
    else{/*Nothing*/}
    
    /*TIMER0 interrupt source*/
    if((INTCONbits.TMR0IE == INTERRUPT_ENABLE) && (INTCONbits.TMR0IF == INTERRUPT_OCCUR)){
        TMR0_ISR();
    }
    else{/*Nothing*/}
    
    /*MSSP I2C Transmit & Receive interrupt source*/
    if((PIE1bits.SSPIE == INTERRUPT_ENABLE) && (PIR1bits.SSPIF == INTERRUPT_OCCUR)){
        MSSP_I2C_ISR();
    }
    else{/*Nothing*/}
    
    /*MSSP I2C Bus Collision interrupt source*/
    if((PIE2bits.BCLIE == INTERRUPT_ENABLE) && (PIR2bits.BCLIF == INTERRUPT_OCCUR)){
        MSSP_I2C_BUS_COLLISION_ISR();
    }
    else{/*Nothing*/}
}

void __interrupt(low_priority)  LowPriority_ISR(void){
    if((INTCON3bits.INT1E == INTERRUPT_ENABLE) && (INTCON3bits.INT1F == INTERRUPT_OCCUR)){
        INT1_ISR();
    }
    else if((INTCON3bits.INT2IE == INTERRUPT_ENABLE) && (INTCON3bits.INT2IF == INTERRUPT_OCCUR)){
        INT2_ISR();
    }
    
    /*TIMER1 interrupt source*/
    if((PIE1bits.TMR1IE == INTERRUPT_ENABLE) && (PIR1bits.TMR1IF == INTERRUPT_OCCUR)){
        TMR1_ISR();
    }
    else{/*Nothing*/}
    
    /*TIMER2 interrupt source*/
    if((PIE1bits.TMR2IE == INTERRUPT_ENABLE) && (PIR1bits.TMR2IF == INTERRUPT_OCCUR)){
        TMR2_ISR();
    }
    else{/*Nothing*/}
    
    /*TIMER2 interrupt source*/
    if((PIE2bits.TMR3IE == INTERRUPT_ENABLE) && (PIR2bits.TMR3IF == INTERRUPT_OCCUR)){
        TMR3_ISR();
    }
    else{/*Nothing*/}
    
    /*CCP1 interrupt source*/
    if((PIE1bits.CCP1IE == INTERRUPT_ENABLE) && (PIR1bits.CCP1IF == INTERRUPT_OCCUR)){
        CCP1_ISR();
    }
    else{/*Nothing*/}
    
    /*CCP2 interrupt source*/
    if((PIE2bits.CCP2IE == INTERRUPT_ENABLE) && (PIR2bits.CCP2IF == INTERRUPT_OCCUR)){
        CCP2_ISR();
    }
    else{/*Nothing*/}
    
    /*EUSART TX interrupt source*/
    if((PIE1bits.TXIE == INTERRUPT_ENABLE) && (PIR1bits.TXIF == INTERRUPT_OCCUR)){
        EUSART_TX_ISR();
    }
    else{/*Nothing*/}
    
    /*EUSART RX interrupt source*/
    if((PIE1bits.RCIE == INTERRUPT_ENABLE) && (PIR1bits.RCIF == INTERRUPT_OCCUR)){
        EUSART_RX_ISR();
    }
    else{/*Nothing*/}
    
    /*SPI interrupt source*/
    if((PIE1bits.SSPIE == INTERRUPT_ENABLE) && (PIR1bits.SSPIF == INTERRUPT_OCCUR)){
        SPI_ISR();
    }
    else{/*Nothing*/}
    
    /*MSSP I2C Transmit & Receive interrupt source*/
    if((PIE1bits.SSPIE == INTERRUPT_ENABLE) && (PIR1bits.SSPIF == INTERRUPT_OCCUR)){
        MSSP_I2C_ISR();
    }
    else{/*Nothing*/}
    
    /*MSSP I2C Bus Collision interrupt source*/
    if((PIE2bits.BCLIE == INTERRUPT_ENABLE) && (PIR2bits.BCLIF == INTERRUPT_OCCUR)){
        MSSP_I2C_BUS_COLLISION_ISR();
    }
    else{/*Nothing*/}
}
#elif INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_DISABLE
void __interrupt() HighPriority_ISR(void){
    if((INTCONbits.INT0IE == INTERRUPT_ENABLE) && (INTCONbits.INT0F == INTERRUPT_OCCUR)){
        INT0_ISR();
    }
    if((INTCON3bits.INT1E == INTERRUPT_ENABLE) && (INTCON3bits.INT1F == INTERRUPT_OCCUR)){
        INT1_ISR();
    }
    if((INTCON3bits.INT2IE == INTERRUPT_ENABLE) && (INTCON3bits.INT2IF == INTERRUPT_OCCUR)){
        INT2_ISR();
    }
    
    /*PORTB external on-change interrupt*/
    if((INTCONbits.RBIE == INTERRUPT_ENABLE) && (INTCONbits.RBIF == INTERRUPT_OCCUR) && (PORTBbits.RB4 == GPIO_HIGH) && (RB4_flag == 1)){
        RB4_flag = 0;
        RB4_ISR(1);
    }
    else{/*Nothing*/}
    if((INTCONbits.RBIE == INTERRUPT_ENABLE) && (INTCONbits.RBIF == INTERRUPT_OCCUR) && (PORTBbits.RB4 == GPIO_LOW)&& (RB4_flag == 0)){
        RB4_flag = 1;
        RB4_ISR(0);
    }
    else{/*Nothing*/}
    
    if((INTCONbits.RBIE == INTERRUPT_ENABLE) && (INTCONbits.RBIF == INTERRUPT_OCCUR) && (PORTBbits.RB5 == GPIO_HIGH) && (RB5_flag == 1)){
        RB5_flag = 0;
        RB5_ISR(1);
    }
    else{/*Nothing*/}
    if((INTCONbits.RBIE == INTERRUPT_ENABLE) && (INTCONbits.RBIF == INTERRUPT_OCCUR) && (PORTBbits.RB5 == GPIO_LOW)&& (RB5_flag == 0)){
        RB5_flag = 1;
        RB5_ISR(0);
    }
    else{/*Nothing*/}
    
    if((INTCONbits.RBIE == INTERRUPT_ENABLE) && (INTCONbits.RBIF == INTERRUPT_OCCUR) && (PORTBbits.RB6 == GPIO_HIGH) && (RB6_flag == 1)){
        RB6_flag = 0;
        RB6_ISR(1);
    }
    else{/*Nothing*/}
    if((INTCONbits.RBIE == INTERRUPT_ENABLE) && (INTCONbits.RBIF == INTERRUPT_OCCUR) && (PORTBbits.RB6 == GPIO_LOW)&& (RB6_flag == 0)){
        RB6_flag = 1;
        RB6_ISR(0);
    }
    else{/*Nothing*/}
    
    if((INTCONbits.RBIE == INTERRUPT_ENABLE) && (INTCONbits.RBIF == INTERRUPT_OCCUR) && (PORTBbits.RB7 == GPIO_HIGH) && (RB7_flag == 1)){
        RB7_flag = 0;
        RB7_ISR(1);
    }
    else{/*Nothing*/}
    if((INTCONbits.RBIE == INTERRUPT_ENABLE) && (INTCONbits.RBIF == INTERRUPT_OCCUR) && (PORTBbits.RB7 == GPIO_LOW)&& (RB7_flag == 0)){
        RB7_flag = 1;
        RB7_ISR(0);
    }
    else{/*Nothing*/}
   
    /*PORTB external on-change interrupt*/
    if((PIR1bits.ADIF == INTERRUPT_ENABLE) && (PIE1bits.ADIE == INTERRUPT_OCCUR)){
        ADC_ISR();
    }
    
    /*TIMER0 interrupt source*/
    if((INTCONbits.TMR0IE == INTERRUPT_ENABLE) && (INTCONbits.TMR0IF == INTERRUPT_OCCUR)){
        TMR0_ISR();
    }
    else{/*Nothing*/}
    
    /*TIMER1 interrupt source*/
    if((PIE1bits.TMR1IE == INTERRUPT_ENABLE) && (PIR1bits.TMR1IF == INTERRUPT_OCCUR)){
        TMR1_ISR();
    }
    else{/*Nothing*/}
    
    /*TIMER2 interrupt source*/
    if((PIE1bits.TMR2IE == INTERRUPT_ENABLE) && (PIR1bits.TMR2IF == INTERRUPT_OCCUR)){
        TMR2_ISR();
    }
    else{/*Nothing*/}
    
    /*TIMER3 interrupt source*/
    if((PIE2bits.TMR3IE == INTERRUPT_ENABLE) && (PIR2bits.TMR3IF == INTERRUPT_OCCUR)){
        TMR3_ISR();
    }
    else{/*Nothing*/}
    
    /*CCP1 interrupt source*/
    if((PIE1bits.CCP1IE == INTERRUPT_ENABLE) && (PIR1bits.CCP1IF == INTERRUPT_OCCUR)){
        CCP1_ISR();
    }
    else{/*Nothing*/}
    
    /*CCP2 interrupt source*/
    if((PIE2bits.CCP2IE == INTERRUPT_ENABLE) && (PIR2bits.CCP2IF == INTERRUPT_OCCUR)){
        CCP2_ISR();
    }
    else{/*Nothing*/}
    
    /*EUSART TX interrupt source*/
    if((PIE1bits.TXIE == INTERRUPT_ENABLE) && (PIR1bits.TXIF == INTERRUPT_OCCUR)){
        EUSART_TX_ISR();
    }
    else{/*Nothing*/}
    
    /*EUSART RX interrupt source*/
    if((PIE1bits.RCIE == INTERRUPT_ENABLE) && (PIR1bits.RCIF == INTERRUPT_OCCUR)){
        EUSART_RX_ISR();
    }
    else{/*Nothing*/}
    
    /*SPI interrupt source*/
    if((PIE1bits.SSPIE == INTERRUPT_ENABLE) && (PIR1bits.SSPIF == INTERRUPT_OCCUR)){
        SPI_ISR();
    }
    else{/*Nothing*/}
    
    /*MSSP I2C Transmit & Receive interrupt source */
    /*
    if((PIE1bits.SSPIE == INTERRUPT_ENABLE) && (PIR1bits.SSPIF == INTERRUPT_OCCUR)){
        MSSP_I2C_ISR();
    }
    else{}*/
    

    /* MSSP I2C Bus Collision interrupt source */
    /*
    if((PIE2bits.BCLIE == INTERRUPT_ENABLE) && (PIR2bits.BCLIF == INTERRUPT_OCCUR)){
        MSSP_I2C_BUS_COLLISION_ISR();
    }
    else{}
    */
}
#endif




