/* 
 * File:   mcal_external_interrupt.c
 * Author: Mohamed Elsayed
 *
 * Created on January 5, 2024, 3:13 PM
 */

#include "mcal_external_interrupt.h"
#include "mcal_interrupt_gen_cfg.h"

/*CallBack funtions for interrupts sources (INT0 _ INT1 _ INT2)*/
static InterruptHandler INT0_InterruptHandler = NULL;
static InterruptHandler INT1_InterruptHandler = NULL;
static InterruptHandler INT2_InterruptHandler = NULL;

/*CallBack funtions for interrupts sources (RB4 _ RB5 _ RB6 _ RB7)*/
static InterruptHandler RB4_high_InterruptHandler = NULL;
static InterruptHandler RB4_low_InterruptHandler = NULL;
static InterruptHandler RB5_high_InterruptHandler = NULL;
static InterruptHandler RB5_low_InterruptHandler = NULL;
static InterruptHandler RB6_high_InterruptHandler = NULL;
static InterruptHandler RB6_low_InterruptHandler = NULL;
static InterruptHandler RB7_high_InterruptHandler = NULL;
static InterruptHandler RB7_low_InterruptHandler  = NULL;

/*Set the CallBack funtions for every interrupts source (INT0 _ INT1 _ INT2)*/
static Std_ReturnType INT0_SetInterruptHandler(void (*InterruptHandler)(void));
static Std_ReturnType INT1_SetInterruptHandler(void (*InterruptHandler)(void));
static Std_ReturnType  INT2_SetInterruptHandler(void (*InterruptHandler)(void));

/*Set the CallBack funtions for every interrupts source (RB4 _ RB5 _ RB6 _ RB7)*/
static Std_ReturnType RB4_SetInterruptHandler(const interrupt_RBx_t *int_obj);
static Std_ReturnType RB5_SetInterruptHandler(const interrupt_RBx_t *int_obj);
static Std_ReturnType RB6_SetInterruptHandler(const interrupt_RBx_t *int_obj);
static Std_ReturnType RB7_SetInterruptHandler(const interrupt_RBx_t *int_obj);

/*Helper function to set the CallBack functions*/
static Std_ReturnType  Interrupt_INTx_SetInterruptHandler(const interrupt_INTx_t *int_obj);

/*Helper function to set the CallBack functions*/
static Std_ReturnType  Interrupt_RBx_SetInterruptHandler(const interrupt_RBx_t *int_obj);

/*Helper function for INTx interrupt sources*/
static Std_ReturnType Interrupt_INTx_Enable(const interrupt_INTx_t *int_obj);
static Std_ReturnType Interrupt_INTx_Disable(const interrupt_INTx_t *int_obj);
static Std_ReturnType Interrupt_INTx_Set_Edge(const interrupt_INTx_t *int_obj);
static Std_ReturnType Interrupt_INTx_Set_Pin(const interrupt_INTx_t *int_obj);
static Std_ReturnType Interrupt_INTx_Clear_Flag(const interrupt_INTx_t *int_obj);
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_PRIORITY_ENABLE
static Std_ReturnType Interrupt_INTx_Set_Priority(const interrupt_INTx_t *int_obj);
#endif

/*Helper function for RBx interrupt sources*/
static Std_ReturnType Interrupt_RBx_Enable(const interrupt_RBx_t *int_obj);
static Std_ReturnType Interrupt_RBx_Disable(const interrupt_RBx_t *int_obj);
static Std_ReturnType Interrupt_RBx_Set_Priority(const interrupt_RBx_t *int_obj);
static Std_ReturnType Interrupt_RBx_Set_Pin(const interrupt_RBx_t *int_obj);

