/*
 * APP_config.c
 *
 *  Created on: Jul 18, 2026
 *      Author: ANAS
 *
 *      File for init conifigurations for the project
 */


#include "APP_config.h"


/// ADC Buffer initialisation
volatile uint32_t adc_measurement[3] = {0u};

/// The gpio pins needed to be configured for this project
const GPIO_PinConfig_t gpio_pin_config[] = {

		{	gpioa_ptr,	PIN0,	PIN_ANALOG_INPUT,	PORTA	},
		{	gpioa_ptr,	PIN1,	PIN_ANALOG_INPUT,	PORTA	},
		{	gpioa_ptr,	PIN3,	PIN_ANALOG_INPUT,	PORTA	},
		{	gpiob_ptr,	PIN1,	PIN_ANALOG_INPUT,	PORTB	},
};

const uint8_t gpio_pin_config_size = sizeof(gpio_pin_config)/sizeof(gpio_pin_config[0]);


// ***********************************************************************************************
/// The adc channels needed to be configured for this project
const ADC_Channel_config_t adc1_channel_config[] =
{
		{CHANNEL_1	, SAMPLES_480},
		{CHANNEL_3	, SAMPLES_480},
};
const ADC_Channel_config_t adc2_channel_config[] =
{
		{CHANNEL_9	, SAMPLES_480},
};

const uint8_t adc1_channel_count = sizeof(adc1_channel_config)/sizeof(adc1_channel_config[0]);
const uint8_t adc2_channel_count = sizeof(adc2_channel_config)/sizeof(adc2_channel_config[0]);

/// The ADC Modules needed to be configured for this project
const ADC_Module_config_t adc_module_config[] = {
		{	ADC1,	adc1_ptr,	ADC_RIGHT_ALIGN,	adc1_channel_config,	adc1_channel_count,	SCAN_MODE_ENABLED,	CONITNUOUS_MODE_ENABLED,	ADC_ENABLED,	EOC_AFTER_EACH_CONVERSION,	DMA_ENABLE,	},
		{	ADC2,	adc2_ptr,	ADC_RIGHT_ALIGN,	adc2_channel_config,	adc2_channel_count,	SCAN_MODE_DISABLED,	CONITNUOUS_MODE_DISABLED,	ADC_ENABLED,	EOC_AFTER_EACH_CONVERSION,	DMA_ENABLE,	},
};

const uint8_t adc_module_config_size = sizeof(adc_module_config)/sizeof(adc_module_config[0]);

// *******************************************************************************************
/// The dma streams to be configured for this project
const DMA_Stream_Config dma_stream_config[] = {
		{	DMA2,	dma2_ptr ,	STREAM0,	DMA2_STREAM0_ADC1,	PERIPHERAL_TO_MEMORY,	&(adc1_ptr->DR),	&adc_measurement[0],	adc1_channel_count,	PERIPHERAL_DATA_REG_32BIT,	MEMORY_SIZE_16BIT,	MEMORY_INCREMENT_ENABLE,	PERIPHERAL_INCREMENT_DISABLE,	CIRCULAR_MODE_ENABLE,	HIGH_PRIORITY,},
		{	DMA2,	dma2_ptr ,	STREAM3,	DMA2_STREAM3_ADC2,	PERIPHERAL_TO_MEMORY,	&(adc2_ptr->DR),	&adc_measurement[2],	adc2_channel_count,	PERIPHERAL_DATA_REG_32BIT,	MEMORY_SIZE_16BIT,	MEMORY_INCREMENT_ENABLE,	PERIPHERAL_INCREMENT_DISABLE,	CIRCULAR_MODE_ENABLE,	HIGH_PRIORITY,},
};

const uint8_t dma_stream_count = sizeof (dma_stream_config) / sizeof(dma_stream_config[0]);


