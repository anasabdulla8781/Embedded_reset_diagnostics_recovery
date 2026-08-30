/*
 * STM32_IWDG.h
 *
 *  Created on: Nov 26, 2025
 *      Author: uie83604
 */

/// ***************************************** HISTORY ******************************************

/// Version 1.1	 ---------------------------   26-11-25 - Creating new file

#ifndef INC_STM32_IWDG_H_
#define INC_STM32_IWDG_H_

#include <stdint.h>
#include "STM32_DBGMCU.h"
#include "STM32_RCC.h"


typedef struct
{
    volatile uint32_t KR;   // 0x00  Key Register
    volatile uint32_t PR;   // 0x04  Prescaler Register
    volatile uint32_t RLR;  // 0x08  Reload Register
    volatile uint32_t SR;   // 0x0C  Status Register
} iwdg_structure;


/// Base address

#define IWDG_BASEADDRESS	0x40003000

/// pointer to the structure

#define iwdg_ptr ((volatile iwdg_structure*) IWDG_BASEADDRESS)


// Macros

#define PRESCALAR_AND_RELOAD_ACCESS		0x5555
#define IWDG_START						0xCCCC
#define RESET_WATCHDOG					0xAAAA


// Function declarations
extern void init_independent_watchdog(void);
extern void feed_watchdog(void);
extern void error_code(void);

#endif /* INC_STM32_IWDG_H_ */