#if EXTERNAL_INTERRUPT_INTx_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
Std_ReturnType Interrupt_INTx_init(const interrupt_INTx_t *int_obj){
    Std_ReturnType ret = E_OK;
    if(int_obj == NULL){
        ret = E_NOT_OK;
    }
    else{
        /*Disable external interrupt*/
        ret =  Interrupt_INTx_Disable(int_obj);
        /*Clear the interrupt flag : External interrupt didn't occur*/
        ret &= Interrupt_INTx_Clear_Flag(int_obj);
        /*Configure the external interrupt I/O pin : intialize the direction to input*/
        ret &= Interrupt_INTx_Set_Pin(int_obj);
        /*Configure the external interrupt edge*/
        ret &= Interrupt_INTx_Set_Edge(int_obj);
        /*Configure default interrupt CallBack function*/
        ret &= Interrupt_INTx_SetInterruptHandler(int_obj);
        /*Enable external interrupt*/
        ret &=  Interrupt_INTx_Enable(int_obj);
        /*Configure the external interrupt priority*/
        #if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_PRIORITY_ENABLE
        INTERRUPT_InterruptPriorityEnable();
        ret &= Interrupt_INTx_Set_Priority(int_obj);
        #endif
    }
    return ret;
}

void INT0_ISR(void){
    /*The flag must be cleared in software*/
    INTERRUPT_INT0_CLEAR_FLAG();
    /*Execute some code*/
    
    /*Call the CallBack function "gets executed everytime this ISR executes"*/
    if(INT0_InterruptHandler != NULL){
        INT0_InterruptHandler();
    } 
}

void INT1_ISR(void){
    /*The flag must be cleared in software*/
    INTERRUPT_INT1_CLEAR_FLAG();
    /*Execute some code*/
    
    /*Call the CallBack function "gets executed everytime this ISR executes"*/
    if(INT1_InterruptHandler != NULL){
        INT1_InterruptHandler();
    } 
}

void INT2_ISR(void){
    /*The flag must be cleared in software*/
    INTERRUPT_INT2_CLEAR_FLAG();
    /*Execute some code*/
    
    /*Call the CallBack function "gets executed everytime this ISR executes"*/
    if(INT2_InterruptHandler != NULL){
        INT2_InterruptHandler();
    } 
}

Std_ReturnType Interrupt_INTx_Dinit(const interrupt_INTx_t *int_obj){
        Std_ReturnType ret = E_OK;
    if(int_obj == NULL){
        ret = E_NOT_OK;
    }
    else{
        ret =  Interrupt_INTx_Disable(int_obj);
    }
    return ret;
}

Std_ReturnType Interrupt_RBx_init(const interrupt_RBx_t *int_obj){
    Std_ReturnType ret = E_OK;
    if(int_obj == NULL){
        ret = E_NOT_OK;
    }
    else{
        /*Disable external interrupt*/
        ret =  Interrupt_RBx_Disable(int_obj);
        /*Clear the interrupt flag : External interrupt didn't occur*/
        ret &= INTERRUPT_RBx_CLEAR_FLAG();
        /*Configure the external interrupt I/O pin : intialize the direction to input*/
        ret &= Interrupt_RBx_Set_Pin(int_obj);
        /*Configure default interrupt CallBack function*/
        ret &= Interrupt_RBx_SetInterruptHandler(int_obj);
        /*Enable external interrupt*/
        ret &=  Interrupt_RBx_Enable(int_obj);
        /*Configure the external interrupt priority*/
        #if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_PRIORITY_ENABLE
        INTERRUPT_InterruptPriorityEnable();
        ret &= Interrupt_RBx_Set_Priority(int_obj);
        #endif
    }
    return ret;
}

Std_ReturnType Interrupt_RBx_Dinit(const interrupt_RBx_t *int_obj){
    Std_ReturnType ret = E_OK;
    if(int_obj == NULL){
        ret = E_NOT_OK;
    }
    else{
        ret = Interrupt_INTx_Disable(int_obj);
    }
    return ret;
}


static Std_ReturnType Interrupt_INTx_Enable(const interrupt_INTx_t *int_obj){
    Std_ReturnType ret = E_OK;
    if(int_obj == NULL){
        ret = E_NOT_OK;
    }
    else{
        #if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_PRIORITY_ENABLE
        /*This macro enables priority feature for interrupts*/
        INTERRUPT_InterruptPriorityEnable();
        INTERRUT_GlobalInterruptHighEnable();
        INTERRUT_GlobalInterruptLowEnable();
        #elif INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_PRIORITY_DISABLE
        INTERRUPT_InterruptPriorityDisable();
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
        #endif 
        
        if(EXT_INT0 == int_obj->INTx_source){
            INTERRUPT_INT0_ENABLE();
        }
        else if(EXT_INT1 == int_obj->INTx_source){
            INTERRUPT_INT1_ENABLE();
        }
        else if(EXT_INT2 == int_obj->INTx_source){
            INTERRUPT_INT2_ENABLE();
        }
        else{
            ret = E_NOT_OK;
        }
    }
    return ret;
}

