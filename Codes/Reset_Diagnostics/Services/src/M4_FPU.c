/*
 * M4_FPU.c
 *
 *  Created on: Dec 9, 2025
 *      Author: ANAS
 */



#include "M4_FPU.h"


void FPU_Enable(void)
{
	scb_ptr->CPACR |= (0xF << 20);   // enable full access to CP10 and CP11 (FPU)
}

