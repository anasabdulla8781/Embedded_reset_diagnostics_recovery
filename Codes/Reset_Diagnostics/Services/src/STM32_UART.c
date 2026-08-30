/*
 * STM32_UART.c
 *
 *  Created on: Dec 7, 2025
 *      Author: ANAS
 */


#include "STM32_UART.h"

uint8_t i;

void uart2_init(void)
{
	// Step 1 - Enable the clocks
	usart2_clock_enable();
	// Step 2.1 - Set the GPIO Pins in alternalte fucntions mode for UART
	gpio_pin_set_mode(2,PIN_ALTERNATE_FUNCTION,gpioa_ptr);
	gpio_pin_set_mode(3,PIN_ALTERNATE_FUNCTION,gpioa_ptr);
	// Step 2.2 - Set the alternate function corresponding to UART for PA2 and PA3
	gpioa_ptr->AFRL	&= ~( 0x0F << 8  );	 		/// Clearing the bits
	gpioa_ptr->AFRL |= ( 7 << 8 );				/// Setting into AF7 mode
	gpioa_ptr->AFRL &= ~( 0x0F << 12  );	 	/// Clearing the bits
	gpioa_ptr->AFRL |= ( 7 << 12 );				/// Setting into AF7 mode
	// Step 3 - Set the baud rate
	uart2_set_baud_rate(9600,0);
	// Step 4 - Enable UART
	uart2_enable(0);
}

void uart2_set_baud_rate(uint32_t baud_rate , uint8_t oversampling)
{
	uint32_t internal_clock = 8000000;																	// 16Mhz clock
	float usartdiv = (float)internal_clock / ((float)baud_rate * (8 * (2 - oversampling)));				// Finding the usartdiv using the equation
	uint32_t mantassa = (uint32_t)usartdiv;
	uint32_t fraction;
	if (oversampling == 1)		/// for oversampling of 8
	{
		fraction = (uint32_t)(( (float)(usartdiv - mantassa) * 8) + 0.5);
		fraction &= 0x07;      // keep only 3 bits
	}
	else if (oversampling == 0)/// for oversampling of 16
	{
		fraction = (uint32_t)(( (float)(usartdiv - mantassa) * 16) + 0.5);
		fraction &= 0x0F;      // keep only 4 bits
	}
	usart2_ptr->BRR = 0x0000;
	usart2_ptr->BRR |= (mantassa << 4);
	usart2_ptr->BRR |= fraction;

}

void uart2_enable(uint8_t oversampling)
{
	/// Step 1 - Clearing CR Register for first time
	usart2_ptr->CR1 = 0;
	/// Step 2 - Setting the oversampling mode
	usart2_ptr->CR1 |= (oversampling << 15);
	/// Step 3 - Enabling UART Rx and Tx
	usart2_ptr->CR1 |=  1<<3;				/// Setting the uart TX
	usart2_ptr->CR1 |=  1<<2;				/// Setting the uart RX
	/// Step 4 - Enabling UART module
	usart2_ptr->CR1 |=  1<<13;				/// Setting the uart enable bit
}

// Step 5 - Function for writing the data
void uart2_write_char(uint8_t charecter)
{
	while (!(usart2_ptr->SR & (1<<7)));
	usart2_ptr->DR = charecter;
}

// Step 6 - Function for writing the data as the whole string
void uart_print(char* str)
{
	while (*str)
	{
		uart2_write_char(*str++);
	}
	uart2_write_char('\r');
	uart2_write_char('\n');
}

// Step 7 - Function for reading from UART
uint8_t uart_read_char()
{
	while (!((usart2_ptr->SR) & (1 << 5)));
	return (uint8_t)(usart2_ptr->DR);
}


void uart_echo_test(uint8_t* ch , uint8_t size)
{
    while (!(usart2_ptr->SR & (1<<5)));

    uint8_t c = (uint8_t)usart2_ptr->DR;

    if (c == '\r')
    {
        ch[i] = '\0';   // ✅ terminate at current index

        uart2_write_char('\r');
        uart2_write_char('\n');

        uart_print((char*)ch);

        i = 0;
    }
    else
    {
        if (i < (size - 1))
        {
            ch[i++] = c;     // store safely
        }

        uart2_write_char(c); // echo anyway
    }
}


void itoa_custom(int32_t number , char* string)
{
	/// The idea is , we will have a buffer called string , lets have it with size 100 . we will pass it to the function
	/// we will have the number as well .. that also we will pass

	/// We are making one by one byte to charecters .. so we will start from LSB .. thats easy approach
	uint8_t number_is_negative = 0;
	char last_digit;
	uint8_t index = 0;

	/// handling for negative number
	if (number < 0)
	{
		/// For a negative number , we will set it as positive for the moment ... for doing all the calculations .. but once every calculaiton is over , then we can consider the fact that it was negative
		number = -number ;
		/// We need to remember that it was a negative number .. then only we can consider it at the end
		number_is_negative = 1;
	}
	else if (number == 0)
	{
		string[index++] = '0';
		string[index] = '\0';
		return;

	}

	/// as long as we have some values , we will keep on converting each of the last bytes to charecters and store it
	while (number)
	{
		/// We took the last byte and converted it into the charecter
		last_digit = ((number %10) + '0');
		/// Added the last digit to the string array
		string[index ++] = last_digit;
		/// We removed the last byte from the number
		number = number /10;
	}

	/// We added all the bytes , make the string buffer to proper string
	if (number == 0)
	{
		//// Adding the sign also at the end ( We are adding each bytes from LSB . so the numbers will be in reverse order )
		if (number_is_negative)
		{
			string[index++] = '-';
		}
		/// adding the string ending charecter in the end
		string[index] = '\0';
	}

	/// Now we need to reverse the string
	str_reverse_inplace(string);
}

void str_reverse_inplace(char*string)
{
	char temp;
	int i = 0, j  = 0;

	while (string[j] != '\0')
	{
		/// We reached the end of the string . now j is the location of '\0' and i is in the starting
		j++;
	}
	/// we are not going to reverse the null charecter .. so moved j one step back
	j = j-1;

	/// interchange the charecters in such a way that i should be always less than J
	while (i < j)
	{
		/// Interchange the charecters
		temp = string[i];
		string[i] = string[j];
		string[j] = temp;
		/// Moving the indexes in both direction
		i++;
		j--;
	}
}

void strcat_custom(char* buffer , char* string)
{
	uint8_t index_buffer = 0;
	uint8_t index_string = 0;

	while (buffer[index_buffer] != '\0')
	{
		index_buffer ++;
	}

	while (string[index_string] != '\0')
	{
		buffer[index_buffer++] = string[index_string++];
	}
	buffer[index_buffer] = '\0';
}