static Std_ReturnType Interrupt_INTx_Disable(const interrupt_INTx_t *int_obj){
    Std_ReturnType ret = E_OK;
    if(int_obj == NULL){
        ret = E_NOT_OK;
    }
    else{
        if(EXT_INT0 == int_obj->INTx_source){
            INTERRUPT_INT0_DISABLE();
        }
        else if(EXT_INT1 == int_obj->INTx_source){
            INTERRUPT_INT1_DISABLE();
        }
        else if(EXT_INT2 == int_obj->INTx_source){
            INTERRUPT_INT2_DISABLE();
        }
        else{
            ret = E_NOT_OK;
        }
    }
    return ret;
}

#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_PRIORITY_ENABLE
static Std_ReturnType Interrupt_INTx_Set_Priority(const interrupt_INTx_t *int_obj){
    Std_ReturnType ret = E_OK;
    if(int_obj == NULL){
        ret = E_NOT_OK;
    }
    else{
        if(HIGH_PRIORITY == int_obj->INTx_priority){
            if(EXT_INT1 == int_obj->INTx_source){
                INTERRUPT_INT1_HIGH_PRIORITY();
            }
            else if(EXT_INT2 == int_obj->INTx_source){
                INTERRUPT_INT2_HIGH_PRIORITY();
            }
            else{
                ret = E_NOT_OK;
            }
        }
        else if(LOW_PRIORITY == int_obj->INTx_priority){
            if(EXT_INT1 == int_obj->INTx_source){
                INTERRUPT_INT1_LOW_PRIORITY();
            }
            else if(EXT_INT2 == int_obj->INTx_source){
                INTERRUPT_INT2_LOW_PRIORITY();
            }
            else{
                ret = E_NOT_OK;
            }    
        }
    }
    return ret;
}
#endif

static Std_ReturnType Interrupt_INTx_Set_Edge(const interrupt_INTx_t *int_obj){
    Std_ReturnType ret = E_OK;
    if(int_obj == NULL){
        ret = E_NOT_OK;
    }
    else{
        if(RISING_EDGE == int_obj->INTx_edge){
            if(EXT_INT0 == int_obj->INTx_source){
                INTERRUPT_INT0_RISING_EDGE();
            }
            else if(EXT_INT1 == int_obj->INTx_source){
                INTERRUPT_INT1_RISING_EDGE();
            }
            else if(EXT_INT2 == int_obj->INTx_source){
                INTERRUPT_INT2_RISING_EDGE();
            }
            else{
                ret = E_NOT_OK;
            }
        }
        else if(FALLING_EDGE == int_obj->INTx_edge){
            if(EXT_INT0 == int_obj->INTx_source){
                INTERRUPT_INT0_FALLING_EDGE();
            }
            else if(EXT_INT1 == int_obj->INTx_source){
                INTERRUPT_INT1_FALLING_EDGE();
            }
            else if(EXT_INT2 == int_obj->INTx_source){
                INTERRUPT_INT2_FALLING_EDGE();
            }
            else{
                ret = E_NOT_OK;
            }    
        }
    }
    return ret;
}

static Std_ReturnType Interrupt_INTx_Set_Pin(const interrupt_INTx_t *int_obj){
    Std_ReturnType ret = E_OK;
    if(int_obj == NULL){
        ret = E_NOT_OK;
    }
    else{
        ret = gpio_pin_direction_intialize(&(int_obj->pin));
    }
    return ret;
}

static Std_ReturnType Interrupt_INTx_Clear_Flag(const interrupt_INTx_t *int_obj){
    Std_ReturnType ret = E_OK;
    if(int_obj == NULL){
        ret = E_NOT_OK;
    }
    else{
        if(EXT_INT0 == int_obj->INTx_source){
            INTERRUPT_INT0_CLEAR_FLAG();
        }
        else if(EXT_INT1 == int_obj->INTx_source){
            INTERRUPT_INT1_CLEAR_FLAG();
        }
        else if(EXT_INT2 == int_obj->INTx_source){
            INTERRUPT_INT2_CLEAR_FLAG();
        }
        else{
            ret = E_NOT_OK;
        }
    }
    return ret;
}

