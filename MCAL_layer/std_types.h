/* 
 * File:   std_types.h
 * Author: Dubai Store
 *
 * Created on September 30, 2023, 12:06 PM
 */

#ifndef STD_TYPES_H
#define	STD_TYPES_H

/*SECTION : INCLUDES*/
#include "std_libraries.h"
#include "compiler.h"

/* SECTION : MACROS DECLARATION*/
#define STD_HIGH     0X01
#define STD_LOW      0X00

#define STD_ON       0X01
#define STD_OFF      0X00

#define ENABLE       0x01
#define DISABLE      0x00

#define STD_ACTIVE   0X01
#define STD_IDEAL    0X00

#define E_OK         0x01
#define E_NOT_OK     0X00

/*SECTION : MACRO FUNCTIONS*/

/*SECTION : DATA TYPE DECLARATION*/
typedef uint8_t Std_ReturnType;   // return E_OK -> (0x01u) OR E_NOT_OK -> (0x00u)
typedef uint8_t Std_uint8;
/*FUNCTIONS DECLARATIONS*/


#endif	/* STD_TYPES_H */

