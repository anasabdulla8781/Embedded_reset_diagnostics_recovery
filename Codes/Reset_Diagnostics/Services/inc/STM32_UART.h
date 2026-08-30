/*
 * STM32_UART.h
 *
 *  Created on: Dec 7, 2025
 *      Author: ANAS
 */

#ifndef INC_STM32_UART_H_
#define INC_STM32_UART_H_


#include <STM32_GPIO.h>
#include <STM32_RCC.h>


/// Function prototypes

typedef struct usart_structure
{
    volatile uint32_t SR;     // Status register           (offset 0x00)
    volatile uint32_t DR;     // Data register             (offset 0x04)
    volatile uint32_t BRR;    // Baud rate register        (offset 0x08)
    volatile uint32_t CR1;    // Control register 1        (offset 0x0C)
    volatile uint32_t CR2;    // Control register 2        (offset 0x10)
    volatile uint32_t CR3;    // Control register 3        (offset 0x14)
    volatile uint32_t GTPR;   // Guard time/prescaler      (offset 0x18)
} usart_structure;

#define USART2_BASEADDRESS	0x40004400U


// Macros

#define SET_OVERSAMPLING_8_SAMPLE	1
#define SET_OVERSAMPLING_16_SAMPLE	0


#define usart2_ptr		((usart_structure*)USART2_BASEADDRESS)


/// variables
extern uint8_t i;


/// Function declarations

void uart2_init(void);
void uart2_set_baud_rate(uint32_t baud_rate , uint8_t oversampling);
void uart2_enable(uint8_t oversampling);
void uart2_write_char(uint8_t charecter);
void uart_print(char* str);
uint8_t uart_read_char();
void uart_echo_test(uint8_t* ch , uint8_t size);
void itoa_custom(int32_t number , char* string);
void str_reverse_inplace(char*string);
void strcat_custom(char* buffer , char* string);


#endif /* INC_STM32_UART_H_ */