Std_ReturnType Interrupt_INTx_SetInterruptHandler(const interrupt_INTx_t *int_obj){
    Std_ReturnType ret = E_OK;
    if(int_obj == NULL){
        ret = E_NOT_OK;
    }
    else{
        if(EXT_INT0 == int_obj->INTx_source){
            ret = INT0_SetInterruptHandler(int_obj->EXT_InterruptHandler);
        }
        else if(EXT_INT1 == int_obj->INTx_source){
            ret = INT1_SetInterruptHandler(int_obj->EXT_InterruptHandler);
        }
        else if(EXT_INT2 == int_obj->INTx_source){
            ret = INT2_SetInterruptHandler(int_obj->EXT_InterruptHandler);
        }
        else{
            ret = E_NOT_OK;
        }
    }
    return ret;
}

static Std_ReturnType INT0_SetInterruptHandler(void (*InterruptHandler)(void)){
    Std_ReturnType ret = E_OK;
    if(InterruptHandler == NULL){
        ret = E_NOT_OK;
    }
    else{
        INT0_InterruptHandler = InterruptHandler;
    }
    return ret;
}

static Std_ReturnType INT1_SetInterruptHandler(void (*InterruptHandler)(void)){
    Std_ReturnType ret = E_OK;
    if(InterruptHandler == NULL){
        ret = E_NOT_OK;
    }
    else{
        INT1_InterruptHandler = InterruptHandler;
    }
    return ret;
}

static Std_ReturnType  INT2_SetInterruptHandler(void (*InterruptHandler)(void)){
    Std_ReturnType ret = E_OK;
    if(InterruptHandler == NULL){
        ret = E_NOT_OK;
    }
    else{
        INT2_InterruptHandler = InterruptHandler;
    }
    return ret;
}
#endif

#if EXTERNAL_INTERRUPT_OnChange_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
static Std_ReturnType Interrupt_RBx_Enable(const interrupt_RBx_t *int_obj){
    Std_ReturnType ret = E_OK;
    if(int_obj == NULL){
        ret = E_NOT_OK;
    }
    else{
        #if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_PRIORITY_ENABLE
        /*This macro enables priority feature for interrupts*/
        INTERRUPT_InterruptPriorityEnable();
        INTERRUT_GlobalInterruptHighEnable();
        INTERRUT_GlobalInterruptLowEnable();
        #elif INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_PRIORITY_DISABLE
        INTERRUPT_InterruptPriorityDisable();
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
        #endif 
        INTERRUPT_RBx_ENABLE();
    }
    return ret;
}
static Std_ReturnType Interrupt_RBx_Disable(const interrupt_RBx_t *int_obj){
    Std_ReturnType ret = E_OK;
    if(int_obj == NULL){
        ret = E_NOT_OK;
    }
    else{
        ret = INTERRUPT_RBx_DISABLE();
    }
    return ret;
}
#if INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_PRIORITY_ENABLE
static Std_ReturnType Interrupt_RBx_Set_Priority(const interrupt_RBx_t *int_obj){
    Std_ReturnType ret = E_OK;
    if(int_obj == NULL){
        ret = E_NOT_OK;
    }
    else{
        if(int_obj->RBx_priority == HIGH_PRIORITY){
           ret = INTERRUPT_RBx_HIGH_PRIORITY();
        }
        else if(int_obj->RBx_priority == LOW_PRIORITY){
            ret = INTERRUPT_RBx_LOW_PRIORITY();
        }
    }
    return ret;
}
#endif
static Std_ReturnType Interrupt_RBx_Set_Pin(const interrupt_RBx_t *int_obj){
    Std_ReturnType ret = E_OK;
    if(int_obj == NULL){
        ret = E_NOT_OK;
    }
    else{
        ret = gpio_pin_direction_intialize(&(int_obj->pin));
    }
    return ret;
}

