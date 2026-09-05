/*
 * STM32_IWDG.c
 *
 *  Created on: Nov 26, 2025
 *      Author: uie83604
 */

#include <STM32_IWDG.h>


void init_independent_watchdog(volatile Watchdog_Module_Config config)
{
	volatile iwdg_structure* config_ptr = config.module_pointer;
	/// Step 1 - Enable the LSI for the watchdog module
	LSI_clock_enable();
	/// Step 2 - Enable the debug MCU module ( Ohterwise the counter will be decrementing even if we stop the controller using breakpoint )
	init_dbgmcu();

	/// Step 3 - Key for the prescalar and reload access
	config_ptr->KR = PRESCALAR_AND_RELOAD_ACCESS;						// Getting the access to prescalar and reload register
	config_ptr->PR = config.prescalar;   								// Setting the prescalar to 256
	config_ptr->RLR = config.reload_counter;							// Setting the timing for 1 second ( reset time )
//	while(iwdg_ptr->SR != 0);							// Waiting till the bit fillings are completed in the PR and RLR
	config_ptr->KR = REFRESH_WATCHDOG;						// Before starting , lets reset the reload register
	config_ptr->KR = IWDG_START;							// Starting the watchdog , counter ( RLR ) decrement is started now

}

void feed_watchdog(void)
{
	iwdg_ptr->KR = REFRESH_WATCHDOG;
}

void error_code(void)
{
//	while (1);
}
