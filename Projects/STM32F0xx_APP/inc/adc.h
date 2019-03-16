#ifndef __ADC_H
#define __ADC_H

#include "stm32f0xx.h"

#define ADC1_DR_Address    0x40012440

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern __IO uint16_t  ADC1ConvertedValue;

void prvAdcTask( void *pvParameters );

#endif