static Std_ReturnType  Interrupt_RBx_SetInterruptHandler(const interrupt_RBx_t *int_obj){
    Std_ReturnType ret = E_OK;
    if(int_obj == NULL){
        ret = E_NOT_OK;
    }
    else{
        if(GPIO_PIN4 == int_obj->pin.pin){
            ret = RB4_SetInterruptHandler(int_obj);
        }
        else if(GPIO_PIN5 == int_obj->pin.pin){
            ret = RB5_SetInterruptHandler(int_obj);
        }
        else if(GPIO_PIN6 == int_obj->pin.pin){
            ret = RB6_SetInterruptHandler(int_obj);
        }
        else if(GPIO_PIN7 == int_obj->pin.pin){
            ret = RB7_SetInterruptHandler(int_obj);
        }
        else{
            ret = E_NOT_OK;
        }
    }
    return ret;
}

static Std_ReturnType RB4_SetInterruptHandler(const interrupt_RBx_t *int_obj){
    Std_ReturnType ret = E_OK;
    if(int_obj == NULL){
        ret = E_NOT_OK;
    }
    else{
        RB4_high_InterruptHandler = int_obj->EXT_high_InterruptHandler;
        RB4_low_InterruptHandler = int_obj->EXT_low_InterruptHandler;
    }
    return ret;
}

static Std_ReturnType RB5_SetInterruptHandler(const interrupt_RBx_t *int_obj){
    Std_ReturnType ret = E_OK;
    if(int_obj == NULL){
        ret = E_NOT_OK;
    }
    else{
        RB5_high_InterruptHandler = int_obj->EXT_high_InterruptHandler;
        RB5_low_InterruptHandler = int_obj->EXT_low_InterruptHandler;
    }
    return ret;
}

static Std_ReturnType RB6_SetInterruptHandler(const interrupt_RBx_t *int_obj){
    Std_ReturnType ret = E_OK;
    if(int_obj == NULL){
        ret = E_NOT_OK;
    }
    else{
        RB6_high_InterruptHandler = int_obj->EXT_high_InterruptHandler;
        RB6_low_InterruptHandler = int_obj->EXT_low_InterruptHandler;
    }
    return ret;
}

static Std_ReturnType RB7_SetInterruptHandler(const interrupt_RBx_t *int_obj){
    Std_ReturnType ret = E_OK;
    if(int_obj == NULL){
        ret = E_NOT_OK;
    }
    else{
        RB7_high_InterruptHandler = int_obj->EXT_high_InterruptHandler;
        RB7_low_InterruptHandler = int_obj->EXT_low_InterruptHandler;
    }
    return ret;
}

void RB4_ISR(uint8_t RBx_source){
    /*The flag must be cleared in software*/
    INTERRUPT_RBx_CLEAR_FLAG();
    /*Execute some code*/
    
    /*Call the CallBack function "gets executed everytime this ISR executes"*/
    if(1 == RBx_source){
        RB4_high_InterruptHandler();
    }
    else if(0 == RBx_source){
        RB4_low_InterruptHandler();
    }
}

void RB5_ISR(uint8_t RBx_source){
    /*The flag must be cleared in software*/
    INTERRUPT_RBx_CLEAR_FLAG();
    /*Execute some code*/
    
    /*Call the CallBack function "gets executed everytime this ISR executes"*/
    if(1 == RBx_source){
        RB5_high_InterruptHandler();
    }
    else if(0 == RBx_source){
        RB5_low_InterruptHandler();
    }
}

void RB6_ISR(uint8_t RBx_source){
    /*The flag must be cleared in software*/
    INTERRUPT_RBx_CLEAR_FLAG();
    /*Execute some code*/
    
    /*Call the CallBack function "gets executed everytime this ISR executes"*/
    if(1 == RBx_source){
        RB6_high_InterruptHandler();
    }
    else if(0 == RBx_source){
        RB6_low_InterruptHandler();
    }
}

void RB7_ISR(uint8_t RBx_source){
    /*The flag must be cleared in software*/
    INTERRUPT_RBx_CLEAR_FLAG();
    /*Execute some code*/
    
    /*Call the CallBack function "gets executed everytime this ISR executes"*/
    if(1 == RBx_source){
        RB7_high_InterruptHandler();
    }
    else if(0 == RBx_source){
        RB7_low_InterruptHandler();
    }
}
#endif