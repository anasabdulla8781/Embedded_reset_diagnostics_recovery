/*
 * STM32_IWDG.c
 *
 *  Created on: Nov 26, 2025
 *      Author: uie83604
 */

/// ***************************************** HISTORY ******************************************

/// Version 1.1	 ---------------------------   26-11-25 - Creating new file

#include <STM32_IWDG.h>


void init_independent_watchdog(void)
{
	LSI_clock_enable();
	init_dbgmcu();

	iwdg_ptr->KR = PRESCALAR_AND_RELOAD_ACCESS;			// Getting the access to prescalar and reload register
	iwdg_ptr->PR = 6;   								// Setting the prescalar to 256
	iwdg_ptr->RLR = 4000;								// Setting the timing for 1 second ( reset time )
//	while(iwdg_ptr->SR != 0);							// Waiting till the bit fillings are completed in the PR and RLR
	iwdg_ptr->KR = RESET_WATCHDOG;						// Before starting , lets reset the reload register
	iwdg_ptr->KR = IWDG_START;							// Starting the watchdog , counter ( RLR ) decrement is started now

}

void feed_watchdog(void)
{
	iwdg_ptr->KR = RESET_WATCHDOG;
}

void error_code(void)
{
//	while (1);
}
