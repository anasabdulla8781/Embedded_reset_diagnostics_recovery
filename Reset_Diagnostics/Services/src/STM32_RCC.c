/*
 * STM32_rcc_ptr.c
 *
 *  Created on: Oct 20, 2025
 *      Author: ANAS
 */


#include <STM32_RCC.h>
#include <STM32_I2C.h>
#include <STM32_SPI.h>
#include <STM32_UART.h>


// variables for checking the reset reason and CSR Register value for further debugging
char reset_reason_string[200] = {0};
char csr_register_content[50] = {0};


void LSI_clock_enable (void)
{
	rcc_ptr->CSR |= 1<<0;					/// Set the LSI clock
	while (!(rcc_ptr->CSR & (1<<1)));
}


// Function to enable the clock for UART module . We need to enable the clock of USART2 module and GPIOA module
void usart_clock_enable(uint8_t module_number)
{
	switch(module_number)
	{
	case USART_MODULE2:
		rcc_ptr->APB1ENR |= 1<<17;				/// Set the USART2 clock enable bit
		break;
	default:
		break;
	}
}


void i2c_clock_enable(uint8_t module)
{
	if (module == I2C_1)
	{
		rcc_ptr->APB1ENR |= (1<<21);						/// Enabled the clock for I2C1 Module
	}
	else if(module == I2C_2)
	{
		rcc_ptr->APB1ENR |= (1<<22);						/// Enabled the clock for I2C2 Module
	}
	else if(module == I2C_3)
	{
		rcc_ptr->APB1ENR |= (1<<23);						/// Enabled the clock for I2C3 Module
	}
	else
	{
		/// Do nothing
	}
}

void gpio_clock_enable(uint8_t gpio_module)
{

	if (gpio_module >= GPIOA && gpio_module <= GPIOI)
	{
		rcc_ptr->AHB1ENR |= 1<<(GPIO_CLOCK_ENABLE + gpio_module);							//// Enabled the clock for GPIO Module
	}
	else
	{
		;
	}
}


void adc_clock_enable(uint8_t adc_module)
{

	if (( adc_module <= ADC3) && (adc_module>=ADC1) )
	{
		rcc_ptr->APB2ENR |= (1 << (adc_module + ADC_CLOCK_ENABLE));   							/// Enabled ADC clock
	}
	else
	{
		;
	}
}

void spi_clock_enable(uint8_t module)
{
	switch(module)
	{
	case SPI1:
		rcc_ptr->APB2ENR |= (1<<12);
		break;
	case SPI2:
		rcc_ptr->APB1ENR |= (1<<14);
		break;
	case SPI3:
		rcc_ptr->APB1ENR |= (1<<15);
		break;
	default:
		break;
	}
}

void reset_reason_check( char *reset_reason_string)
{
    uint32_t csr = rcc_ptr->CSR;
    itoa_custom(csr , csr_register_content);

    char *message;
    uint8_t counter = 0;							// Counter to count how many reset reasons are there

    for (uint8_t i= 25; i<31; i++)
    {
    	if (csr & (1<<i))
    	{
    		switch (i)
    		{
    			case 25:
    				message = "BROWN_OUT_RESET";
    				counter++;
    				break;
    			case 26:
    				message = "RESET_BUTTON_RESET";
    				counter++;
    				break;
    			case 27:
    				message = "POWER_ON_RESET";
    				counter++;
    				break;
    			case 28:
    				message = "SOFTWARE_RESET";
    				counter++;
    				break;
    			case 29:
    				message = "INDEPENDENT_WATCHDOG_RESET";
    				counter++;
    				break;
    			case 30:
    				message = "WINDOW_WATCHDOG_RESET";
    				counter++;
    				break;
    			default:
    				message = "UNKNOWN_RESET";
    				counter++;
    				break;
    		}
    		if (counter >1)
    		{
    			*reset_reason_string++ = '&';
    		}
    		while (*message != '\0')
    		{
    		    *reset_reason_string++ = *message;
    		    *message = '\0';
    		    message++;
    		}
    	}
    	else
    	{
    		// No reset
    	}
    }
    // Clearing the CSR Register
    rcc_ptr->CSR |= (1U << 24);
}

void clock_enable_HSE(void)											/// Default HSE Clock is coming with 8MHZ . that improves the accuracy
{
	// 1 . Enabling the HSE Clock
	rcc_ptr->CR |= 1<<16;

	// 2 . Wait until HSE oscillater based clock is ready
	while (!(rcc_ptr->CR & (1 << 17)));

    // 3. Clear SW[1:0] bits
    rcc_ptr->CFGR &= ~(3 << 0);

    // 4. Select HSE (01)
    rcc_ptr->CFGR |= (1 << 0);

    // 5. Wait until switch is done (VERY IMPORTANT)
    while (!((rcc_ptr->CFGR &  0x0C ) == 0x04));
}

void syscfg_clock_enable(void)
{
	rcc_ptr->APB2ENR |= (1<<14);
}

void dma_clock_enable(uint8_t module)
{
	rcc_ptr->AHB1ENR |= 1<< (20 + module);
}


void print_reset_reason (void)
{
	/// Temporary buffer to store what to print
	char temporary_buffer[100] = {0};

	// Printing the Reset reason
	strcat_custom(temporary_buffer , "        ");
	strcat_custom(temporary_buffer , reset_reason_string);
	strcat_custom(temporary_buffer , "        ");
	uart_write(temporary_buffer);
	uart_write("========================================");
	temporary_buffer[0] = '\0';

//	 Printing the CSR Register content
	strcat_custom(temporary_buffer , "        ");
	strcat_custom(temporary_buffer , "CSR_ADDRESS - ");
	strcat_custom(temporary_buffer , csr_register_content);
	strcat_custom(temporary_buffer , "        ");
	uart_write(temporary_buffer);
	uart_write("========================================");
	temporary_buffer[0] = '\0';
}




