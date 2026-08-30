/*
 * APP_config.h
 *
 *  Created on: Jul 18, 2026
 *      Author: ANAS
 */

#ifndef INC_APP_CONFIG_H_
#define INC_APP_CONFIG_H_

#include "stdint.h"
#include "service.h"


/// Variable declartion ( Configurations )

/// GPIO
extern const GPIO_PinConfig_t gpio_pin_config[] ;
extern const uint8_t gpio_pin_config_size;

/// ADC
extern const ADC_Channel_config_t adc1_channel_config[];
extern const ADC_Channel_config_t adc2_channel_config[];
extern const uint8_t adc1_channel_count;
extern const uint8_t adc2_channel_count;

extern const ADC_Module_config_t adc_module_config[];
extern const uint8_t adc_module_config_size;

/// DMA
extern const DMA_Stream_Config dma_stream_config[];
extern const uint8_t dma_stream_count;

/// ADC Buffer
extern volatile uint32_t adc_measurement[3];

#endif /* INC_APP_CONFIG_H_ */
