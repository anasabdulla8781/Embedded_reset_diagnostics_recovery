/*
 * app_main.h
 *
 *  Created on: Oct 17, 2025
 *      Author: ANAS
 */

#ifndef INC_APP_MAIN_H_
#define INC_APP_MAIN_H_

#include "stddef.h"

extern uint8_t reset_count;
extern uint8_t magic_number;

/// function declarations

extern void service_init(void);
extern void app_init(void);



#endif /* INC_APP_MAIN_H_ */
